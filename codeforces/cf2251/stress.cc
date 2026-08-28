// Local harness: example 1, random stress, and policy comparison.
#include "scheduler.hh"
#include "simulator.hh"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>

namespace {

using edge_cloud::example1;
using edge_cloud::Policy;
using edge_cloud::Scenario;
using edge_cloud::Scheduler;
using edge_cloud::SimResult;
using edge_cloud::Simulator;

Scenario with_table(Scenario sc, const std::vector<int>& batches, const double cols[6]) {
  for (int b : batches) {
    sc.cfg.table.add_row(b, cols);
  }
  sc.cfg.table.finalize();
  return sc;
}

Scenario random_scenario(std::mt19937_64& rng, int r_lo, int r_hi, bool burst) {
  std::uniform_int_distribution<int> dk(1, 8);
  std::uniform_int_distribution<int> ds(1, 4);
  std::uniform_int_distribution<int> dlayers(1, 16);
  std::uniform_int_distribution<int> dr(r_lo, r_hi);
  std::uniform_int_distribution<int> dlin(1, 64);
  std::uniform_int_distribution<int> dlout(1, 8);
  std::uniform_real_distribution<double> dgap(0.0, 20.0);
  std::uniform_real_distribution<double> dw(0.0, 1.0);

  Scenario sc;
  sc.cfg.k = dk(rng);
  sc.cfg.s = double(ds(rng));
  sc.cfg.latency_ms = 2;
  sc.cfg.bandwidth_gbps = 1;
  sc.cfg.bytes_per_token = 125000;
  sc.cfg.num_layers = dlayers(rng);
  sc.cfg.slo1 = 80;
  sc.cfg.slo2 = 40;
  sc.cfg.tp_ub = 0.2;
  sc.cfg.tp_base = 0.01;
  sc.cfg.dist_base = 2;
  sc.cfg.w_tp = dw(rng);
  sc.cfg.w_c = 1.0 - sc.cfg.w_tp;
  const double cols[] = {3, 10, 2, 1, 4, 1};
  sc = with_table(std::move(sc), {1, 2, 4, 8, 16, 32}, cols);

  const int r = dr(rng);
  double t = 0;
  for (int i = 0; i < r; i++) {
    if (i > 0 && !burst) {
      t += dgap(rng);
    }
    sc.reqs.push_back({t, dlin(rng), dlout(rng)});
  }
  return sc;
}

SimResult run_policy(const Scenario& sc, const Policy& policy) {
  Scheduler sched;
  sched.set_policy(policy);
  Simulator sim;
  return sim.run(sc, sched);
}

void require(bool cond, const std::string& msg) {
  if (!cond) {
    std::cerr << "FAIL: " << msg << "\n";
    std::exit(1);
  }
}

void check_example1() {
  Scheduler sched;
  Simulator sim;
  const SimResult res = sim.run(example1(), sched);
  require(res.ok, "example 1 protocol: " + res.error);
  require(std::abs(res.last_time - 45.0) < 1e-6, "example 1 last token should be 45 ms");
  require(std::abs(res.score.tdr - 30.0) < 1e-6, "example 1 TDR should be 30 ms");
  require(std::abs(res.score.score - 500.0) < 1e-3, "example 1 score should be 500");
  std::cout << "example1 ok  score=" << res.score.score << " t=" << res.last_time
            << " frames=" << res.frames << "\n";

  Scheduler s;
  s.init(example1().cfg);
  using FE = edge_cloud::FrameEvent;
  FE arr{FE::Type::Arr, "ARR 0 4"};
  const auto a0 = s.on_frame({arr}, 0);
  require(a0.size() == 1 && a0[0].format() == "E P PRE 0 0", "first assignment");
  FE tdn{FE::Type::Tdn, "TDN E P PRE 0 0 3.000000000"};
  require(s.on_frame({tdn}, 4).empty(), "should idle during prefill UP");
  FE xdn{FE::Type::Xdn, "XDN UP 0 500000 PRE 1 0"};
  const auto a1 = s.on_frame({xdn}, 10);
  require(a1.size() == 1 && a1[0].format() == "C0 P PROC 0 4 0 0", "full prefill piece");
}

struct Agg {
  int n = 0;
  int fail = 0;
  double score_sum = 0;
  double best = 0;
  double worst = 1e100;
};

void add(Agg& a, const SimResult& r) {
  a.n++;
  if (!r.ok) {
    a.fail++;
    return;
  }
  a.score_sum += r.score.score;
  a.best = std::max(a.best, r.score.score);
  a.worst = std::min(a.worst, r.score.score);
}

void show(const char* name, const Agg& a) {
  const double mean = a.n == a.fail ? 0 : a.score_sum / double(a.n - a.fail);
  std::cout << name << " n=" << a.n << " fail=" << a.fail << " mean=" << mean << " best=" << a.best
            << " worst=" << (a.fail == a.n ? 0 : a.worst) << "\n";
}

} // namespace

