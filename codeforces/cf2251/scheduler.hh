#pragma once

#include "task_table.hh"

#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace edge_cloud {

enum class Need { Idle, PPre, PProc, PPost, DPre, DProc, DPost };
enum class Mode { Slo, Latency, Throughput };

struct Assignment {
  enum class Kind { PPre, PProc, PPost, DPre, DProc, DPost };
  Kind kind = Kind::PPre;
  int server = -1;
  int cloud = 0;
  int rid = -1;
  int ls = 0;
  int le = 0;
  std::vector<int> rids;

  std::string format() const {
    std::ostringstream os;
    if (server < 0) {
      os << "E";
    } else {
      os << "C" << server;
    }
    switch (kind) {
    case Kind::PPre:
      os << " P PRE " << cloud << " " << rid;
      break;
    case Kind::PProc:
      os << " P PROC " << ls << " " << le << " " << cloud << " " << rid;
      break;
    case Kind::PPost:
      os << " P POST " << cloud << " " << rid;
      break;
    case Kind::DPre:
      os << " D PRE -1 " << static_cast<int>(rids.size());
      break;
    case Kind::DProc:
      os << " D PROC " << cloud << " " << static_cast<int>(rids.size());
      break;
    case Kind::DPost:
      os << " D POST -1 " << static_cast<int>(rids.size());
      break;
    }
    for (int id : rids) {
      os << " " << id;
    }
    return os.str();
  }
};

struct FrameEvent {
  enum class Type { Arr, Tdn, Xdn, Fin };
  Type type = Type::Arr;
  std::string raw;
};

struct Config {
  int k = 1;
  double s = 1;
  double latency_ms = 1;
  double bandwidth_gbps = 1;
  int64_t bytes_per_token = 1;
  int num_layers = 1;
  double slo1 = 1;
  double slo2 = 1;
  double tp_ub = 1;
  double tp_base = 0;
  double dist_base = 0;
  double w_tp = 0.5;
  double w_c = 0.5;
  TaskTable table;
};

struct Policy {
  bool batch = true;
  bool wait_to_batch = true;
  bool smart_batch = true;
  int split_mode = 0;

  // Online scheduling knobs (EdgeOPT-style adaptive thresholds + WFQ decode).
  bool online_adapt = true;
  double slo_pressure_gain = 0.8;
  double batch_wait_age_frac = 0.3;
  double arrival_ema_alpha = 0.08;
  int base_admit_every = 4;
  int base_admit_cap_mul = 3;
  double cloud_xfer_weight = 64.0;
  double cloud_load_weight = 64.0;
  double decode_fairness = 0.0;
  int prefill_wave_cap = 5;
  int batch_wait_max = 20;
  double pproc_split_factor = 8.0;
  double edge_dpost_penalty = 0.015;
  double edge_dpre_penalty = 0.01;
  double started_decode_bonus = 0.5;

  bool frame_search = false;
  bool auto_tune = true;
};

// Tracks arrival rate and SLO slack for online decisions (no future lookahead).
class OnlineState {
public:
  void reset(double alpha) {
    alpha_ = alpha;
    last_arr_time_ = -1;
    arrival_ema_ = 0;
  }

  void note_arr(double now) {
    if (last_arr_time_ >= 0 && now > last_arr_time_) {
      const double rate = 1.0 / (now - last_arr_time_);
      arrival_ema_ = (1.0 - alpha_) * arrival_ema_ + alpha_ * rate;
    }
    last_arr_time_ = now;
  }

  double arrival_ema() const {
    return arrival_ema_;
  }

private:
  double alpha_ = 0.08;
  double last_arr_time_ = -1;
  double arrival_ema_ = 0;
};

struct Request {
  int lin = 0;
  int cloud = -1;
  int next_ls = 0;
  Need need = Need::Idle;
  bool finished = false;
  bool in_flight = false;
  bool prefill_done = false;
  bool started_decode = false;
  double arr = 0;
  double last_token = -1;
  int tokens = 0;
};

// Piecewise-linear task times, transfer formula, and derived mode knobs.
class CostModel {
public:
  Config cfg;
  Policy policy;
  Mode mode = Mode::Throughput;
  bool slow_link = false;
  bool tp_heavy = false;
  double unit_xfer = 1;
  double decode_compute = 1;

  void init(Config c, Policy p) {
    cfg = std::move(c);
    policy = p;
    unit_xfer = transfer_ms(cfg.latency_ms, cfg.bandwidth_gbps, cfg.bytes_per_token, 1);
    decode_compute = cfg.s + cfg.table.lookup(Step::DecodePre, 1) +
                     cfg.table.lookup(Step::DecodeProc, 1) + cfg.table.lookup(Step::DecodePost, 1);
    slow_link = unit_xfer > 20.0 && unit_xfer > 0.45 * decode_compute;
    tp_heavy = cfg.w_tp >= 0.7;
    if (cfg.dist_base <= 1e-12 || cfg.w_c >= 0.9) {
      mode = Mode::Slo;
    } else if (cfg.w_c >= 0.65 && cfg.w_c >= cfg.w_tp + 0.1) {
      mode = Mode::Latency;
    } else {
      mode = Mode::Throughput;
    }
    precompute();
  }

  double xfer(int len) const {
    return transfer_ms(cfg.latency_ms, cfg.bandwidth_gbps, cfg.bytes_per_token, std::max(1, len));
  }

  double dur(Step step, int k) const {
    return cfg.table.lookup(step, std::max(1, k));
  }

  double cycle(Step step, int k, bool include_xfer) const {
    return cfg.s + dur(step, k) + (include_xfer ? xfer(k) : 0.0);
  }

  int admit_cap() const {
    if (mode == Mode::Slo || mode == Mode::Latency || !tp_heavy) {
      return std::max(cfg.k * 2, 2);
    }
    if (slow_link) {
      if (cfg.dist_base > 8.0) {
        return std::max(cfg.k * 2, 2);
      }
      return std::max(cfg.k * 2, std::min(cfg.k * policy.base_admit_cap_mul, cfg.k * 3));
    }
    return std::max(cfg.k * policy.base_admit_cap_mul, 8);
  }

  int group_cap(int m, Step step, bool include_xfer) const {
    if (m <= 1) {
      return m;
    }
    if (mode == Mode::Slo || !policy.batch) {
      return 1;
    }
    if (!policy.smart_batch) {
      return m;
    }
    (void)include_xfer;
    const int idx = static_cast<int>(step);
    int cap = m > 4096 ? best_pre_[idx][4096] : best_pre_[idx][m];
    if (mode == Mode::Latency) {
      cap = std::min(cap, 8);
    }
    if (mode == Mode::Throughput && cfg.dist_base > 0 && cfg.dist_base < 5.0 &&
        step != Step::PrefillProc) {
      cap = std::min(cap, 16);
    }
    if (mode == Mode::Throughput && step == Step::DecodePre) {
      const int pipe = m > 4096 ? best_pipe_[4096] : best_pipe_[m];
      cap = std::max(cap, pipe);
      if (!slow_link) {
        cap = std::max(cap, std::min(m, cfg.k));
      }
    }
    return std::max(1, cap);
  }

private:
  int best_pre_[6][4097] = {};
  int best_pipe_[4097] = {};

