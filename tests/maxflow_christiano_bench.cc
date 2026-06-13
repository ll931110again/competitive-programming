// Benchmark: undirected Dinic (exact) vs Christiano electrical-flow max flow.

#include "lib/maxflow_christiano.hh"
#include "lib/maxflow_dinic.hh"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>

namespace {

using Clock = std::chrono::steady_clock;

struct StressRng {
  uint64_t state;

  explicit StressRng(uint64_t seed) : state(seed + 1) {}

  uint64_t next_u64() {
    state = state * 6364136223846793005ULL + 1442695040888963407ULL;
    return state;
  }

  int rand_int(int lo, int hi) {
    if (hi <= lo) {
      return lo;
    }
    return lo + static_cast<int>(next_u64() % static_cast<uint64_t>(hi - lo + 1));
  }
};

struct FlowGraph {
  int n = 0;
  std::vector<std::tuple<int, int, int>> edges;

  void add_edge(int u, int v, int cap) {
    edges.emplace_back(u, v, cap);
  }
};

maxflow::christiano::Graph to_christiano(const FlowGraph& g) {
  maxflow::christiano::Graph out;
  out.n = g.n;
  for (const auto& [u, v, cap] : g.edges) {
    out.add_edge(u, v, static_cast<double>(cap));
  }
  return out;
}

int undirected_dinic_value(const FlowGraph& graph, int s, int t) {
  maxflow::Dinic dinic(graph.n);
  for (const auto& [u, v, cap] : graph.edges) {
    dinic.add_edge(u, v, cap);
    dinic.add_edge(v, u, cap);
  }
  return dinic.max_flow(s, t);
}

maxflow::christiano::Config bench_config() {
  maxflow::christiano::Config config;
  config.epsilon = 0.2;
  config.improved = true;
  config.max_mw_rounds = 20;
  config.flow_delta = 0.05;
  return config;
}

FlowGraph random_undirected_graph(StressRng& rng, int n, int m) {
  FlowGraph g;
  g.n = n;
  for (int i = 0; i < m; ++i) {
    const int u = rng.rand_int(0, n - 2);
    const int v = rng.rand_int(u + 1, n - 1);
    g.add_edge(u, v, rng.rand_int(1, 100));
  }
  g.add_edge(0, n - 1, rng.rand_int(10, 200));
  for (int i = 0; i + 1 < n; ++i) {
    if (rng.rand_int(0, 2) == 0) {
      g.add_edge(i, i + 1, rng.rand_int(5, 100));
    }
  }
  return g;
}

FlowGraph grid_graph(int h, int w, int cap) {
  FlowGraph graph;
  graph.n = h * w;
  auto id = [w](int r, int c) { return r * w + c; };
  for (int r = 0; r < h; ++r) {
    for (int c = 0; c < w; ++c) {
      if (r + 1 < h) {
        graph.add_edge(id(r, c), id(r + 1, c), cap);
      }
      if (c + 1 < w) {
        graph.add_edge(id(r, c), id(r, c + 1), cap);
      }
    }
  }
  return graph;
}

FlowGraph layered_network(int layers, int width, uint64_t seed) {
  FlowGraph graph;
  graph.n = layers * width;
  StressRng rng(seed);
  for (int L = 0; L + 1 < layers; ++L) {
    for (int i = 0; i < width; ++i) {
      for (int j = 0; j < width; ++j) {
        if (rng.rand_int(0, 2) == 0) {
          graph.add_edge(L * width + i, (L + 1) * width + j, rng.rand_int(5, 50));
        }
      }
    }
  }
  for (int i = 0; i + 1 < width; ++i) {
    graph.add_edge(i, i + 1, rng.rand_int(20, 80));
    graph.add_edge(graph.n - width + i, graph.n - width + i + 1, rng.rand_int(20, 80));
  }
  return graph;
}

FlowGraph figure1_graph(int k) {
  FlowGraph graph;
  graph.n = k * (k - 1) + 2;
  graph.add_edge(0, 1, k + 1);
  for (int i = 0; i < k; ++i) {
    int prev = 0;
    for (int j = 1; j < k; ++j) {
      const int node = 2 + i * (k - 1) + (j - 1);
      graph.add_edge(prev, node, k + 1);
      prev = node;
    }
    graph.add_edge(prev, 1, k + 1);
  }
  return graph;
}

struct BenchCase {
  std::string name;
  FlowGraph graph;
  int s = 0;
  int t = 0;
};

template <typename Fn> double time_min_ms(int reps, Fn fn) {
  double best = 1e300;
  for (int i = 0; i < reps; ++i) {
    const auto t0 = Clock::now();
    fn();
    const auto t1 = Clock::now();
    const double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();
    best = std::min(best, ms);
  }
  return best;
}

struct BenchRow {
  std::string name;
  int n = 0;
  int m = 0;
  int dinic_value = 0;
  double electrical_value = 0.0;
  double ratio = 0.0;
  double dinic_ms = 0.0;
  double electrical_ms = 0.0;
  double slowdown = 0.0;
};

BenchRow run_case(const BenchCase& bench, const maxflow::christiano::Config& config, int reps) {
  const int s = bench.s;
  const int t = bench.t;
  const auto& graph = bench.graph;
  const auto christiano_g = to_christiano(graph);

  int dinic_val = 0;
  const double dinic_time =
      time_min_ms(reps, [&] { dinic_val = undirected_dinic_value(graph, s, t); });

  maxflow::christiano::FlowResult electrical;
  const double electrical_time = time_min_ms(reps, [&] {
    electrical = maxflow::christiano::approximate_max_flow(christiano_g, s, t, config);
  });

  BenchRow row;
  row.name = bench.name;
  row.n = graph.n;
  row.m = static_cast<int>(graph.edges.size());
  row.dinic_value = dinic_val;
  row.electrical_value = electrical.ok ? electrical.value : -1.0;
  row.ratio = dinic_val > 0 ? row.electrical_value / dinic_val : 0.0;
  row.dinic_ms = dinic_time;
  row.electrical_ms = electrical_time;
  row.slowdown = row.dinic_ms > 0.0 ? row.electrical_ms / row.dinic_ms : 0.0;
  return row;
}

void print_table(const std::vector<BenchRow>& rows) {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::left << std::setw(22) << "case" << std::right << std::setw(5) << "n"
            << std::setw(6) << "m" << std::setw(8) << "dinic" << std::setw(10) << "elect"
            << std::setw(7) << "ratio" << std::setw(10) << "din_ms" << std::setw(10) << "ef_ms"
            << std::setw(8) << "ef/din" << '\n';
  std::cout << std::string(86, '-') << '\n';

  double total_dinic = 0.0;
  double total_electrical = 0.0;
  for (const auto& row : rows) {
    std::cout << std::left << std::setw(22) << row.name << std::right << std::setw(5) << row.n
              << std::setw(6) << row.m << std::setw(8) << row.dinic_value << std::setw(10)
              << row.electrical_value << std::setw(7) << row.ratio << std::setw(10) << row.dinic_ms
              << std::setw(10) << row.electrical_ms << std::setw(8) << row.slowdown << '\n';
    total_dinic += row.dinic_ms;
    total_electrical += row.electrical_ms;
  }
  std::cout << std::string(86, '-') << '\n';
  std::cout << std::left << std::setw(22) << "TOTAL" << std::right << std::setw(5) << ""
            << std::setw(6) << "" << std::setw(8) << "" << std::setw(10) << "" << std::setw(7) << ""
            << std::setw(10) << total_dinic << std::setw(10) << total_electrical << std::setw(8)
            << (total_dinic > 0.0 ? total_electrical / total_dinic : 0.0) << '\n';
}

std::vector<BenchCase> build_cases() {
  std::vector<BenchCase> cases;

  for (int seed = 0; seed < 4; ++seed) {
    StressRng rng(5000 + seed);
    const int n = rng.rand_int(64, 96);
    const int m = rng.rand_int(3 * n, 6 * n);
    cases.push_back(
        {"random_s" + std::to_string(seed), random_undirected_graph(rng, n, m), 0, n - 1});
  }

  {
    const int h = 20;
    const int w = 20;
    auto g = grid_graph(h, w, 10);
    cases.push_back({"grid_20x20", std::move(g), 0, h * w - 1});
  }

  {
    const int layers = 8;
    const int width = 15;
    auto g = layered_network(layers, width, 4242);
    cases.push_back({"layered_8x15", std::move(g), 0, layers * width - 1});
  }

  for (int k : {4, 5, 6}) {
    auto g = figure1_graph(k);
    cases.push_back({"figure1_k" + std::to_string(k), std::move(g), 0, 1});
  }

  return cases;
}

} // namespace

int main(int argc, char** argv) {
  int reps = 3;
  if (argc >= 2) {
    reps = std::max(1, std::atoi(argv[1]));
  }

  const auto config = bench_config();
  std::cout << "Christiano vs Dinic benchmark (undirected max flow)\n";
  std::cout << "epsilon=" << config.epsilon << " mw_rounds=" << config.max_mw_rounds
            << " reps=" << reps << " (min time per algo)\n\n";

  const auto cases = build_cases();
  std::vector<BenchRow> rows;
  rows.reserve(cases.size());
  for (const auto& bench : cases) {
    rows.push_back(run_case(bench, config, reps));
  }
  print_table(rows);
  return 0;
}