int main() {
  check_example1();

  Policy prod;
  prod.auto_tune = false;
  Policy no_batch = prod;
  no_batch.batch = false;
  Policy no_wait = prod;
  no_wait.wait_to_batch = false;
  Policy split = prod;
  split.split_mode = 1;
  Policy always_split = prod;
  always_split.split_mode = 2;
  Policy no_smart = prod;
  no_smart.smart_batch = false;

  Agg a_prod, a_nobatch, a_nowait, a_split, a_alwayssplit, a_nosmart;
  std::mt19937_64 rng(2251);
  constexpr int kTrials = 120;
  for (int i = 0; i < kTrials; i++) {
    const bool burst = (i % 4 == 0);
    const Scenario sc = random_scenario(rng, 1, 40, burst);
    const SimResult r0 = run_policy(sc, prod);
    add(a_prod, r0);
    if (!r0.ok) {
      std::cerr << "prod failed trial " << i << ": " << r0.error << " stuck=" << r0.stuck
                << " R=" << sc.reqs.size() << " K=" << sc.cfg.k << " layers=" << sc.cfg.num_layers
                << "\n";
    }
    add(a_nobatch, run_policy(sc, no_batch));
    add(a_nowait, run_policy(sc, no_wait));
    add(a_split, run_policy(sc, split));
    add(a_alwayssplit, run_policy(sc, always_split));
    add(a_nosmart, run_policy(sc, no_smart));
  }

  show("prod         ", a_prod);
  show("no_batch     ", a_nobatch);
  show("no_wait      ", a_nowait);
  show("split_shared ", a_split);
  show("always_split ", a_alwayssplit);
  show("no_smart     ", a_nosmart);
  require(a_prod.fail == 0, "production policy failed a random test");

  {
    std::mt19937_64 big_rng(2251 * 19);
    const Scenario big = random_scenario(big_rng, 200, 400, true);
    const SimResult big_r = run_policy(big, prod);
    require(big_r.ok, "large burst failed: " + big_r.error);
    std::cout << "large-burst  R=" << big.reqs.size() << " K=" << big.cfg.k
              << " score=" << big_r.score.score << " frames=" << big_r.frames << "\n";
  }

  Scenario two = example1();
  two.cfg.k = 2;
  two.cfg.dist_base = 2;
  two.cfg.tp_base = 0;
  two.reqs = {{0.0, 4, 3}, {1.0, 4, 3}};
  const SimResult g = run_policy(two, prod);
  const SimResult s = run_policy(two, no_batch);
  require(g.ok && s.ok, "two-request run failed: " + g.error + " / " + s.error);
  require(g.score.score + 1e-9 >= s.score.score, "batching lost to serial on the overlap case");
  std::cout << "two-request  batch=" << g.score.score << " serial=" << s.score.score << "\n";

  Scenario layers1 = example1();
  layers1.cfg.num_layers = 1;
  layers1.reqs = {{0.0, 4, 2}, {5.0, 8, 1}};
  const SimResult l1 = run_policy(layers1, prod);
  require(l1.ok, "num_layers=1 failed: " + l1.error);
  std::cout << "layers1 ok   score=" << l1.score.score << "\n";

  Scenario steep = example1();
  steep.cfg.k = 1;
  steep.cfg.dist_base = 5;
  steep.cfg.tp_base = 0;
  steep.cfg.table = {};
  const double c1[] = {3, 10, 2, 1, 1, 1};
  const double c2[] = {3, 10, 2, 3, 3, 3};
  const double c4[] = {3, 10, 2, 20, 20, 20};
  const double c8[] = {3, 10, 2, 80, 80, 80};
  steep.cfg.table.add_row(1, c1);
  steep.cfg.table.add_row(2, c2);
  steep.cfg.table.add_row(4, c4);
  steep.cfg.table.add_row(8, c8);
  steep.cfg.table.finalize();
  steep.reqs = {{0, 4, 2}, {0, 4, 2}, {0, 4, 2}, {0, 4, 2}};
  const SimResult sm = run_policy(steep, prod);
  const SimResult fm = run_policy(steep, no_smart);
  require(sm.ok && fm.ok, "superlinear table failed: " + sm.error + " / " + fm.error);
  std::cout << "superlinear  smart=" << sm.score.score << " full=" << fm.score.score << "\n";

  // Pure-latency (prelim #3 shape): waiting-time only, tight SLOs.
  Scenario slo = example1();
  slo.cfg.k = 2;
  slo.cfg.w_tp = 0;
  slo.cfg.w_c = 1;
  slo.cfg.dist_base = 0;
  slo.cfg.slo1 = 80;
  slo.cfg.slo2 = 40;
  slo.cfg.tp_base = 0;
  slo.cfg.tp_ub = 1;
  slo.reqs = {{0, 4, 4}, {2, 4, 4}, {4, 8, 3}, {6, 4, 4}, {8, 4, 2}};
  const SimResult slo_r = run_policy(slo, prod);
  require(slo_r.ok, "slo scenario failed: " + slo_r.error);
  std::cout << "slo          score=" << slo_r.score.score << " tdr=" << slo_r.score.tdr
            << " tpot=" << slo_r.score.tpot << " dist=" << slo_r.score.dist << "\n";

  // Staggered arrivals: decode run-to-completion must not starve later prefills.
  Scenario slo_stagger = slo;
  slo_stagger.cfg.slo1 = 200;
  slo_stagger.cfg.slo2 = 80;
  slo_stagger.cfg.dist_base = 5;
  slo_stagger.reqs.clear();
  for (int i = 0; i < 16; i++) {
    slo_stagger.reqs.push_back({i * 10.0, 4, 8});
  }
  const SimResult slo_st = run_policy(slo_stagger, prod);
  require(slo_st.ok, "slo stagger failed: " + slo_st.error);
  std::cout << "slo-stagger  score=" << slo_st.score.score << " tdr=" << slo_st.score.tdr
            << " tpot=" << slo_st.score.tpot << " dist=" << slo_st.score.dist << "\n";

  // Fat network: waiting on prefill-down would stall decode for a long time.
  Scenario fat = example1();
  fat.cfg.k = 2;
  fat.cfg.w_tp = 1;
  fat.cfg.w_c = 0;
  fat.cfg.dist_base = 10;
  fat.cfg.tp_base = 0;
  fat.cfg.tp_ub = 0.5;
  fat.cfg.latency_ms = 50;
  fat.cfg.bandwidth_gbps = 0.05;
  fat.cfg.bytes_per_token = 500000;
  fat.reqs.clear();
  for (int i = 0; i < 40; i++) {
    fat.reqs.push_back({i * 5.0, 32, 8});
  }
  const SimResult fat_r = run_policy(fat, prod);
  const SimResult fat_s = run_policy(fat, no_batch);
  require(fat_r.ok, "fat network failed: " + fat_r.error);
  require(fat_s.ok, "fat network serial failed: " + fat_s.error);
  std::cout << "fat-net      batch=" << fat_r.score.score << " serial=" << fat_s.score.score
            << " tp=" << fat_r.score.tp << " t=" << fat_r.last_time << "\n";

  // Throughput burst (prelim #5/#19 shape).
  Scenario burst = example1();
  burst.cfg.k = 4;
  burst.cfg.w_tp = 1;
  burst.cfg.w_c = 0;
  burst.cfg.dist_base = 50;
  burst.cfg.tp_base = 0.01;
  burst.cfg.tp_ub = 0.8;
  burst.cfg.slo1 = 1e6;
  burst.cfg.slo2 = 1e6;
  burst.reqs.clear();
  for (int i = 0; i < 80; i++) {
    burst.reqs.push_back({0.0, 16, 16});
  }
  const SimResult burst_r = run_policy(burst, prod);
  const SimResult burst_s = run_policy(burst, no_batch);
  require(burst_r.ok && burst_s.ok, "burst failed: " + burst_r.error);
  require(burst_r.score.score + 1e-9 >= burst_s.score.score, "throughput burst lost to serial");
  std::cout << "tp-burst     batch=" << burst_r.score.score << " serial=" << burst_s.score.score
            << " tp=" << burst_r.score.tp << "\n";

  // High-latency decode-heavy: cohort batching should beat serial decode UPs.
  Scenario slowd = example1();
  slowd.cfg.k = 2;
  slowd.cfg.w_tp = 1;
  slowd.cfg.w_c = 0;
  slowd.cfg.dist_base = 10;
  slowd.cfg.tp_base = 0;
  slowd.cfg.tp_ub = 0.2;
  slowd.cfg.latency_ms = 50;
  slowd.cfg.bandwidth_gbps = 1;
  slowd.reqs.clear();
  for (int i = 0; i < 24; i++) {
    slowd.reqs.push_back({0.0, 4, 32});
  }
  const SimResult sd_b = run_policy(slowd, prod);
  const SimResult sd_s = run_policy(slowd, no_batch);
  require(sd_b.ok && sd_s.ok, "slow-decode failed: " + sd_b.error);
  require(sd_b.score.score + 1e-9 >= sd_s.score.score, "slow-decode lost to serial");
  std::cout << "slow-decode  batch=" << sd_b.score.score << " serial=" << sd_s.score.score
            << " tp=" << sd_b.score.tp << " t=" << sd_b.last_time << "\n";

  // 50/50 weights must still batch (prelim #22 was TLE on size-1 decode).
  Scenario mix = example1();
  mix.cfg.k = 4;
  mix.cfg.w_tp = 0.5;
  mix.cfg.w_c = 0.5;
  mix.cfg.dist_base = 5;
  mix.cfg.tp_base = 0;
  mix.cfg.tp_ub = 0.5;
  mix.cfg.slo1 = 1e4;
  mix.cfg.slo2 = 1e4;
  mix.reqs.clear();
  for (int i = 0; i < 60; i++) {
    mix.reqs.push_back({0.0, 4, 48});
  }
  const SimResult mix_r = run_policy(mix, prod);
  const SimResult mix_s = run_policy(mix, no_batch);
  require(mix_r.ok, "balanced high-token failed: " + mix_r.error);
  require(mix_r.frames < 80000, "balanced high-token too many frames");
  require(mix_r.score.score + 1e-9 >= mix_s.score.score, "balanced high-token lost to serial");
  std::cout << "balanced     score=" << mix_r.score.score << " frames=" << mix_r.frames
            << " serial_frames=" << mix_s.frames << "\n";

  // Cheap-net 50/50 like prelim #22: mixed D PRE, no wait, 2K admit.
  Scenario cheap = example1();
  cheap.cfg.k = 4;
  cheap.cfg.w_tp = 0.5;
  cheap.cfg.w_c = 0.5;
  cheap.cfg.dist_base = 200;
  cheap.cfg.tp_base = 0;
  cheap.cfg.tp_ub = 2.0;
  cheap.cfg.slo1 = 200;
  cheap.cfg.slo2 = 20;
  cheap.reqs.clear();
  for (int i = 0; i < 32; i++) {
    cheap.reqs.push_back({0.0, 4, 24});
  }
  const SimResult ch_b = run_policy(cheap, prod);
  const SimResult ch_s = run_policy(cheap, no_batch);
  require(ch_b.ok, "cheap 50/50 failed: " + ch_b.error);
  require(ch_b.score.tpot + 1e-9 <= ch_s.score.tpot * 3.0 + 50.0, "cheap 50/50 TPOT exploded");
  require(ch_b.score.tp + 1e-9 >= ch_s.score.tp, "cheap 50/50 lost throughput");
  std::cout << "cheap-50/50  score=" << ch_b.score.score << " tp=" << ch_b.score.tp
            << " tpot=" << ch_b.score.tpot << " tdr=" << ch_b.score.tdr
            << " serial_tp=" << ch_s.score.tp << "\n";

  std::cout << "stress ok\n";
  return 0;
}