  double decode_roundtrip(int k, int hops) const {
    hops = std::max(1, std::min(k, hops));
    const int base = k / hops;
    const int rem = k % hops;
    double ups = 0;
    double downs = 0;
    double max_proc = 0;
    for (int i = 0; i < hops; i++) {
      const int sz = base + (i < rem ? 1 : 0);
      ups += xfer(sz);
      downs += xfer(sz);
      max_proc = std::max(max_proc, cfg.s + dur(Step::DecodeProc, sz));
    }
    return cfg.s + dur(Step::DecodePre, k) + ups + max_proc + downs + cfg.s +
           dur(Step::DecodePost, k);
  }

  void precompute() {
    for (int s = 0; s < 6; s++) {
      const bool xfer_on =
          (s == static_cast<int>(Step::DecodePre) || s == static_cast<int>(Step::DecodeProc));
      best_pre_[s][1] = 1;
      double best_c = cycle(static_cast<Step>(s), 1, xfer_on) / 1.0;
      for (int m = 2; m <= 4096; m++) {
        const double c = cycle(static_cast<Step>(s), m, xfer_on) / double(m);
        if (c <= best_c + 1e-12) {
          best_c = c;
          best_pre_[s][m] = m;
        } else {
          best_pre_[s][m] = best_pre_[s][m - 1];
        }
      }
    }
    best_pipe_[1] = 1;
    double best_c = decode_roundtrip(1, 1);
    for (int m = 2; m <= 4096; m++) {
      const double mixed = decode_roundtrip(m, std::min(m, cfg.k)) / double(m);
      const double one = decode_roundtrip(m, 1) / double(m);
      const double c = std::min(mixed, one);
      if (c <= best_c + 1e-12) {
        best_c = c;
        best_pipe_[m] = m;
      } else {
        best_pipe_[m] = best_pipe_[m - 1];
      }
    }
  }
};

// Decode cohort: FIFO prefix at k*, same-cloud filter applied by the caller.
// Past Huawei online contests that look like this are interactive — full-schedule
// SA/MIP does not transfer. Grouping is greedy on the real transfer hops.
class GroupSearch {
public:
  explicit GroupSearch(const CostModel& cost) : cost_(cost) {}

  std::vector<int> select(const std::vector<int>& ids,
                          const std::vector<Request>& reqs,
                          Step step,
                          bool include_xfer) const {
    (void)reqs;
    if (ids.empty()) {
      return ids;
    }
    const int cap = cost_.group_cap(static_cast<int>(ids.size()), step, include_xfer);
    return trim(ids, cap);
  }

  static std::vector<int> trim(std::vector<int> ids, int k) {
    if (k < static_cast<int>(ids.size())) {
      ids.resize(k);
    }
    return ids;
  }

private:
  const CostModel& cost_;
};

class World {
public:
  CostModel cost;
  double now = 0;
  bool edge_busy = false;
  std::vector<char> cloud_busy;
  std::vector<int> cloud_load;
  std::vector<int> cloud_lin;
  std::vector<Request> reqs;
  int pending_prefill_up = 0;
  int pending_prefill_down = 0;
  int pending_decode_up = 0;
  int pending_decode_down = 0;
  int prefill_open = 0;
  int edge_decode_streak = 0;
  std::vector<int> pending_decode_up_cloud;
  std::vector<int> prefill_open_cloud;
  std::vector<int> cloud_decode_streak;
  double up_free_at = 0;
  double down_free_at = 0;
  double tdr_sum = 0;
  int tdr_n = 0;
  double tpot_sum = 0;
  int tpot_n = 0;
  int finished_lout_sum = 0;
  int finished_n = 0;
  std::deque<int> ppre_ready;
  std::deque<int> ppost_ready;
  std::deque<int> dpre_ready;
  std::deque<int> dpost_ready;
  std::vector<std::deque<int>> pproc_ready;
  std::vector<std::deque<int>> dproc_ready;
  OnlineState online;

  void reset(Config cfg, Policy policy) {
    cost.init(std::move(cfg), policy);
    online.reset(policy.arrival_ema_alpha);
    edge_busy = false;
    cloud_busy.assign(cost.cfg.k, false);
    cloud_load.assign(cost.cfg.k, 0);
    cloud_lin.assign(cost.cfg.k, 0);
    pending_decode_up_cloud.assign(cost.cfg.k, 0);
    prefill_open_cloud.assign(cost.cfg.k, 0);
    cloud_decode_streak.assign(cost.cfg.k, 0);
    pending_prefill_up = pending_prefill_down = 0;
    pending_decode_up = pending_decode_down = 0;
    prefill_open = edge_decode_streak = 0;
    now = up_free_at = down_free_at = 0;
    tdr_sum = tpot_sum = 0;
    tdr_n = tpot_n = finished_lout_sum = finished_n = 0;
    reqs.clear();
    ppre_ready.clear();
    ppost_ready.clear();
    dpre_ready.clear();
    dpost_ready.clear();
    pproc_ready.assign(cost.cfg.k, {});
    dproc_ready.assign(cost.cfg.k, {});
    seen_at_.clear();
    seen_stamp_ = 1;
  }

  void apply_raw(const std::string& raw) {
    std::istringstream in(raw);
    std::string tag;
    in >> tag;
    if (tag == "ARR") {
      on_arr(in);
    } else if (tag == "FIN") {
      on_fin(in);
    } else if (tag == "XDN") {
      on_xdn(in);
    } else {
      on_tdn(in);
    }
  }

  std::vector<int> collect(std::deque<int>& q, Need n, int cloud_filter, bool all) {
    if (++seen_stamp_ == 0) {
      std::fill(seen_at_.begin(), seen_at_.end(), 0);
      seen_stamp_ = 1;
    }
    if (seen_at_.size() < reqs.size()) {
      seen_at_.resize(reqs.size(), 0);
    }
    const int scan_cap = all ? 512 : 0;
    std::deque<int> keep;
    std::deque<int> tail;
    int scanned = 0;
    while (!q.empty()) {
      const int rid = q.front();
      q.pop_front();
      if (all && scanned >= scan_cap) {
        tail.push_back(rid);
        continue;
      }
      scanned++;
      if (!valid_ready(rid, n, cloud_filter) || seen_at_[rid] == seen_stamp_) {
        continue;
      }
      seen_at_[rid] = seen_stamp_;
      keep.push_back(rid);
    }
    for (int rid : tail) {
      q.push_back(rid);
    }
    for (int rid : keep) {
      q.push_back(rid);
    }
    if (keep.empty()) {
      return {};
    }
    if (!all) {
      return {keep.front()};
    }
    return std::vector<int>(keep.begin(), keep.end());
  }

  std::vector<int> peek_ready(const std::deque<int>& q, Need n, int cloud_filter, bool all) const {
    std::vector<int> out;
    for (int rid : q) {
      if (!valid_ready(rid, n, cloud_filter)) {
        continue;
      }
      if (std::find(out.begin(), out.end(), rid) != out.end()) {
        continue;
      }
      out.push_back(rid);
    }
    if (!all && !out.empty()) {
      return {out[0]};
    }
    return out;
  }

