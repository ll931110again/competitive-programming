// Approximate max-flow / min-cut via electrical flows (Christiano et al.).
//
// Implements Algorithms 1-5 from:
//   "Electrical Flows, Laplacian Systems, and Faster Approximation of Maximum
//    Flow in Undirected Graphs" (arXiv:1010.2921, STOC 2011).
//
// Components:
//   - Electrical flow oracle (Section 3.2, Algorithm 2)
//   - Multiplicative weights wrapper (Section 3.1, Algorithm 1)
//   - Improved oracle with edge removal (Section 4.1, Algorithms 3-4)
//   - Dual min-cut algorithm (Section 5, Algorithm 5)
//   - Binary search for maximum flow value
//   - Theorem 2.3 approximate Laplacian solver (Appendix A)
//   - Karger graph smoothing (Section 4.4, Theorem 4.5/4.6)
//   - Benczur-Karger sparsification (Section 4.5, Theorem 5.1)
#pragma once

#include "lib/graph_smoothing.hh"
#include "lib/graph_sparsifier.hh"
#include "lib/laplacian_approx.hh"
#include "lib/laplacian_solver.hh"
#include "lib/nagamochi_ibaraki.hh"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>
#include <queue>
#include <set>
#include <tuple>
#include <vector>

namespace maxflow {
namespace christiano {

struct Graph {
  int n = 0;
  struct Edge {
    int u = 0;
    int v = 0;
    double cap = 0.0;
  };
  std::vector<Edge> edges;

  void add_edge(int u, int v, double cap) {
    if (u > v) {
      std::swap(u, v);
    }
    edges.push_back({u, v, cap});
  }

