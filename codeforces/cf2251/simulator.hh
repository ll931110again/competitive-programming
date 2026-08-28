#pragma once

#include "scheduler.hh"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace edge_cloud {

struct ScenarioRequest {
  double arr = 0;
  int lin = 1;
  int lout = 1;
};

struct Scenario {
  Config cfg;
  std::vector<ScenarioRequest> reqs;
};

struct SimResult {
  bool ok = false;
  bool stuck = false;
  std::string error;
  ScoreBreakdown score;
  int frames = 0;
  double last_time = 0;
  int64_t tokens = 0;
};

inline Scenario example1() {
  Scenario sc;
  sc.cfg.k = 1;
  sc.cfg.s = 1;
  sc.cfg.latency_ms = 2;
  sc.cfg.bandwidth_gbps = 1;
  sc.cfg.bytes_per_token = 125000;
  sc.cfg.num_layers = 4;
  sc.cfg.slo1 = 30;
  sc.cfg.slo2 = 15;
  sc.cfg.tp_ub = 0.0625;
  sc.cfg.tp_base = 0.022222222;
  sc.cfg.dist_base = 0;
  sc.cfg.w_tp = 0.5;
  sc.cfg.w_c = 0.5;
  const double row1[] = {3, 10, 2, 1, 4, 1};
  const double row4[] = {3, 10, 2, 1, 4, 1};
  sc.cfg.table.add_row(1, row1);
  sc.cfg.table.add_row(4, row4);
  sc.cfg.table.finalize();
  sc.reqs.push_back({0.0, 4, 1});
  return sc;
}

class Simulator {
public:
  SimResult run(const Scenario& scenario, Scheduler& sched) {
    reset(scenario);
    sched.init(scenario.cfg);
    try {
      while (!pq_.empty()) {
        const double t = pq_.top().t;
        std::vector<Ev> batch;
        while (!pq_.empty() && pq_.top().t == t) {
          batch.push_back(pq_.top());
          pq_.pop();
        }
        std::sort(
            batch.begin(), batch.end(), [](const Ev& a, const Ev& b) { return a.seq < b.seq; });
        std::vector<FrameEvent> frame;
        for (const Ev& ev : batch) {
          apply_event(t, ev, frame);
        }
        result_.frames++;
        result_.last_time = t;
        const std::vector<Assignment> asg = sched.on_frame(frame, t);
        const std::string err = start_assignments(t, asg);
        if (!err.empty()) {
          result_.error = err;
          result_.ok = false;
          return result_;
        }
        if (unfinished() && pq_.empty() && !link_busy()) {
          result_.stuck = true;
          result_.error = "stuck: no future event with unfinished requests";
          result_.ok = false;
          return result_;
        }
      }
      if (unfinished()) {
        result_.stuck = true;
        result_.error = "stuck: queue drained with unfinished requests";
        result_.ok = false;
        return result_;
      }
      result_.ok = true;
      result_.score = make_score();
      return result_;
    } catch (const std::exception& ex) {
      result_.ok = false;
      result_.error = ex.what();
      return result_;
    }
  }

private:
  enum class RNeed {
    None,
    PPre,
    PProc,
    PPost,
    DPre,
    DProc,
    DPost,
    Done,
  };

  struct RState {
    double arr = 0;
    int lin = 1;
    int lout = 1;
    int cloud = -1;
    int next_ls = 0;
    int tokens = 0;
    RNeed need = RNeed::None;
    bool in_flight = false;
    bool arrived = false;
    double tdr_end = -1;
    std::vector<double> token_times;
  };

  struct Transfer {
    bool up = true;
    int cloud = 0;
    int64_t size = 0;
    bool pre = true;
    std::vector<int> rids;
    double dur = 0;
  };

  struct Link {
    bool busy = false;
    std::queue<Transfer> waiting;
  };

  struct Ev {
    double t = 0;
    int seq = 0;
    int kind = 0; // 0 ARR, 1 TDN, 2 XDN
    int rid = 0;
    Assignment asg;
    Transfer xfer;
  };