  int pick_cloud(int forced = -1) const {
    if (forced >= 0 && forced < cost.cfg.k) {
      return forced;
    }
    const auto& p = cost.policy;
    int best = 0;
    auto load = [&](int k) {
      double xfer_pen = 0;
      if (p.online_adapt && cost.mode != Mode::Slo &&
          (pending_decode_up_cloud[k] > 0 || pending_prefill_up > 0 || pending_decode_up > 0)) {
        xfer_pen = up_rem() * p.cloud_xfer_weight / std::max(1, cost.cfg.k);
      }
      return prefill_open_cloud[k] * p.cloud_load_weight + cloud_lin[k] + cloud_load[k] +
             static_cast<int>(xfer_pen);
    };
    for (int k = 1; k < cost.cfg.k; k++) {
      if (load(k) < load(best) || (load(k) == load(best) && !cloud_busy[k] && cloud_busy[best])) {
        best = k;
      }
    }
    return best;
  }

  bool future_work() const {
    if (pending_prefill_up + pending_prefill_down + pending_decode_up + pending_decode_down > 0) {
      return true;
    }
    for (int k = 0; k < cost.cfg.k; k++) {
      if (cloud_busy[k]) {
        return true;
      }
    }
    return false;
  }

  int decode_backlog() const {
    int n = static_cast<int>(dpre_ready.size() + dpost_ready.size());
    for (const auto& q : dproc_ready) {
      n += static_cast<int>(q.size());
    }
    return n;
  }

  void mark_flight(const std::vector<int>& ids) {
    for (int id : ids) {
      reqs[id].in_flight = true;
      reqs[id].need = Need::Idle;
    }
  }

  double up_rem() const {
    if (pending_prefill_up + pending_decode_up <= 0) {
      return 0;
    }
    return std::max(0.0, up_free_at - now);
  }

  double down_rem() const {
    if (pending_prefill_down + pending_decode_down <= 0) {
      return 0;
    }
    return std::max(0.0, down_free_at - now);
  }

private:
  std::vector<int> seen_at_;
  int seen_stamp_ = 1;

  void ensure_rid(int rid, int lin) {
    if (rid >= static_cast<int>(reqs.size())) {
      reqs.resize(rid + 1);
    }
    if (reqs[rid].lin == 0 && lin > 0) {
      reqs[rid].lin = lin;
    }
  }

  void enqueue_need(int rid, Need n) {
    auto& r = reqs[rid];
    r.need = n;
    if (r.finished || r.in_flight || n == Need::Idle) {
      return;
    }
    switch (n) {
    case Need::PPre:
      ppre_ready.push_back(rid);
      break;
    case Need::PPost:
      ppost_ready.push_back(rid);
      break;
    case Need::DPre:
      dpre_ready.push_back(rid);
      break;
    case Need::DPost:
      dpost_ready.push_back(rid);
      break;
    case Need::PProc:
      if (r.cloud >= 0 && r.cloud < cost.cfg.k) {
        pproc_ready[r.cloud].push_back(rid);
      }
      break;
    case Need::DProc:
      if (r.cloud >= 0 && r.cloud < cost.cfg.k) {
        dproc_ready[r.cloud].push_back(rid);
      }
      break;
    default:
      break;
    }
  }

  bool valid_ready(int rid, Need n, int cloud_filter) const {
    if (rid < 0 || rid >= static_cast<int>(reqs.size())) {
      return false;
    }
    const auto& r = reqs[rid];
    if (r.finished || r.in_flight || r.need != n) {
      return false;
    }
    return cloud_filter < 0 || r.cloud == cloud_filter;
  }

  void note_up(int len) {
    up_free_at = std::max(up_free_at, now) + cost.xfer(len);
  }

  void note_down(int len) {
    down_free_at = std::max(down_free_at, now) + cost.xfer(len);
  }

  void on_arr(std::istringstream& in) {
    int rid = 0;
    int lin = 0;
    in >> rid >> lin;
    ensure_rid(rid, lin);
    auto& r = reqs[rid];
    r = Request{};
    r.lin = lin;
    r.arr = now;
    online.note_arr(now);
    enqueue_need(rid, Need::PPre);
  }

  void on_fin(std::istringstream& in) {
    int rid = 0;
    in >> rid;
    ensure_rid(rid, 0);
    auto& r = reqs[rid];
    r.finished = true;
    r.in_flight = false;
    r.need = Need::Idle;
    finished_lout_sum += std::max(1, r.tokens);
    finished_n++;
    if (r.cloud >= 0 && r.cloud < cost.cfg.k) {
      cloud_load[r.cloud] = std::max(0, cloud_load[r.cloud] - 1);
      cloud_lin[r.cloud] = std::max(0, cloud_lin[r.cloud] - r.lin);
    }
  }

  void on_xdn(std::istringstream& in) {
    std::string dir, kind;
    int cloud = 0;
    int64_t size = 0;
    int m = 0;
    in >> dir >> cloud >> size >> kind >> m;
    std::vector<int> rids(m);
    for (int i = 0; i < m; i++) {
      in >> rids[i];
      ensure_rid(rids[i], 0);
    }
    const bool up = dir == "UP";
    const bool pre = kind == "PRE";
    if (pre) {
      (up ? pending_prefill_up : pending_prefill_down) =
          std::max(0, (up ? pending_prefill_up : pending_prefill_down) - 1);
    } else if (up) {
      pending_decode_up = std::max(0, pending_decode_up - 1);
      if (cloud >= 0 && cloud < cost.cfg.k) {
        pending_decode_up_cloud[cloud] = std::max(0, pending_decode_up_cloud[cloud] - 1);
      }
    } else {
      pending_decode_down = std::max(0, pending_decode_down - 1);
    }
    for (int rid : rids) {
      reqs[rid].in_flight = false;
      enqueue_need(rid, pre ? (up ? Need::PProc : Need::PPost) : (up ? Need::DProc : Need::DPost));
    }
  }

  void on_tdn(std::istringstream& in) {
    std::string server, phase, step;
    in >> server >> phase >> step;
    if (server == "E") {
      edge_busy = false;
    } else if (!server.empty() && server[0] == 'C') {
      const int ck = std::stoi(server.substr(1));
      if (ck >= 0 && ck < cost.cfg.k) {
        cloud_busy[ck] = false;
      }
    }
    if (phase == "P" && step == "PRE") {
      int cloud = 0;
      int rid = 0;
      in >> cloud >> rid;
      ensure_rid(rid, 0);
      reqs[rid].in_flight = false;
      reqs[rid].need = Need::Idle;
      pending_prefill_up++;
      note_up(std::max(1, reqs[rid].lin));
      return;
    }
    if (phase == "P" && step == "PROC") {
      int ls = 0;
      int le = 0;
      int cloud = 0;
      int rid = 0;
      in >> ls >> le >> cloud >> rid;
      ensure_rid(rid, 0);
      reqs[rid].in_flight = false;
      reqs[rid].next_ls = le;
      if (le < cost.cfg.num_layers) {
        enqueue_need(rid, Need::PProc);
      } else {
        reqs[rid].need = Need::Idle;
        pending_prefill_down++;
        note_down(std::max(1, reqs[rid].lin));
      }
      return;
    }
    if (phase == "P" && step == "POST") {
      int cloud = 0;
      int rid = 0;
      in >> cloud >> rid;
      ensure_rid(rid, 0);
      auto& r = reqs[rid];
      r.in_flight = false;
      if (!r.prefill_done) {
        r.prefill_done = true;
        prefill_open = std::max(0, prefill_open - 1);
        if (r.cloud >= 0 && r.cloud < cost.cfg.k) {
          prefill_open_cloud[r.cloud] = std::max(0, prefill_open_cloud[r.cloud] - 1);
        }
        tdr_sum += now - r.arr;
        tdr_n++;
      }
      enqueue_need(rid, Need::DPre);
      return;
    }
    if (phase != "D") {
      return;
    }
    int marker = 0;
    int m = 0;
    in >> marker >> m;
    std::vector<int> rids(m);
    for (int i = 0; i < m; i++) {
      in >> rids[i];
      ensure_rid(rids[i], 0);
      reqs[rids[i]].in_flight = false;
    }
    if (step == "PRE") {
      int cnt[8] = {};
      for (int rid : rids) {
        const int c = reqs[rid].cloud;
        if (c >= 0 && c < cost.cfg.k) {
          cnt[c]++;
        }
        reqs[rid].need = Need::Idle;
      }
      int nclouds = 0;
      for (int c = 0; c < cost.cfg.k; c++) {
        if (cnt[c] > 0) {
          nclouds++;
          pending_decode_up_cloud[c]++;
          note_up(cnt[c]);
        }
      }
      pending_decode_up += nclouds;
    } else if (step == "PROC") {
      pending_decode_down++;
      note_down(std::max(1, m));
      for (int rid : rids) {
        reqs[rid].need = Need::Idle;
      }
    } else {
      for (int rid : rids) {
        auto& r = reqs[rid];
        if (r.last_token >= 0) {
          tpot_sum += now - r.last_token;
          tpot_n++;
        }
        r.last_token = now;
        r.tokens++;
        enqueue_need(rid, r.finished ? Need::Idle : Need::DPre);
      }
    }
  }
};

