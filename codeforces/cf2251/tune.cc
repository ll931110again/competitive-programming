// Grid search over online scheduling parameters against stress scenarios.
#include "scheduler.hh"
#include "simulator.hh"

#include <algorithm>
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

struct Bench {
  std::string name;
  Scenario sc;
};

double eval_policy(const Policy& p, const std::vector<Bench>& benches) {
  double sum = 0;
  int n = 0;
  for (const auto& b : benches) {
    const SimResult r = run_policy(b.sc, p);
    if (!r.ok) {
      return -1e18;
    }
    sum += r.score.score;
    n++;
  }
  return sum / double(n);
}

void apply_variant(Policy& p, int variant) {
  p.online_adapt = true;
  p.auto_tune = false;
  switch (variant % 6) {
  case 0:
    p.slo_pressure_gain = 0.8;
    p.batch_wait_age_frac = 0.3;
    p.base_admit_every = 3;
    break;
  case 1:
    p.slo_pressure_gain = 1.2;
    p.batch_wait_age_frac = 0.4;
    p.base_admit_every = 4;
    break;
  case 2:
    p.slo_pressure_gain = 1.6;
    p.batch_wait_age_frac = 0.5;
    p.base_admit_every = 5;
    break;
  case 3:
    p.slo_pressure_gain = 2.0;
    p.batch_wait_age_frac = 0.35;
    p.base_admit_every = 4;
    break;
  case 4:
    p.slo_pressure_gain = 1.0;
    p.batch_wait_age_frac = 0.45;
    p.base_admit_every = 6;
    break;
  default:
    p.slo_pressure_gain = 1.4;
    p.batch_wait_age_frac = 0.38;
    p.base_admit_every = 4;
    break;
  }
  p.cloud_xfer_weight = 32.0 + 16.0 * (variant / 6);
  p.decode_fairness = (variant % 3) * 0.3;
  p.cloud_load_weight = 48.0 + 16.0 * ((variant / 3) % 3);
  p.prefill_wave_cap = 6 + (variant % 5);
  p.batch_wait_max = 24 + 8 * (variant % 3);
}

std::vector<Bench> make_benches() {
  std::vector<Bench> out;
  std::mt19937_64 rng(2251);

  for (int i = 0; i < 40; i++) {
    out.push_back({"rand" + std::to_string(i), random_scenario(rng, 1, 40, i % 4 == 0)});
  }

  {
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
    out.push_back({"slo", slo});
  }

  {
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
    out.push_back({"fat", fat});
  }

  {
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
    out.push_back({"burst", burst});
  }

  {
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
    out.push_back({"mix", mix});
  }

  {
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
    out.push_back({"cheap", cheap});
  }

  return out;
}

} // namespace

int main() {
  const std::vector<Bench> benches = make_benches();

  Policy baseline;
  baseline.online_adapt = false;
  baseline.auto_tune = false;
  const double base_score = eval_policy(baseline, benches);
  std::cout << "baseline (no online) mean=" << base_score << "\n";

  Policy best = baseline;
  best.online_adapt = true;
  double best_score = eval_policy(best, benches);
  int best_v = -1;

  for (int v = 0; v < 54; v++) {
    Policy p;
    apply_variant(p, v);
    const double s = eval_policy(p, benches);
    if (s > best_score) {
      best_score = s;
      best = p;
      best_v = v;
    }
  }

  std::cout << "best variant=" << best_v << " mean=" << best_score
            << " delta=" << (best_score - base_score) << "\n";
  std::cout << "  slo_pressure_gain=" << best.slo_pressure_gain
            << " batch_wait_age_frac=" << best.batch_wait_age_frac
            << " base_admit_every=" << best.base_admit_every
            << " cloud_xfer_weight=" << best.cloud_xfer_weight
            << " decode_fairness=" << best.decode_fairness
            << " cloud_load_weight=" << best.cloud_load_weight
            << " prefill_wave_cap=" << best.prefill_wave_cap
            << " batch_wait_max=" << best.batch_wait_max << "\n";

  Policy prod;
  prod = best;
  prod.online_adapt = true;
  const double prod_score = eval_policy(prod, benches);
  std::cout << "production policy mean=" << prod_score << "\n";
  return prod_score >= base_score - 1e-6 ? 0 : 1;
}