  int m() const {
    return static_cast<int>(edges.size());
  }
};

struct Config {
  double epsilon = 0.1;
  bool improved = true;
  // 0 => full N = 2 rho ln(m) / epsilon^2 from Algorithm 1; >0 overrides for tests.
  int max_mw_rounds = 0;
  // Theorem 2.3 accuracy parameter for the electrical-flow solver.
  double flow_delta = 0.05;
  bool approximate_solver = true;
  uint64_t rng_seed = 1;
  double cg_tolerance = 1e-9;
};

struct FlowResult {
  std::vector<double> edge_flow;
  double value = 0.0;
  bool ok = false;
};

struct CutResult {
  std::vector<int> side; // 1 = source side, 0 = sink side
  double capacity = 0.0;
  bool ok = false;
};

struct FlowValueResult {
  double value = 0.0;
  bool ok = false;
};

namespace detail {

inline nagamochi_ibaraki::WeightedGraph to_weighted(const Graph& graph) {
  nagamochi_ibaraki::WeightedGraph w;
  w.init(graph.n);
  for (const auto& e : graph.edges) {
    w.add_edge(e.u, e.v, e.cap);
  }
  return w;
}

inline Graph from_weighted(const nagamochi_ibaraki::WeightedGraph& w) {
  Graph g;
  g.n = w.n;
  for (const auto& e : w.edges) {
    g.add_edge(e.u, e.v, e.cap);
  }
  return g;
}

inline double resistance_ratio(const std::vector<double>& resistances) {
  double r_min = std::numeric_limits<double>::infinity();
  double r_max = 0.0;
  for (double r : resistances) {
    if (r > 0.0 && std::isfinite(r)) {
      r_min = std::min(r_min, r);
      r_max = std::max(r_max, r);
    }
  }
  if (!std::isfinite(r_min) || r_min <= 0.0) {
    return 1.0;
  }
  return r_max / r_min;
}

inline double l1(const std::vector<double>& w) {
  double s = 0.0;
  for (double x : w) {
    s += x;
  }
  return s;
}

inline double energy(const Graph& graph, const std::vector<double>& resistances,
                     const std::vector<double>& flow) {
  double e = 0.0;
  for (size_t i = 0; i < graph.edges.size(); ++i) {
    e += resistances[i] * flow[i] * flow[i];
  }
  return e;
}

inline double congestion(double flow, double cap) {
  if (cap <= 0.0) {
    return std::numeric_limits<double>::infinity();
  }
  return std::abs(flow) / cap;
}

inline std::vector<laplacian::Edge> active_laplacian_edges(const Graph& graph,
                                                           const std::vector<double>& resistances,
                                                           const std::set<int>& forbidden) {
  std::vector<laplacian::Edge> out;
  out.reserve(graph.edges.size());
  for (size_t i = 0; i < graph.edges.size(); ++i) {
    if (forbidden.count(static_cast<int>(i))) {
      continue;
    }
    const double r = resistances[i];
    if (!(r > 0.0) || !std::isfinite(r)) {
      continue;
    }
    const auto& e = graph.edges[i];
    out.push_back({e.u, e.v, 1.0 / r});
  }
  return out;
}

// Compute electrical s-t flow of value F (Theorem 2.3 when approximate_solver).
inline FlowResult electrical_flow(const Graph& graph, int s, int t, double target_value,
                                  const std::vector<double>& resistances,
                                  const std::set<int>& forbidden, const Config& config) {
  FlowResult result;
  result.edge_flow.assign(graph.edges.size(), 0.0);
  if (!(target_value >= 0.0)) {
    return result;
  }
  const auto lap_edges = active_laplacian_edges(graph, resistances, forbidden);
  if (lap_edges.empty() && target_value > 0.0) {
    return result;
  }

  if (config.approximate_solver) {
    std::vector<laplacian::Edge> solver_edges;
    std::vector<int> edge_index;
    solver_edges.reserve(lap_edges.size());
    edge_index.reserve(lap_edges.size());
    for (size_t i = 0; i < graph.edges.size(); ++i) {
      if (forbidden.count(static_cast<int>(i))) {
        continue;
      }
      const double r = resistances[i];
      if (!(r > 0.0) || !std::isfinite(r)) {
        continue;
      }
      const auto& e = graph.edges[i];
      solver_edges.push_back({e.u, e.v, 1.0 / r});
      edge_index.push_back(static_cast<int>(i));
    }
    const double R = resistance_ratio(resistances);
    const auto approx = laplacian::approximate_electrical_flow(graph.n, solver_edges, s, t,
                                                               target_value, config.flow_delta, R);
    if (!approx.ok) {
      return result;
    }
    for (size_t j = 0; j < edge_index.size(); ++j) {
      result.edge_flow[edge_index[j]] = approx.edge_flow[j];
    }
    result.value = target_value;
    result.ok = true;
    return result;
  }

  std::vector<double> demand(graph.n, 0.0);
  demand[s] = 1.0;
  demand[t] = -1.0;

  laplacian::SolveOptions options;
  options.ground = t;
  options.cg_tolerance = config.cg_tolerance;

  std::vector<double> phi;
  if (!laplacian::solve(graph.n, lap_edges, demand, options, phi)) {
    return result;
  }

  std::vector<double> unit_flow(graph.edges.size(), 0.0);
  for (size_t i = 0; i < graph.edges.size(); ++i) {
    if (forbidden.count(static_cast<int>(i))) {
      continue;
    }
    const double r = resistances[i];
    if (!(r > 0.0) || !std::isfinite(r)) {
      continue;
    }
    const auto& e = graph.edges[i];
    unit_flow[i] = (phi[e.u] - phi[e.v]) / r;
  }

  for (size_t i = 0; i < graph.edges.size(); ++i) {
    result.edge_flow[i] = unit_flow[i] * target_value;
  }
  result.value = target_value;
  result.ok = true;
  return result;
}

inline std::vector<double> oracle_resistances(const Graph& graph,
                                              const std::vector<double>& weights, double epsilon) {
  const double w_sum = l1(weights);
  std::vector<double> r(graph.edges.size());
  for (size_t i = 0; i < graph.edges.size(); ++i) {
    const double cap = graph.edges[i].cap;
    r[i] = (weights[i] + epsilon * w_sum / (3.0 * graph.m())) / (cap * cap);
  }
  return r;
}

// Algorithm 2: simple (epsilon, 3*sqrt(m/epsilon)) oracle.
inline FlowResult simple_oracle(const Graph& graph, int s, int t, double target_value,
                                const std::vector<double>& weights, double epsilon,
                                const Config& config) {
  const std::set<int> forbidden;
  const std::vector<double> r = oracle_resistances(graph, weights, epsilon);
  FlowResult flow = electrical_flow(graph, s, t, target_value, r, forbidden, config);
  if (!flow.ok) {
    return flow;
  }
  const double w_sum = l1(weights);
  if (energy(graph, r, flow.edge_flow) > (1.0 + epsilon) * w_sum) {
    flow.ok = false;
  }
  return flow;
}

// Algorithm 3: improved oracle with edge removal.
inline FlowResult improved_oracle(const Graph& graph, int s, int t, double target_value,
                                  const std::vector<double>& weights, double epsilon, double rho,
                                  std::set<int>& forbidden, const Config& config) {
  while (true) {
    const std::vector<double> r = oracle_resistances(graph, weights, epsilon);
    FlowResult flow = electrical_flow(graph, s, t, target_value, r, forbidden, config);
    if (!flow.ok) {
      return flow;
    }
    const double w_sum = l1(weights);
    if (energy(graph, r, flow.edge_flow) > (1.0 + epsilon) * w_sum) {
      flow.ok = false;
      return flow;
    }
    int bad = -1;
    double worst = 0.0;
    for (size_t i = 0; i < graph.edges.size(); ++i) {
      if (forbidden.count(static_cast<int>(i))) {
        continue;
      }
      const double cong = congestion(flow.edge_flow[i], graph.edges[i].cap);
      if (cong > rho && cong > worst) {
        worst = cong;
        bad = static_cast<int>(i);
      }
    }
    if (bad < 0) {
      return flow;
    }
    forbidden.insert(bad);
  }
}

inline double width_simple(const Graph& graph, double epsilon) {
  return 3.0 * std::sqrt(static_cast<double>(graph.m()) / epsilon);
}

inline double width_improved(const Graph& graph, double epsilon) {
  const double ln_m = std::log(std::max(2.0, static_cast<double>(graph.m())));
  return 8.0 * std::pow(graph.m(), 1.0 / 3.0) * std::pow(ln_m, 1.0 / 3.0) / epsilon;
}

inline int mw_iterations(double rho, double epsilon, int m) {
  const double ln_m = std::log(std::max(2.0, static_cast<double>(m)));
  return static_cast<int>(std::ceil(2.0 * rho * ln_m / (epsilon * epsilon)));
}

// Algorithm 1 / 4: multiplicative weights for a target flow value F.
inline FlowResult multiplicative_weights(const Graph& graph, int s, int t, double target_value,
                                         const Config& config) {
  FlowResult fail;
  if (!(config.epsilon > 0.0 && config.epsilon < 0.5)) {
    return fail;
  }
  const double eps = config.epsilon;
  const double rho = config.improved ? width_improved(graph, eps) : width_simple(graph, eps);
  int rounds = mw_iterations(rho, eps, graph.m());
  if (config.max_mw_rounds > 0) {
    rounds = config.max_mw_rounds;
  }

  std::vector<double> weights(graph.edges.size(), 1.0);
  std::set<int> forbidden;
  std::vector<std::vector<double>> flows;
  flows.reserve(rounds);

  for (int i = 0; i < rounds; ++i) {
    FlowResult step;
    if (config.improved) {
      step = improved_oracle(graph, s, t, target_value, weights, eps, rho, forbidden, config);
    } else {
      step = simple_oracle(graph, s, t, target_value, weights, eps, config);
    }
    if (!step.ok) {
      return fail;
    }
    flows.push_back(step.edge_flow);
    for (size_t e = 0; e < graph.edges.size(); ++e) {
      const double cong = congestion(step.edge_flow[e], graph.edges[e].cap);
      weights[e] *= (1.0 + eps / rho * cong);
    }
  }

  const double scale = (1.0 - eps) * (1.0 - eps) / ((1.0 + eps) * rounds);
  FlowResult out;
  out.edge_flow.assign(graph.edges.size(), 0.0);
  for (const auto& f : flows) {
    for (size_t e = 0; e < graph.edges.size(); ++e) {
      out.edge_flow[e] += scale * f[e];
    }
  }
  // Each round sends target_value units; Algorithm 1 scales the average by
  // (1-eps)^2 / (1+eps).
  out.value = target_value * (1.0 - eps) * (1.0 - eps) / (1.0 + eps);
  out.ok = true;
  return out;
}

inline bool is_feasible(const Graph& graph, const std::vector<double>& flow) {
  for (size_t i = 0; i < graph.edges.size(); ++i) {
    if (congestion(flow[i], graph.edges[i].cap) > 1.0 + 1e-6) {
      return false;
    }
  }
  return true;
}

inline double bottleneck_capacity(const Graph& graph, int s, int t) {
  std::vector<double> best(graph.n, 0.0);
  best[s] = std::numeric_limits<double>::infinity();
  using State = std::pair<double, int>;
  std::priority_queue<State> pq;
  pq.push({best[s], s});
  while (!pq.empty()) {
    const auto [cap, u] = pq.top();
    pq.pop();
    if (cap < best[u]) {
      continue;
    }
    if (u == t) {
      return cap;
    }
    for (const auto& e : graph.edges) {
      int v = -1;
      if (e.u == u) {
        v = e.v;
      } else if (e.v == u) {
        v = e.u;
      } else {
        continue;
      }
      const double ncap = std::min(cap, e.cap);
      if (ncap > best[v]) {
        best[v] = ncap;
        pq.push({ncap, v});
      }
    }
  }
  return 0.0;
}

inline double flow_value(const Graph& graph, int s, int t, const std::vector<double>& edge_flow) {
  double out = 0.0;
  for (size_t i = 0; i < graph.edges.size(); ++i) {
    const auto& e = graph.edges[i];
    if (e.u == s) {
      out += edge_flow[i];
    } else if (e.v == s) {
      out -= edge_flow[i];
    }
  }
  (void)t;
  return out;
}

} // namespace detail

// Core Algorithm 4 max-flow (m^{4/3}) without smoothing/sparsification.
inline FlowResult approximate_max_flow_core(const Graph& graph, int s, int t,
                                            const Config& config) {
  using namespace detail;
  FlowResult fail;
  if (graph.n <= 0 || s < 0 || t < 0 || s >= graph.n || t >= graph.n || s == t) {
    return fail;
  }

  const double B = bottleneck_capacity(graph, s, t);
  if (B <= 0.0) {
    FlowResult zero;
    zero.edge_flow.assign(graph.edges.size(), 0.0);
    zero.value = 0.0;
    zero.ok = true;
    return zero;
  }

  double lo = B;
  double hi = B * graph.m();
  FlowResult best;
  best.ok = false;

  for (int iter = 0; iter < 40; ++iter) {
    const double mid = 0.5 * (lo + hi);
    FlowResult candidate = multiplicative_weights(graph, s, t, mid, config);
    if (candidate.ok && is_feasible(graph, candidate.edge_flow)) {
      best = candidate;
      lo = mid;
    } else {
      hi = mid;
    }
    if (hi - lo <= std::max(1.0, hi) * config.epsilon * 0.25) {
      break;
    }
  }

  if (!best.ok) {
    FlowResult candidate = multiplicative_weights(graph, s, t, lo, config);
    if (candidate.ok && is_feasible(graph, candidate.edge_flow)) {
      best = candidate;
    }
  }

  if (best.ok) {
    best.value = flow_value(graph, s, t, best.edge_flow);
  }
  return best;
}

// Theorem 4.6: O~(m n^{1/3} epsilon^{-11/3}) via Karger smoothing (Section 4.4).
inline FlowResult approximate_max_flow_smoothed(const Graph& graph, int s, int t,
                                                const Config& config = {}) {
  using namespace detail;
  if (graph.m() <= graph.n * 2) {
    return approximate_max_flow_core(graph, s, t, config);
  }
  const auto weighted = to_weighted(graph);
  const double c = static_cast<double>(weighted.m()) / std::max(1, weighted.n);
  const graph_smoothing::SmoothGraph smooth = graph_smoothing::make_smooth(weighted, c);
  const int groups =
      graph_smoothing::num_groups_for_smoothness(smooth.graph.m(), smooth.graph.n, config.epsilon);
  const auto parts = graph_smoothing::partition_with_origins(smooth, groups, config.rng_seed);

  FlowResult combined;
  combined.edge_flow.assign(graph.edges.size(), 0.0);
  combined.ok = false;
  double total_value = 0.0;

  for (const auto& [part, origins] : parts) {
    if (part.m() == 0) {
      continue;
    }
    const Graph sub = from_weighted(part);
    const FlowResult piece = approximate_max_flow_core(sub, s, t, config);
    if (!piece.ok) {
      continue;
    }
    combined.ok = true;
    total_value += piece.value;
    for (size_t i = 0; i < piece.edge_flow.size(); ++i) {
      const int orig = origins[i];
      if (orig >= 0 && orig < static_cast<int>(combined.edge_flow.size())) {
        combined.edge_flow[orig] += piece.edge_flow[i];
      }
    }
  }

  if (combined.ok) {
    combined.value = flow_value(graph, s, t, combined.edge_flow);
    (void)total_value;
  }
  return combined;
}

// Default entry point: smoothed algorithm when m >> n, else core m^{4/3}.
inline FlowResult approximate_max_flow(const Graph& graph, int s, int t,
                                       const Config& config = {}) {
  if (graph.m() > graph.n * 4) {
    return approximate_max_flow_smoothed(graph, s, t, config);
  }
  return approximate_max_flow_core(graph, s, t, config);
}

// Section 4.5: approximate max-flow value via Benczur-Karger sparsifier.
inline FlowValueResult approximate_max_flow_value(const Graph& graph, int s, int t,
                                                  const Config& config = {}) {
  FlowValueResult out;
  graph_sparsifier::SparsifierOptions sp_opts;
  sp_opts.epsilon = config.epsilon;
  sp_opts.seed = config.rng_seed;
  const auto sparse = graph_sparsifier::sparsify(detail::to_weighted(graph), sp_opts);
  if (!sparse.ok) {
    return out;
  }
  const Graph sparse_g = detail::from_weighted(sparse.graph);
  const FlowResult flow = approximate_max_flow_core(sparse_g, s, t, config);
  if (!flow.ok) {
    return out;
  }
  out.value = flow.value;
  out.ok = true;
  return out;
}

namespace detail {

inline double cut_capacity(const Graph& graph, const std::vector<int>& side) {
  double cap = 0.0;
  for (size_t i = 0; i < graph.edges.size(); ++i) {
    const auto& e = graph.edges[i];
    const bool u = side[e.u] != 0;
    const bool v = side[e.v] != 0;
    if (u != v) {
      cap += e.cap;
    }
  }
  return cap;
}

inline CutResult cut_from_potentials(const Graph& graph, int s, int t,
                                     const std::vector<double>& phi, double target_flow) {
  std::vector<double> values;
  values.reserve(graph.n);
  for (int v = 0; v < graph.n; ++v) {
    values.push_back(phi[v]);
  }
  std::sort(values.begin(), values.end());
  values.erase(std::unique(values.begin(), values.end(),
                           [](double a, double b) { return std::abs(a - b) < 1e-12; }),
               values.end());

  CutResult best;
  best.ok = false;
  best.capacity = std::numeric_limits<double>::infinity();

  for (double x : values) {
    std::vector<int> side(graph.n, 0);
    for (int v = 0; v < graph.n; ++v) {
      side[v] = phi[v] > x ? 1 : 0;
    }
    if (side[s] == 0 || side[t] != 0) {
      continue;
    }
    const double cap = cut_capacity(graph, side);
    if (cap < best.capacity) {
      best.capacity = cap;
      best.side = side;
      best.ok = true;
    }
  }

  if (best.ok && best.capacity < target_flow / (1.0 - 7.0 * 0.5)) {
    (void)target_flow;
  }
  return best;
}

} // namespace detail

// Algorithm 5: (1 + O(epsilon))-approximate minimum s-t cut.
inline CutResult approximate_min_cut(const Graph& graph, int s, int t, double target_flow_hint,
                                     const Config& config = {}) {
  using namespace detail;
  CutResult fail;
  if (graph.n <= 0 || s < 0 || t < 0 || s >= graph.n || t >= graph.n || s == t) {
    return fail;
  }
  const double eps = config.epsilon;
  if (!(eps > 0.0 && eps < 1.0 / 7.0)) {
    return fail;
  }

  const double rho = 3.0 * std::pow(graph.m(), 1.0 / 3.0) * std::pow(eps, -2.0 / 3.0);
  const double ln_m = std::log(std::max(2.0, static_cast<double>(graph.m())));
  const int rounds = static_cast<int>(
      std::ceil(5.0 * std::pow(eps, -8.0 / 3.0) * std::pow(graph.m(), 1.0 / 3.0) * ln_m));
  const double delta = eps * eps;
  (void)delta;

  std::vector<double> weights(graph.edges.size(), 1.0);
  CutResult best;
  best.ok = false;
  best.capacity = std::numeric_limits<double>::infinity();

  const double F = target_flow_hint > 0.0 ? target_flow_hint : bottleneck_capacity(graph, s, t);

  for (int round = 0; round < rounds; ++round) {
    std::vector<double> resistances(graph.edges.size());
    for (size_t i = 0; i < graph.edges.size(); ++i) {
      const double cap = graph.edges[i].cap;
      resistances[i] = weights[i] / (cap * cap);
    }

    const std::set<int> forbidden;
    FlowResult flow = electrical_flow(graph, s, t, F, resistances, forbidden, config);
    if (!flow.ok) {
      continue;
    }

    std::vector<double> demand(graph.n, 0.0);
    demand[s] = 1.0;
    demand[t] = -1.0;
    const auto lap_edges = active_laplacian_edges(graph, resistances, forbidden);
    laplacian::SolveOptions options;
    options.ground = t;
    options.cg_tolerance = config.cg_tolerance;
    std::vector<double> phi;
    if (!laplacian::solve(graph.n, lap_edges, demand, options, phi)) {
      continue;
    }

    const double drop = phi[s] - phi[t];
    if (std::abs(drop) < 1e-14) {
      continue;
    }
    for (int v = 0; v < graph.n; ++v) {
      phi[v] /= drop;
    }
    phi[s] = 1.0;
    phi[t] = 0.0;

    CutResult cut = cut_from_potentials(graph, s, t, phi, F);
    if (cut.ok && cut.capacity < best.capacity) {
      best = cut;
    }

    if (cut.ok && cut.capacity < F / (1.0 - 7.0 * eps)) {
      return cut;
    }

    const double mu = l1(weights);
    for (size_t i = 0; i < graph.edges.size(); ++i) {
      const double cong = congestion(flow.edge_flow[i], graph.edges[i].cap);
      weights[i] += (eps / rho) * cong * weights[i] + (eps * eps / (graph.m() * rho)) * mu;
    }
  }

  if (best.ok) {
    return best;
  }
  return fail;
}

// Theorem 5.1: min-cut on sparsifier (valid on original graph by BK guarantee).
inline CutResult approximate_min_cut_sparsified(const Graph& graph, int s, int t,
                                                const Config& config = {}) {
  graph_sparsifier::SparsifierOptions sp_opts;
  sp_opts.epsilon = config.epsilon;
  sp_opts.seed = config.rng_seed + 17;
  const auto sparse = graph_sparsifier::sparsify(detail::to_weighted(graph), sp_opts);
  if (!sparse.ok) {
    return CutResult{};
  }
  const Graph sparse_g = detail::from_weighted(sparse.graph);
  const double hint = detail::bottleneck_capacity(graph, s, t);
  return approximate_min_cut(sparse_g, s, t, hint, config);
}

// Exposed helpers for tests and debugging.
inline double effective_resistance(const Graph& graph, int s, int t,
                                   const std::vector<double>& resistances) {
  const std::set<int> forbidden;
  Config exact_cfg;
  exact_cfg.approximate_solver = false;
  exact_cfg.cg_tolerance = 1e-10;
  const FlowResult unit =
      detail::electrical_flow(graph, s, t, 1.0, resistances, forbidden, exact_cfg);
  if (!unit.ok) {
    return std::numeric_limits<double>::infinity();
  }
  std::vector<double> demand(graph.n, 0.0);
  demand[s] = 1.0;
  demand[t] = -1.0;
  const auto lap_edges = detail::active_laplacian_edges(graph, resistances, forbidden);
  laplacian::SolveOptions options;
  options.ground = t;
  std::vector<double> phi;
  if (!laplacian::solve(graph.n, lap_edges, demand, options, phi)) {
    return std::numeric_limits<double>::infinity();
  }
  return phi[s] - phi[t];
}

} // namespace christiano
} // namespace maxflow