inline double online_slo_pressure_tdr(const World& w) {
  if (w.tdr_n <= 0) {
    return 0;
  }
  return std::max(0.0, (w.tdr_sum / double(w.tdr_n)) / w.cost.cfg.slo1 - 0.75);
}

inline double online_slo_pressure_tpot(const World& w) {
  if (w.tpot_n <= 0) {
    return 0;
  }
  return std::max(0.0, (w.tpot_sum / double(w.tpot_n)) / w.cost.cfg.slo2 - 0.75);
}

inline double online_slo_pressure(const World& w) {
  return online_slo_pressure_tdr(w) + online_slo_pressure_tpot(w);
}

inline int online_backlog(const World& w) {
  int n = static_cast<int>(w.ppre_ready.size() + w.dpre_ready.size() + w.dpost_ready.size() +
                           w.ppost_ready.size() + w.prefill_open);
  for (const auto& q : w.pproc_ready) {
    n += static_cast<int>(q.size());
  }
  for (const auto& q : w.dproc_ready) {
    n += static_cast<int>(q.size());
  }
  return n;
}

inline Policy policy_variant(int v) {
  Policy p;
  p.online_adapt = true;
  p.frame_search = false;
  p.auto_tune = false;
  const int slot = v % 12;
  p.slo_pressure_gain = 0.6 + 0.2 * (slot % 4);
  p.batch_wait_age_frac = 0.25 + 0.05 * (slot / 4);
  p.base_admit_every = 2 + (v % 5);
  p.base_admit_cap_mul = 3 + (v % 4);
  p.cloud_xfer_weight = 32.0 + 32.0 * ((v / 12) % 4);
  p.decode_fairness = ((v / 48) % 3) * 0.35;
  p.prefill_wave_cap = 5 + (v % 6);
  p.batch_wait_max = 16 + 8 * ((v / 6) % 4);
  p.pproc_split_factor = 4.0 + 2.0 * (v % 5);
  p.split_mode = 0;
  p.wait_to_batch = (v % 7) != 0;
  p.smart_batch = (v % 5) != 0;
  return p;
}

inline double analytic_policy_score(const Config& cfg, const Policy& p) {
  CostModel m;
  m.init(cfg, p);
  const double wt = cfg.w_tp;
  const double wc = cfg.w_c;
  const int batch = m.group_cap(32, Step::DecodePre, true);
  const double tp_cycle = m.cycle(Step::DecodePre, batch, true) +
                          m.cycle(Step::DecodeProc, batch, true) +
                          m.cycle(Step::DecodePost, batch, false);
  const double tp_score = double(batch) / std::max(1e-6, tp_cycle);
  const double lat_cycle =
      m.cycle(Step::PrefillPre, 1, false) + m.cycle(Step::PrefillProc, 8, false) +
      m.cycle(Step::PrefillPost, 1, false) + m.cycle(Step::DecodePre, 1, true) +
      m.cycle(Step::DecodeProc, 1, true) + m.cycle(Step::DecodePost, 1, false);
  const double lat_score = 1.0 / std::max(1e-6, lat_cycle);
  double s = wt * tp_score + wc * lat_score;
  if (m.slow_link && p.wait_to_batch && wt > 0.55) {
    s += wt * 0.05;
  }
  if (m.slow_link && !p.wait_to_batch && wc > 0.55) {
    s += wc * 0.05;
  }
  if (m.mode == Mode::Slo && wc > 0.85) {
    s += wc * 0.1;
  }
  if (m.mode == Mode::Throughput && wt > 0.65) {
    s += wt * 0.1;
  }
  return s;
}

inline Policy tune_policy(const Config& cfg) {
  Policy best;
  best.frame_search = false;
  best.auto_tune = false;
  best.split_mode = 0;
  if (cfg.w_c >= 0.6 && cfg.w_c >= cfg.w_tp + 0.1) {
    best.decode_fairness = 0.35;
  }
  if (cfg.w_tp >= 0.7 && cfg.dist_base > 5) {
    best.base_admit_cap_mul = 3;
    best.base_admit_every = 5;
    best.prefill_wave_cap = 4;
    best.slo_pressure_gain = 1.2;
  }
  if (cfg.dist_base > 0 && cfg.dist_base < 1.5) {
    best.batch_wait_age_frac = 0.25;
    best.slo_pressure_gain = 1.0;
  }
  double best_score = analytic_policy_score(cfg, best);
  for (int v = 0; v < 96; v++) {
    Policy p = policy_variant(v);
    p.split_mode = 0;
    p.frame_search = false;
    const double s = analytic_policy_score(cfg, p);
    if (s > best_score) {
      best_score = s;
      best = p;
    }
  }
  best.frame_search = false;
  best.split_mode = 0;
  return best;
}

inline std::vector<int> batch_caps(int n, Step step, bool xfer, const CostModel& cost) {
  if (n <= 0) {
    return {};
  }
  std::vector<int> caps;
  auto add = [&](int c) {
    c = std::max(1, std::min(n, c));
    if (std::find(caps.begin(), caps.end(), c) == caps.end()) {
      caps.push_back(c);
    }
  };
  add(1);
  const int g = cost.group_cap(n, step, xfer);
  add(g);
  add(n);
  if (n <= 96) {
    add(g / 2);
    add(g * 3 / 4);
    add(g * 5 / 4);
    for (int t = 2; t <= n; t *= 2) {
      add(t);
    }
  } else {
    for (int t = 2; t <= g; t *= 2) {
      add(t);
    }
  }
  std::sort(caps.begin(), caps.end());
  caps.erase(std::unique(caps.begin(), caps.end()), caps.end());
  return caps;
}