  struct EvCmp {
    bool operator()(const Ev& a, const Ev& b) const {
      if (a.t != b.t) {
        return a.t > b.t;
      }
      return a.seq > b.seq;
    }
  };

  Scenario sc_;
  SimResult result_;
  std::vector<RState> rs_;
  bool edge_busy_ = false;
  std::vector<char> cloud_busy_;
  Link up_;
  Link down_;
  int seq_ = 0;
  std::priority_queue<Ev, std::vector<Ev>, EvCmp> pq_;

  static std::string fmt9(double x) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(9) << x;
    return os.str();
  }

  void reset(const Scenario& scenario) {
    sc_ = scenario;
    result_ = {};
    rs_.assign(sc_.reqs.size(), {});
    for (int i = 0; i < static_cast<int>(sc_.reqs.size()); i++) {
      rs_[i].arr = sc_.reqs[i].arr;
      rs_[i].lin = sc_.reqs[i].lin;
      rs_[i].lout = sc_.reqs[i].lout;
    }
    edge_busy_ = false;
    cloud_busy_.assign(sc_.cfg.k, false);
    up_ = {};
    down_ = {};
    seq_ = 0;
    pq_ = {};
    for (int i = 0; i < static_cast<int>(sc_.reqs.size()); i++) {
      Ev ev;
      ev.t = sc_.reqs[i].arr;
      ev.seq = seq_++;
      ev.kind = 0;
      ev.rid = i;
      pq_.push(ev);
    }
  }

  bool unfinished() const {
    for (const auto& r : rs_) {
      if (r.need != RNeed::Done) {
        return true;
      }
    }
    return false;
  }

  bool link_busy() const {
    return up_.busy || down_.busy;
  }

  double task_dur(const Assignment& a) const {
    const auto& tbl = sc_.cfg.table;
    switch (a.kind) {
    case Assignment::Kind::PPre:
      return tbl.lookup(Step::PrefillPre, rs_[a.rid].lin);
    case Assignment::Kind::PProc: {
      const double full = tbl.lookup(Step::PrefillProc, rs_[a.rid].lin);
      return full * double(a.le - a.ls) / double(sc_.cfg.num_layers);
    }
    case Assignment::Kind::PPost:
      return tbl.lookup(Step::PrefillPost, rs_[a.rid].lin);
    case Assignment::Kind::DPre:
      return tbl.lookup(Step::DecodePre, static_cast<int>(a.rids.size()));
    case Assignment::Kind::DProc:
      return tbl.lookup(Step::DecodeProc, static_cast<int>(a.rids.size()));
    case Assignment::Kind::DPost:
      return tbl.lookup(Step::DecodePost, static_cast<int>(a.rids.size()));
    }
    return 1;
  }

  double xfer_dur(int len) const {
    return transfer_ms(sc_.cfg.latency_ms, sc_.cfg.bandwidth_gbps, sc_.cfg.bytes_per_token, len);
  }

  std::string tdn_line(const Assignment& a, double dur) const {
    std::ostringstream os;
    os << "TDN " << a.format() << " " << fmt9(dur);
    return os.str();
  }

  std::string xdn_line(const Transfer& tr) const {
    std::ostringstream os;
    os << "XDN " << (tr.up ? "UP" : "DOWN") << " " << tr.cloud << " " << tr.size << " "
       << (tr.pre ? "PRE" : "DEC") << " " << static_cast<int>(tr.rids.size());
    for (int id : tr.rids) {
      os << " " << id;
    }
    return os.str();
  }

  void enqueue_link(Link& link, bool up, Transfer tr, double now) {
    tr.up = up;
    if (!link.busy) {
      start_link(link, up, std::move(tr), now);
    } else {
      link.waiting.push(std::move(tr));
    }
  }

  void start_link(Link& link, bool up, Transfer tr, double now) {
    link.busy = true;
    Ev ev;
    ev.t = now + tr.dur;
    ev.seq = seq_++;
    ev.kind = 2;
    ev.xfer = std::move(tr);
    ev.xfer.up = up;
    pq_.push(ev);
  }

  void complete_link(Link& link, bool up, double now) {
    if (!link.waiting.empty()) {
      Transfer nxt = std::move(link.waiting.front());
      link.waiting.pop();
      start_link(link, up, std::move(nxt), now);
    } else {
      link.busy = false;
    }
  }

  void queue_prefill_up(int rid, double now) {
    Transfer tr;
    tr.cloud = rs_[rid].cloud;
    tr.size = int64_t(rs_[rid].lin) * sc_.cfg.bytes_per_token;
    tr.pre = true;
    tr.rids = {rid};
    tr.dur = xfer_dur(rs_[rid].lin);
    enqueue_link(up_, true, std::move(tr), now);
  }

  void queue_prefill_down(int rid, double now) {
    Transfer tr;
    tr.cloud = rs_[rid].cloud;
    tr.size = int64_t(rs_[rid].lin) * sc_.cfg.bytes_per_token;
    tr.pre = true;
    tr.rids = {rid};
    tr.dur = xfer_dur(rs_[rid].lin);
    enqueue_link(down_, false, std::move(tr), now);
  }

  void queue_decode_ups(const std::vector<int>& rids, double now) {
    std::vector<std::vector<int>> by_cloud(sc_.cfg.k);
    for (int rid : rids) {
      by_cloud[rs_[rid].cloud].push_back(rid);
    }
    for (int c = 0; c < sc_.cfg.k; c++) {
      if (by_cloud[c].empty()) {
        continue;
      }
      Transfer tr;
      tr.cloud = c;
      tr.size = int64_t(by_cloud[c].size()) * sc_.cfg.bytes_per_token;
      tr.pre = false;
      tr.rids = by_cloud[c];
      tr.dur = xfer_dur(static_cast<int>(by_cloud[c].size()));
      enqueue_link(up_, true, std::move(tr), now);
    }
  }

  void queue_decode_down(const std::vector<int>& rids, int cloud, double now) {
    Transfer tr;
    tr.cloud = cloud;
    tr.size = int64_t(rids.size()) * sc_.cfg.bytes_per_token;
    tr.pre = false;
    tr.rids = rids;
    tr.dur = xfer_dur(static_cast<int>(rids.size()));
    enqueue_link(down_, false, std::move(tr), now);
  }

  void apply_event(double t, const Ev& ev, std::vector<FrameEvent>& frame) {
    FrameEvent fe;
    if (ev.kind == 0) {
      auto& r = rs_[ev.rid];
      r.arrived = true;
      r.need = RNeed::PPre;
      fe.type = FrameEvent::Type::Arr;
      fe.raw = "ARR " + std::to_string(ev.rid) + " " + std::to_string(r.lin);
      frame.push_back(fe);
      return;
    }
    if (ev.kind == 2) {
      const Transfer& tr = ev.xfer;
      for (int rid : tr.rids) {
        auto& r = rs_[rid];
        r.in_flight = false;
        if (tr.pre) {
          r.need = tr.up ? RNeed::PProc : RNeed::PPost;
        } else {
          r.need = tr.up ? RNeed::DProc : RNeed::DPost;
        }
      }
      if (tr.up) {
        complete_link(up_, true, t);
      } else {
        complete_link(down_, false, t);
      }
      fe.type = FrameEvent::Type::Xdn;
      fe.raw = xdn_line(tr);
      frame.push_back(fe);
      return;
    }

    const Assignment& a = ev.asg;
    const double dur = task_dur(a);
    if (a.server < 0) {
      edge_busy_ = false;
    } else {
      cloud_busy_[a.server] = false;
    }
    if (a.kind == Assignment::Kind::PPre) {
      rs_[a.rid].in_flight = false;
      rs_[a.rid].need = RNeed::None;
      queue_prefill_up(a.rid, t);
    } else if (a.kind == Assignment::Kind::PProc) {
      auto& r = rs_[a.rid];
      r.in_flight = false;
      r.next_ls = a.le;
      if (a.le < sc_.cfg.num_layers) {
        r.need = RNeed::PProc;
      } else {
        r.need = RNeed::None;
        queue_prefill_down(a.rid, t);
      }
    } else if (a.kind == Assignment::Kind::PPost) {
      auto& r = rs_[a.rid];
      r.in_flight = false;
      r.need = RNeed::DPre;
      r.tdr_end = t;
    } else if (a.kind == Assignment::Kind::DPre) {
      for (int rid : a.rids) {
        rs_[rid].in_flight = false;
        rs_[rid].need = RNeed::None;
      }
      queue_decode_ups(a.rids, t);
    } else if (a.kind == Assignment::Kind::DProc) {
      for (int rid : a.rids) {
        rs_[rid].in_flight = false;
        rs_[rid].need = RNeed::None;
      }
      queue_decode_down(a.rids, a.cloud, t);
    } else if (a.kind == Assignment::Kind::DPost) {
      for (int rid : a.rids) {
        auto& r = rs_[rid];
        r.in_flight = false;
        r.tokens++;
        r.token_times.push_back(t);
        if (r.tokens >= r.lout) {
          r.need = RNeed::Done;
        } else {
          r.need = RNeed::DPre;
        }
      }
    }
    fe.type = FrameEvent::Type::Tdn;
    fe.raw = tdn_line(a, dur);
    frame.push_back(fe);
    if (a.kind == Assignment::Kind::DPost) {
      for (int rid : a.rids) {
        if (rs_[rid].need == RNeed::Done) {
          FrameEvent fin;
          fin.type = FrameEvent::Type::Fin;
          fin.raw = "FIN " + std::to_string(rid);
          frame.push_back(fin);
        }
      }
    }
  }

  static bool unique_ids(const std::vector<int>& ids) {
    std::unordered_set<int> seen;
    for (int id : ids) {
      if (!seen.insert(id).second) {
        return false;
      }
    }
    return true;
  }

  std::string check_rid(int rid, RNeed need) const {
    if (rid < 0 || rid >= static_cast<int>(rs_.size()) || !rs_[rid].arrived) {
      return "unknown rid " + std::to_string(rid);
    }
    const auto& r = rs_[rid];
    if (r.need == RNeed::Done) {
      return "finished rid " + std::to_string(rid);
    }
    if (r.in_flight) {
      return "in-flight rid " + std::to_string(rid);
    }
    if (r.need != need) {
      return "rid " + std::to_string(rid) + " at the wrong step";
    }
    return {};
  }

  std::string validate(const Assignment& a) const {
    if (a.server < -1 || a.server >= sc_.cfg.k) {
      return "bad server";
    }
    const bool edge = a.server < 0;
    if (edge ? edge_busy_ : cloud_busy_[a.server]) {
      return "busy server for " + a.format();
    }
    switch (a.kind) {
    case Assignment::Kind::PPre: {
      if (!edge) {
        return "P PRE must run on the edge";
      }
      if (a.cloud < 0 || a.cloud >= sc_.cfg.k) {
        return "P PRE cloud out of range";
      }
      return check_rid(a.rid, RNeed::PPre);
    }
    case Assignment::Kind::PProc: {
      if (edge) {
        return "P PROC must run on a cloud";
      }
      const std::string err = check_rid(a.rid, RNeed::PProc);
      if (!err.empty()) {
        return err;
      }
      const auto& r = rs_[a.rid];
      if (a.server != r.cloud || a.cloud != r.cloud) {
        return "P PROC cloud mismatch";
      }
      if (a.ls != r.next_ls || a.le <= a.ls || a.le > sc_.cfg.num_layers || a.ls < 0) {
        return "illegal P PROC piece";
      }
      return {};
    }
    case Assignment::Kind::PPost: {
      if (!edge) {
        return "P POST must run on the edge";
      }
      const std::string err = check_rid(a.rid, RNeed::PPost);
      if (!err.empty()) {
        return err;
      }
      if (a.cloud != rs_[a.rid].cloud) {
        return "P POST cloud mismatch";
      }
      return {};
    }
    case Assignment::Kind::DPre:
    case Assignment::Kind::DPost: {
      if (!edge) {
        return "decode PRE/POST must run on the edge";
      }
      if (a.rids.empty() || !unique_ids(a.rids)) {
        return "malformed decode group";
      }
      const RNeed need = a.kind == Assignment::Kind::DPre ? RNeed::DPre : RNeed::DPost;
      for (int rid : a.rids) {
        const std::string err = check_rid(rid, need);
        if (!err.empty()) {
          return err;
        }
      }
      return {};
    }
    case Assignment::Kind::DProc: {
      if (edge) {
        return "D PROC must run on a cloud";
      }
      if (a.rids.empty() || !unique_ids(a.rids) || a.cloud != a.server) {
        return "malformed D PROC";
      }
      for (int rid : a.rids) {
        const std::string err = check_rid(rid, RNeed::DProc);
        if (!err.empty()) {
          return err;
        }
        if (rs_[rid].cloud != a.server) {
          return "D PROC member on the wrong cloud";
        }
      }
      return {};
    }
    }
    return "unknown assignment kind";
  }

  std::string start_assignments(double t, const std::vector<Assignment>& asg) {
    if (static_cast<int>(asg.size()) > sc_.cfg.k + 1) {
      return "too many assignments";
    }
    std::vector<char> used_cloud(sc_.cfg.k, 0);
    bool used_edge = false;
    for (const auto& a : asg) {
      const std::string err = validate(a);
      if (!err.empty()) {
        return err;
      }
      if (a.server < 0) {
        if (used_edge) {
          return "two edge tasks in one response";
        }
        used_edge = true;
      } else {
        if (used_cloud[a.server]) {
          return "two tasks on one cloud in one response";
        }
        used_cloud[a.server] = 1;
      }
    }
    for (const auto& a : asg) {
      const double dur = task_dur(a);
      if (!(dur > 0)) {
        return "non-positive duration";
      }
      if (a.kind == Assignment::Kind::PPre) {
        rs_[a.rid].cloud = a.cloud;
        rs_[a.rid].in_flight = true;
        rs_[a.rid].need = RNeed::None;
        edge_busy_ = true;
      } else if (a.kind == Assignment::Kind::PProc) {
        rs_[a.rid].in_flight = true;
        rs_[a.rid].need = RNeed::None;
        cloud_busy_[a.server] = true;
      } else if (a.kind == Assignment::Kind::PPost) {
        rs_[a.rid].in_flight = true;
        rs_[a.rid].need = RNeed::None;
        edge_busy_ = true;
      } else {
        for (int rid : a.rids) {
          rs_[rid].in_flight = true;
          rs_[rid].need = RNeed::None;
        }
        if (a.server < 0) {
          edge_busy_ = true;
        } else {
          cloud_busy_[a.server] = true;
        }
      }
      Ev ev;
      ev.t = t + sc_.cfg.s + dur;
      ev.seq = seq_++;
      ev.kind = 1;
      ev.asg = a;
      pq_.push(ev);
    }
    return {};
  }

  ScoreBreakdown make_score() {
    double earliest = rs_.empty() ? 0 : rs_[0].arr;
    double last = 0;
    double tdr_sum = 0;
    double tpot_sum = 0;
    int64_t tpot_n = 0;
    int64_t tokens = 0;
    for (const auto& r : rs_) {
      earliest = std::min(earliest, r.arr);
      tdr_sum += r.tdr_end - r.arr;
      tokens += r.lout;
      for (double tt : r.token_times) {
        last = std::max(last, tt);
      }
      for (std::size_t i = 1; i < r.token_times.size(); i++) {
        tpot_sum += r.token_times[i] - r.token_times[i - 1];
        tpot_n++;
      }
    }
    result_.tokens = tokens;
    const double mean_tdr = rs_.empty() ? 0 : tdr_sum / double(rs_.size());
    const double mean_tpot = tpot_n == 0 ? 0 : tpot_sum / double(tpot_n);
    return compute_score(sc_.cfg.tp_base,
                         sc_.cfg.tp_ub,
                         sc_.cfg.slo1,
                         sc_.cfg.slo2,
                         sc_.cfg.dist_base,
                         sc_.cfg.w_tp,
                         sc_.cfg.w_c,
                         earliest,
                         last,
                         mean_tdr,
                         mean_tpot,
                         tokens);
  }
};

} // namespace edge_cloud