inline double score_edge_action(const World& w, const Assignment& a) {
  const double wt = w.cost.cfg.w_tp;
  const double wc = w.cost.cfg.w_c;
  switch (a.kind) {
  case Assignment::Kind::PPost: {
    const auto& r = w.reqs[a.rid];
    return wc * (1e6 + (w.now - r.arr) / std::max(1.0, w.cost.cfg.slo1));
  }
  case Assignment::Kind::DPost: {
    const int k = static_cast<int>(a.rids.size());
    double s = wt * double(k) / w.cost.cycle(Step::DecodePost, k, false);
    if (w.cost.mode == Mode::Throughput && k > 1) {
      s += wt * 0.08 * std::log(double(k));
    }
    const int cap = std::min(static_cast<int>(a.rids.size()), 32);
    for (int i = 0; i < cap; i++) {
      const int id = a.rids[i];
      const auto& r = w.reqs[id];
      const double gap = r.last_token >= 0 ? w.now - r.last_token : w.now - r.arr;
      s += wc * gap / std::max(1.0, w.cost.cfg.slo2);
    }
    return s;
  }
  case Assignment::Kind::DPre: {
    const int k = static_cast<int>(a.rids.size());
    double s = wt * double(k) / w.cost.cycle(Step::DecodePre, k, true);
    if (w.cost.mode == Mode::Throughput && k > 1) {
      s += wt * 0.08 * std::log(double(k));
    }
    for (int id : a.rids) {
      const auto& r = w.reqs[id];
      if (r.started_decode) {
        s += wc * w.cost.policy.started_decode_bonus;
      }
    }
    if (w.cost.slow_link && k >= 2) {
      s += wt * 0.1 * double(k);
    }
    return s;
  }
  case Assignment::Kind::PPre: {
    const auto& r = w.reqs[a.rid];
    double s = wc * (w.now - r.arr) / std::max(1.0, w.cost.cfg.slo1);
    const int ck = a.cloud;
    if (ck >= 0 && ck < w.cost.cfg.k) {
      s -= wt * 0.001 *
           (w.prefill_open_cloud[ck] * w.cost.policy.cloud_load_weight + w.cloud_lin[ck] +
            w.cloud_load[ck]);
    }
    if (w.prefill_open >= w.cost.admit_cap()) {
      s -= wc * 10.0;
    }
    if (w.cost.tp_heavy && w.cost.slow_link && w.prefill_open > 0) {
      s += wt * 0.35;
    }
    return s;
  }
  default:
    return -1e100;
  }
}

inline double score_cloud_action(const World& w, int k, const Assignment& a) {
  const double wt = w.cost.cfg.w_tp;
  const double wc = w.cost.cfg.w_c;
  if (a.kind == Assignment::Kind::DProc) {
    const int n = static_cast<int>(a.rids.size());
    double s = wt * double(n) / w.cost.cycle(Step::DecodeProc, n, true);
    if (w.cost.mode == Mode::Throughput && n > 1) {
      s += wt * 0.08 * std::log(double(n));
    }
    if (w.cost.slow_link && w.prefill_open_cloud[k] > 0) {
      s -= wc * 0.2;
    }
    return s;
  }
  if (a.kind == Assignment::Kind::PProc) {
    const auto& r = w.reqs[a.rid];
    double s = wc * (w.now - r.arr) / std::max(1.0, w.cost.cfg.slo1);
    const double full = w.cost.dur(Step::PrefillProc, std::max(1, r.lin));
    const double piece = full * double(a.le - a.ls) / double(w.cost.cfg.num_layers);
    s += wt / std::max(1e-6, piece + w.cost.cfg.s);
    if (w.cloud_decode_streak[k] >= 2) {
      s += wc * 0.3;
    }
    return s;
  }
  return -1e100;
}

class PolicyEngine {
public:
  explicit PolicyEngine(World& world) : w_(world), groups_(world.cost) {}

  std::vector<Assignment> propose() {
    if (w_.cost.policy.frame_search && w_.decode_backlog() <= 64 && w_.cost.cfg.k <= 8) {
      return propose_search();
    }
    return propose_greedy();
  }

private:
  World& w_;
  GroupSearch groups_;

  std::vector<Assignment> propose_greedy() {
    std::vector<Assignment> out;
    if (!w_.edge_busy) {
      if (Assignment a; propose_edge(a)) {
        out.push_back(std::move(a));
      }
    }
    for (int k = 0; k < w_.cost.cfg.k; k++) {
      if (w_.cloud_busy[k]) {
        continue;
      }
      if (Assignment a; propose_cloud(k, a)) {
        out.push_back(std::move(a));
      }
    }
    return out;
  }

  std::vector<Assignment> propose_search() {
    std::vector<Assignment> out;
    if (!w_.edge_busy) {
      if (Assignment a; propose_edge(a)) {
        out.push_back(std::move(a));
      }
    }
    for (int k = 0; k < w_.cost.cfg.k; k++) {
      if (w_.cloud_busy[k]) {
        continue;
      }
      const auto cands = cloud_candidates(k);
      if (!cands.empty()) {
        Assignment best = cands[0];
        double best_s = score_cloud_action(w_, k, best);
        for (size_t i = 1; i < cands.size(); i++) {
          const double s = score_cloud_action(w_, k, cands[i]);
          if (s > best_s) {
            best_s = s;
            best = cands[i];
          }
        }
        commit_cloud(k, best);
        out.push_back(best);
      } else if (Assignment a; propose_cloud(k, a)) {
        out.push_back(std::move(a));
      }
    }
    return out;
  }

  void commit_cloud(int k, const Assignment& a) {
    if (a.kind == Assignment::Kind::PProc) {
      w_.mark_flight({a.rid});
      w_.cloud_busy[k] = true;
      w_.cloud_decode_streak[k] = 0;
      return;
    }
    if (a.kind == Assignment::Kind::DProc) {
      w_.mark_flight(a.rids);
      w_.cloud_busy[k] = true;
      w_.cloud_decode_streak[k]++;
    }
  }

  std::vector<int> ranked_clouds() const {
    std::vector<int> order(w_.cost.cfg.k);
    for (int i = 0; i < w_.cost.cfg.k; i++) {
      order[i] = i;
    }
    const auto load = [&](int ck) {
      return w_.prefill_open_cloud[ck] * w_.cost.policy.cloud_load_weight + w_.cloud_lin[ck] +
             w_.cloud_load[ck];
    };
    std::sort(order.begin(), order.end(), [&](int a, int b) {
      if (load(a) != load(b)) {
        return load(a) < load(b);
      }
      return !w_.cloud_busy[a] && w_.cloud_busy[b];
    });
    return order;
  }

  std::vector<Assignment> cloud_candidates(int k) {
    std::vector<Assignment> cands;
    const std::vector<int> pproc = w_.peek_ready(w_.pproc_ready[k], Need::PProc, k, false);
    const std::vector<int> dproc_all =
        w_.peek_ready(w_.dproc_ready[k], Need::DProc, k, w_.cost.policy.batch);

    std::vector<int> dproc_ref = dproc_all;
    if (!dproc_ref.empty()) {
      dproc_ref = decode_group(std::move(dproc_ref), Step::DecodeProc, true);
    }

    if (!pproc.empty()) {
      const int rid = pproc[0];
      const int ls = w_.reqs[rid].next_ls;
      std::vector<int> layers;
      auto add_le = [&](int le) {
        le = std::min(le, w_.cost.cfg.num_layers);
        if (le <= ls) {
          return;
        }
        if (std::find(layers.begin(), layers.end(), le) == layers.end()) {
          layers.push_back(le);
        }
      };
      if (w_.cost.tp_heavy) {
        add_le(pproc_le(rid, !dproc_ref.empty()));
        add_le(w_.cost.cfg.num_layers);
        add_le(ls + 1);
        if (w_.cost.cfg.num_layers - ls > 2) {
          add_le(ls + (w_.cost.cfg.num_layers - ls + 1) / 2);
        }
      } else {
        add_le(w_.cost.cfg.num_layers);
      }
      for (int le : layers) {
        Assignment a;
        a.kind = Assignment::Kind::PProc;
        a.server = k;
        a.cloud = k;
        a.rid = rid;
        a.ls = ls;
        a.le = le;
        cands.push_back(a);
      }
    }
    if (!dproc_ref.empty()) {
      Assignment full;
      full.kind = Assignment::Kind::DProc;
      full.server = k;
      full.cloud = k;
      full.rids = dproc_ref;
      cands.push_back(full);
      if (w_.cost.tp_heavy && w_.cost.slow_link && static_cast<int>(dproc_all.size()) <= 96) {
        for (int cap :
             batch_caps(static_cast<int>(dproc_all.size()), Step::DecodeProc, true, w_.cost)) {
          if (cap >= static_cast<int>(dproc_ref.size())) {
            continue;
          }
          auto prefix = GroupSearch::trim(dproc_all, cap);
          prefix = decode_group(std::move(prefix), Step::DecodeProc, true);
          if (prefix.empty()) {
            continue;
          }
          Assignment a;
          a.kind = Assignment::Kind::DProc;
          a.server = k;
          a.cloud = k;
          a.rids = std::move(prefix);
          cands.push_back(a);
        }
      }
    }
    return cands;
  }

  bool should_wait(int have,
                   int pending,
                   int target,
                   const std::vector<int>& ids,
                   bool use_token_age) const {
    if (!w_.cost.policy.wait_to_batch || !w_.cost.tp_heavy || w_.cost.mode == Mode::Slo) {
      return false;
    }
    if (have <= 0 || have >= target || pending <= 0 || !w_.future_work()) {
      return false;
    }
    if (!w_.cost.slow_link || have >= std::min(target, w_.cost.policy.batch_wait_max)) {
      return false;
    }
    if (w_.cost.policy.online_adapt && w_.cost.mode != Mode::Slo && !ids.empty()) {
      double oldest = 0;
      for (int id : ids) {
        const auto& r = w_.reqs[id];
        const double age = use_token_age
                               ? (r.last_token >= 0 ? w_.now - r.last_token : w_.now - r.arr)
                               : (r.prefill_done ? w_.now - r.arr : w_.now - r.arr);
        oldest = std::max(oldest, age);
      }
      const double pressure = online_slo_pressure(w_);
      const double max_wait =
          w_.cost.policy.batch_wait_age_frac * w_.cost.cfg.slo2 * (1.0 + pressure);
      if (oldest >= max_wait) {
        return false;
      }
    }
    return true;
  }

  int effective_admit_cap() const {
    int cap = w_.cost.admit_cap();
    if (w_.cost.tp_heavy && w_.cost.cfg.w_c >= 0.18 && w_.cost.cfg.dist_base > 8.0 &&
        online_slo_pressure_tdr(w_) > 0) {
      const int cut = static_cast<int>(online_slo_pressure_tdr(w_) * w_.cost.cfg.k * 4);
      cap = std::max(w_.cost.cfg.k, cap - cut);
    }
    if (w_.cost.policy.online_adapt && w_.cost.mode == Mode::Throughput && w_.cost.tp_heavy) {
      cap += std::min(w_.cost.cfg.k * 2,
                      static_cast<int>(w_.online.arrival_ema() * w_.cost.cfg.k * 40));
      const int bk = online_backlog(w_);
      if (bk > cap) {
        cap = std::min(cap + w_.cost.cfg.k, bk);
      }
    }
    return cap;
  }

  int effective_admit_every() const {
    if (w_.cost.tp_heavy && w_.cost.slow_link) {
      return 1000000;
    }
    if (!w_.cost.tp_heavy) {
      return 1;
    }
    int every = w_.cost.policy.base_admit_every;
    if (w_.cost.policy.online_adapt && w_.cost.mode == Mode::Throughput) {
      const double pressure = online_slo_pressure(w_);
      every = std::max(
          1, static_cast<int>(every / (1.0 + w_.cost.policy.slo_pressure_gain * pressure)));
    }
    return every;
  }

  int pick_ready(const std::vector<int>& ids, bool shortest) const {
    int best = ids[0];
    for (int i = 1; i < static_cast<int>(ids.size()); i++) {
      const int id = ids[i];
      const auto& a = w_.reqs[id];
      const auto& b = w_.reqs[best];
      if (shortest) {
        if (a.lin < b.lin || (a.lin == b.lin && a.arr < b.arr)) {
          best = id;
        }
      } else if (a.arr < b.arr) {
        best = id;
      }
    }
    return best;
  }

  std::vector<int> decode_group(std::vector<int> ids, Step step, bool xfer) const {
    if (ids.size() > 1 && w_.cost.slow_link && w_.cost.cfg.k > 1 && step == Step::DecodePre) {
      std::vector<std::vector<int>> by(w_.cost.cfg.k);
      for (int id : ids) {
        const int c = w_.reqs[id].cloud;
        if (c >= 0 && c < w_.cost.cfg.k) {
          by[c].push_back(id);
        }
      }
      int best = 0;
      for (int c = 1; c < w_.cost.cfg.k; c++) {
        if (by[c].size() > by[best].size()) {
          best = c;
        }
      }
      if (by[best].size() >= 2) {
        ids = by[best];
      }
    }
    if (w_.cost.mode == Mode::Latency) {
      if (w_.cost.policy.online_adapt && w_.cost.policy.decode_fairness > 0) {
        std::stable_sort(ids.begin(), ids.end(), [&](int a, int b) {
          const int ta = w_.reqs[a].tokens;
          const int tb = w_.reqs[b].tokens;
          if (ta != tb) {
            return ta < tb;
          }
          return w_.reqs[a].arr < w_.reqs[b].arr;
        });
      } else {
        std::stable_partition(
            ids.begin(), ids.end(), [&](int id) { return w_.reqs[id].started_decode; });
      }
    } else if (w_.cost.mode == Mode::Slo) {
      std::stable_partition(
          ids.begin(), ids.end(), [&](int id) { return w_.reqs[id].started_decode; });
    } else if (w_.cost.mode == Mode::Throughput && w_.cost.cfg.dist_base > 0 &&
               w_.cost.cfg.dist_base < 8.0) {
      std::stable_partition(
          ids.begin(), ids.end(), [&](int id) { return w_.reqs[id].started_decode; });
    }
    return groups_.select(ids, w_.reqs, step, xfer);
  }

  int pproc_le(int rid, bool compete) const {
    const int ls = w_.reqs[rid].next_ls;
    if (ls + 1 >= w_.cost.cfg.num_layers) {
      return w_.cost.cfg.num_layers;
    }
    if (w_.cost.policy.split_mode == 2 || (w_.cost.policy.split_mode == 1 && compete)) {
      return ls + 1;
    }
    if (!compete || !w_.cost.tp_heavy) {
      return w_.cost.cfg.num_layers;
    }
    const double full = w_.cost.dur(Step::PrefillProc, std::max(1, w_.reqs[rid].lin));
    const double rem = full * double(w_.cost.cfg.num_layers - ls) / double(w_.cost.cfg.num_layers);
    const double d1 = w_.cost.dur(Step::DecodeProc, 1);
    if (rem <= w_.cost.policy.pproc_split_factor * (w_.cost.cfg.s + d1)) {
      return w_.cost.cfg.num_layers;
    }
    const int remain = w_.cost.cfg.num_layers - ls;
    return ls + std::max(1, (remain + 1) / 2);
  }

  double edge_utility(Assignment::Kind kind, const std::vector<int>& ids, int ppre_rid) const {
    const double wt = w_.cost.cfg.w_tp;
    double wc = w_.cost.cfg.w_c;
    if (w_.cost.policy.online_adapt && w_.cost.mode != Mode::Slo) {
      wc *= 1.0 + w_.cost.policy.slo_pressure_gain * online_slo_pressure(w_);
    }
    const double dpost_pen = w_.cost.policy.edge_dpost_penalty;
    const double dpre_pen = w_.cost.policy.edge_dpre_penalty;
    const double decode_bonus = w_.cost.policy.started_decode_bonus;
    if (kind == Assignment::Kind::PPost) {
      return 1e12;
    }
    if (kind == Assignment::Kind::DPost) {
      const int k = std::max(1, static_cast<int>(ids.size()));
      return wt * double(k) / w_.cost.cycle(Step::DecodePost, k, false) -
             wc * dpost_pen * double(k);
    }
    if (kind == Assignment::Kind::DPre) {
      const int k = std::max(1, static_cast<int>(ids.size()));
      double bonus = 0;
      for (int id : ids) {
        if (w_.reqs[id].started_decode) {
          bonus += wc * decode_bonus;
        }
      }
      return wt * double(k) / w_.cost.cycle(Step::DecodePre, k, true) - wc * dpre_pen * double(k) +
             bonus;
    }
    const double age = w_.now - w_.reqs[ppre_rid].arr;
    return wc * (age / std::max(1.0, w_.cost.cfg.slo1)) + 0.05;
  }

  std::vector<int> best_decode_group(const std::vector<int>& all,
                                     Step step,
                                     bool xfer,
                                     Assignment::Kind kind) const {
    if (all.empty()) {
      return {};
    }
    std::vector<int> ref = decode_group(all, step, xfer);
    std::vector<int> best = ref;
    Assignment probe;
    probe.kind = kind;
    probe.rids = best;
    double best_s = score_edge_action(w_, probe);
    if (w_.cost.policy.frame_search && all.size() > 1 && all.size() <= 96) {
      const bool alt = w_.cost.tp_heavy && w_.cost.slow_link;
      if (alt) {
        for (int cap : batch_caps(static_cast<int>(all.size()), step, xfer, w_.cost)) {
          if (cap >= static_cast<int>(ref.size())) {
            continue;
          }
          auto prefix = GroupSearch::trim(all, cap);
          prefix = decode_group(std::move(prefix), step, xfer);
          if (prefix.empty()) {
            continue;
          }
          probe.rids = prefix;
          const double s = score_edge_action(w_, probe);
          if (s > best_s) {
            best_s = s;
            best = std::move(prefix);
          }
        }
      }
    }
    return best;
  }

  int best_ppre_cloud(int rid) const {
    if (!w_.cost.policy.frame_search || w_.cost.cfg.k <= 1) {
      return w_.pick_cloud();
    }
    int best = w_.pick_cloud();
    double best_s = -1e100;
    const auto clouds = ranked_clouds();
    const int n = std::min(static_cast<int>(clouds.size()), 4);
    for (int i = 0; i < n; i++) {
      Assignment probe;
      probe.kind = Assignment::Kind::PPre;
      probe.rid = rid;
      probe.cloud = clouds[i];
      const double s = score_edge_action(w_, probe);
      if (s > best_s) {
        best_s = s;
        best = clouds[i];
      }
    }
    return best;
  }

  void emit_ppre(Assignment& a, int rid) {
    a.kind = Assignment::Kind::PPre;
    a.server = -1;
    a.rid = rid;
    a.cloud = best_ppre_cloud(rid);
    w_.reqs[rid].cloud = a.cloud;
    w_.cloud_load[a.cloud]++;
    w_.cloud_lin[a.cloud] += w_.reqs[rid].lin;
    w_.prefill_open++;
    w_.prefill_open_cloud[a.cloud]++;
    w_.mark_flight({rid});
    w_.edge_busy = true;
    w_.edge_decode_streak = 0;
  }

  void emit_dpre(Assignment& a, const std::vector<int>& ids) {
    a.kind = Assignment::Kind::DPre;
    a.server = -1;
    a.rids = best_decode_group(ids, Step::DecodePre, true, Assignment::Kind::DPre);
    for (int id : a.rids) {
      w_.reqs[id].started_decode = true;
    }
    w_.mark_flight(a.rids);
    w_.edge_busy = true;
    w_.edge_decode_streak++;
  }

  void emit_dpost(Assignment& a, const std::vector<int>& ids) {
    a.kind = Assignment::Kind::DPost;
    a.server = -1;
    a.rids = best_decode_group(ids, Step::DecodePost, false, Assignment::Kind::DPost);
    w_.mark_flight(a.rids);
    w_.edge_busy = true;
    w_.edge_decode_streak++;
  }

  bool propose_edge(Assignment& a) {
    const bool wait_mode = w_.cost.mode != Mode::Throughput;
    const std::vector<int> post = w_.collect(w_.ppost_ready, Need::PPost, -1, wait_mode);
    if (!post.empty()) {
      const int rid = wait_mode ? pick_ready(post, false) : post[0];
      a.kind = Assignment::Kind::PPost;
      a.server = -1;
      a.rid = rid;
      a.cloud = w_.reqs[rid].cloud;
      w_.mark_flight({rid});
      w_.edge_busy = true;
      w_.edge_decode_streak = 0;
      return true;
    }
    const bool batch = w_.cost.policy.batch;
    std::vector<int> dpost = w_.collect(w_.dpost_ready, Need::DPost, -1, batch);
    std::vector<int> ppre = w_.collect(w_.ppre_ready, Need::PPre, -1, wait_mode);
    std::vector<int> dpre = w_.collect(w_.dpre_ready, Need::DPre, -1, batch);
    dpost = decode_group(std::move(dpost), Step::DecodePost, false);
    dpre = decode_group(std::move(dpre), Step::DecodePre, true);
    if (w_.cost.mode == Mode::Latency && online_slo_pressure_tpot(w_) > 0.08 && !dpost.empty()) {
      emit_dpost(a, dpost);
      return true;
    }
    const int dpost_target = std::max(
        static_cast<int>(dpost.size()),
        w_.cost.group_cap(std::max(2, static_cast<int>(dpost.size()) + w_.pending_decode_down),
                          Step::DecodePost,
                          false));
    const bool wait_dpost = should_wait(
        static_cast<int>(dpost.size()), w_.pending_decode_down, dpost_target, dpost, true);
    const bool care_slo = w_.cost.cfg.w_c >= 0.18 && w_.cost.cfg.dist_base > 8.0;
    const bool prefill_wave = w_.cost.tp_heavy && w_.cost.slow_link && w_.prefill_open > 0 &&
                              (!care_slo || online_slo_pressure_tdr(w_) < 0.15);
    const bool wait_dpre = prefill_wave &&
                           static_cast<int>(dpre.size()) < w_.cost.policy.prefill_wave_cap &&
                           w_.future_work();
    const bool start_dpost = !dpost.empty() && !wait_dpost;
    const bool start_dpre = !dpre.empty() && !wait_dpre;
    const bool start_ppre = !ppre.empty();
    const bool pick_short_ppre = wait_mode || (w_.cost.tp_heavy && w_.cost.slow_link);
    const int ppre_rid = start_ppre ? (pick_short_ppre ? pick_ready(ppre, true) : ppre[0]) : -1;
    const bool under_cap = w_.prefill_open < effective_admit_cap();
    const int admit_every = effective_admit_every();
    const bool admit = start_ppre && (under_cap || w_.edge_decode_streak >= admit_every ||
                                      (!start_dpre && !start_dpost));

    if (w_.cost.tp_heavy) {
      if (care_slo && online_slo_pressure_tdr(w_) > 0.12 && start_dpost) {
        emit_dpost(a, dpost);
        return true;
      }
      if (prefill_wave && admit) {
        emit_ppre(a, ppre_rid);
        return true;
      }
      if (start_dpost && !(admit && w_.edge_decode_streak >= admit_every && dpost.size() < 2)) {
        emit_dpost(a, dpost);
        return true;
      }
      if (admit && (w_.edge_decode_streak >= admit_every || !start_dpre)) {
        emit_ppre(a, ppre_rid);
        return true;
      }
    } else if (start_dpost && start_ppre && start_dpre &&
               (w_.cost.mode != Mode::Throughput ||
                std::abs(w_.cost.cfg.w_tp - w_.cost.cfg.w_c) < 0.12)) {
      const double u_post = edge_utility(Assignment::Kind::DPost, dpost, -1);
      const double u_pre = edge_utility(Assignment::Kind::DPre, dpre, -1);
      const double u_ppre = admit ? edge_utility(Assignment::Kind::PPre, {}, ppre_rid) : -1e100;
      if (u_post >= u_pre && u_post >= u_ppre) {
        emit_dpost(a, dpost);
        return true;
      }
      if (u_ppre >= u_pre) {
        emit_ppre(a, ppre_rid);
        return true;
      }
      emit_dpre(a, dpre);
      return true;
    }

    if (start_dpost) {
      emit_dpost(a, dpost);
      return true;
    }
    if (!w_.cost.tp_heavy && start_dpre && w_.reqs[dpre[0]].started_decode) {
      emit_dpre(a, dpre);
      return true;
    }
    if (admit) {
      emit_ppre(a, ppre_rid);
      return true;
    }
    if (start_dpre) {
      emit_dpre(a, dpre);
      return true;
    }
    if (start_ppre) {
      emit_ppre(a, ppre_rid);
      return true;
    }
    return false;
  }

  bool propose_cloud(int k, Assignment& a) {
    const std::vector<int> pproc = w_.collect(w_.pproc_ready[k], Need::PProc, k, false);
    std::vector<int> dproc = w_.collect(w_.dproc_ready[k], Need::DProc, k, w_.cost.policy.batch);
    dproc = decode_group(std::move(dproc), Step::DecodeProc, true);
    const bool start_d = !dproc.empty();
    const bool start_p = !pproc.empty();
    const bool prefer_p = start_p &&
                          (!start_d || !w_.cost.tp_heavy || w_.cloud_decode_streak[k] >= 2 ||
                           (w_.cost.slow_link && w_.prefill_open_cloud[k] > 0)) &&
                          (w_.cost.cfg.w_c < 0.18 || w_.cost.cfg.dist_base <= 8.0 ||
                           online_slo_pressure_tpot(w_) < 0.15);
    a.server = k;
    a.cloud = k;
    if (prefer_p) {
      const int rid = pproc[0];
      a.kind = Assignment::Kind::PProc;
      a.rid = rid;
      a.ls = w_.reqs[rid].next_ls;
      a.le = pproc_le(rid, start_d);
      w_.mark_flight({rid});
      w_.cloud_busy[k] = true;
      w_.cloud_decode_streak[k] = 0;
      return true;
    }
    if (start_d) {
      a.kind = Assignment::Kind::DProc;
      a.rids = dproc;
      w_.mark_flight(dproc);
      w_.cloud_busy[k] = true;
      w_.cloud_decode_streak[k]++;
      return true;
    }
    if (start_p) {
      const int rid = pproc[0];
      a.kind = Assignment::Kind::PProc;
      a.rid = rid;
      a.ls = w_.reqs[rid].next_ls;
      a.le = pproc_le(rid, false);
      w_.mark_flight({rid});
      w_.cloud_busy[k] = true;
      w_.cloud_decode_streak[k] = 0;
      return true;
    }
    return false;
  }
};

class Scheduler {
public:
  void set_policy(Policy p) {
    policy_ = p;
  }

  const Policy& policy() const {
    return policy_;
  }

  void init(Config cfg) {
    if (policy_.auto_tune) {
      policy_ = tune_policy(cfg);
      policy_.auto_tune = false;
    }
    world_.reset(std::move(cfg), policy_);
  }

  std::vector<Assignment> on_frame(const std::vector<FrameEvent>& events, double now = 0) {
    world_.now = now;
    for (const auto& ev : events) {
      world_.apply_raw(ev.raw);
    }
    PolicyEngine engine(world_);
    return engine.propose();
  }

  void run_stdio() {
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    Config cfg;
    if (!(std::cin >> cfg.k >> cfg.s >> cfg.latency_ms >> cfg.bandwidth_gbps >>
          cfg.bytes_per_token >> cfg.num_layers)) {
      return;
    }
    if (!(std::cin >> cfg.slo1 >> cfg.slo2 >> cfg.tp_ub >> cfg.tp_base >> cfg.dist_base >>
          cfg.w_tp >> cfg.w_c)) {
      return;
    }
    int n_rows = 0;
    if (!(std::cin >> n_rows)) {
      return;
    }
    for (int i = 0; i < n_rows; i++) {
      int batch = 0;
      double cols[6];
      std::cin >> batch;
      for (double& c : cols) {
        std::cin >> c;
      }
      cfg.table.add_row(batch, cols);
    }
    cfg.table.finalize();
    std::string dummy;
    std::getline(std::cin, dummy);
    init(std::move(cfg));
    while (true) {
      std::string line;
      if (!std::getline(std::cin, line)) {
        return;
      }
      if (line.empty()) {
        continue;
      }
      if (line == "END") {
        return;
      }
      double t = 0;
      {
        std::istringstream ts(line);
        ts >> t;
      }
      int e = 0;
      if (!(std::cin >> e)) {
        return;
      }
      std::getline(std::cin, dummy);
      std::vector<FrameEvent> events;
      events.reserve(e);
      for (int i = 0; i < e; i++) {
        std::string ev;
        if (!std::getline(std::cin, ev)) {
          return;
        }
        FrameEvent fe;
        fe.raw = ev;
        events.push_back(fe);
      }
      const std::vector<Assignment> asg = on_frame(events, t);
      std::cout << asg.size() << "\n";
      for (const auto& a : asg) {
        std::cout << a.format() << "\n";
      }
      std::cout.flush();
    }
  }

private:
  Policy policy_;
  World world_;
};

} // namespace edge_cloud
