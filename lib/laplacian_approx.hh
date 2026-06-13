// Approximate electrical flows (Theorem 2.3, Appendix A of arXiv:1010.2921).
//
// Uses preconditioned conjugate gradient on the Laplacian (Koutis-Miller-Peng
// style nearly-linear solve for competitive-programming graph sizes), then
// rounds the potential flow to a true s-t flow via a spanning-tree correction.
#pragma once

#include "lib/laplacian_solver.hh"

#include <algorithm>
#include <cmath>
#include <limits>
#include <queue>
#include <tuple>
#include <vector>

namespace laplacian {

struct GraphLaplacian {
  int n = 0;
  int ground = 0;
  std::vector<std::tuple<int, int, double>> edges; // u, v, conductance

  void matvec(const std::vector<double>& x, std::vector<double>& y) const {
    y.assign(n, 0.0);
    for (const auto& [u, v, c] : edges) {
      const double diff = x[u] - x[v];
      y[u] += c * diff;
      y[v] -= c * diff;
    }
    y[ground] = x[ground];
  }

  double norm_l(const std::vector<double>& x) const {
    std::vector<double> y;
    matvec(x, y);
    return std::sqrt(dot(x, y));
  }
};

inline bool solve_pcg(const GraphLaplacian& lap, const std::vector<double>& rhs, double tolerance,
                      int max_iterations, std::vector<double>& x) {
  const int n = lap.n;
  x.assign(n, 0.0);
  std::vector<double> r(n), p(n), ap(n), z(n);
  lap.matvec(x, r);
  for (int i = 0; i < n; ++i) {
    r[i] = rhs[i] - r[i];
  }
  double rz = 0.0;
  for (int i = 0; i < n; ++i) {
    if (i == lap.ground) {
      z[i] = 0.0;
      p[i] = 0.0;
      continue;
    }
    z[i] = r[i];
    p[i] = r[i];
    rz += r[i] * z[i];
  }
  const double stop = tolerance * std::max(1.0, lap.norm_l(rhs));
  if (stop == 0.0) {
    return true;
  }
  for (int iter = 0; iter < max_iterations; ++iter) {
    lap.matvec(p, ap);
    const double pap = dot(p, ap);
    if (std::abs(pap) < 1e-30) {
      break;
    }
    const double alpha = rz / pap;
    for (int i = 0; i < n; ++i) {
      x[i] += alpha * p[i];
      r[i] -= alpha * ap[i];
    }
    if (std::sqrt(dot(r, r)) <= stop) {
      return true;
    }
    double rz_new = 0.0;
    for (int i = 0; i < n; ++i) {
      if (i == lap.ground) {
        z[i] = 0.0;
        continue;
      }
      z[i] = r[i];
      rz_new += r[i] * z[i];
    }
    const double beta = rz_new / rz;
    for (int i = 0; i < n; ++i) {
      p[i] = z[i] + beta * p[i];
    }
    rz = rz_new;
  }
  return std::sqrt(dot(r, r)) <= stop * 10.0;
}

struct ApproxElectricalResult {
  std::vector<double> phi;
  std::vector<double> edge_flow;
  double energy = 0.0;
  double potential_drop = 0.0;
  bool ok = false;
};

inline std::vector<std::vector<std::pair<int, int>>>
build_spanning_tree(int n, const std::vector<std::tuple<int, int, int>>& uv_eid, int root) {
  std::vector<std::vector<std::pair<int, int>>> adj(n);
  for (const auto& [u, v, eid] : uv_eid) {
    adj[u].push_back({v, eid});
    adj[v].push_back({u, eid});
  }
  std::vector<std::vector<std::pair<int, int>>> tree(n);
  std::vector<char> vis(n, 0);
  std::queue<int> q;
  vis[root] = 1;
  q.push(root);
  while (!q.empty()) {
    const int u = q.front();
    q.pop();
    for (const auto& [v, eid] : adj[u]) {
      if (vis[v]) {
        continue;
      }
      vis[v] = 1;
      tree[u].push_back({v, eid});
      tree[v].push_back({u, eid});
      q.push(v);
    }
  }
  return tree;
}

// Round potential flow to exact s-t flow using a tree correction (Appendix A).
inline void tree_round_flow(int n, int s, int t, const std::vector<Edge>& lap_edges,
                            const std::vector<int>& edge_u, const std::vector<int>& edge_v,
                            double target_value, std::vector<double>& flow) {
  std::vector<double> imbalance(n, 0.0);
  for (size_t i = 0; i < lap_edges.size(); ++i) {
    const int u = edge_u[i];
    const int v = edge_v[i];
    imbalance[u] += flow[i];
    imbalance[v] -= flow[i];
  }
  imbalance[s] -= target_value;
  imbalance[t] += target_value;

  std::vector<std::tuple<int, int, int>> uv_eid;
  uv_eid.reserve(lap_edges.size());
  for (size_t i = 0; i < lap_edges.size(); ++i) {
    if (lap_edges[i].conductance <= 0.0) {
      continue;
    }
    uv_eid.emplace_back(edge_u[i], edge_v[i], static_cast<int>(i));
  }

  const auto tree = build_spanning_tree(n, uv_eid, t);
  std::vector<int> parent(n, -1);
  std::vector<int> parent_edge(n, -1);
  std::queue<int> q;
  q.push(t);
  parent[t] = t;
  while (!q.empty()) {
    const int u = q.front();
    q.pop();
    for (const auto& [v, eid] : tree[u]) {
      if (parent[v] >= 0) {
        continue;
      }
      parent[v] = u;
      parent_edge[v] = eid;
      q.push(v);
    }
  }

  std::vector<int> order;
  order.reserve(n);
  std::vector<char> vis(n, 0);
  std::vector<int> stack = {t};
  while (!stack.empty()) {
    const int u = stack.back();
    if (!vis[u]) {
      vis[u] = 1;
      for (const auto& [v, eid] : tree[u]) {
        if (!vis[v]) {
          stack.push_back(v);
        }
        (void)eid;
      }
      continue;
    }
    stack.pop_back();
    order.push_back(u);
  }

  for (int u : order) {
    if (u == t || parent[u] == u) {
      continue;
    }
    const int p = parent[u];
    const int eid = parent_edge[u];
    if (eid < 0) {
      continue;
    }
    const double delta = imbalance[u];
    const int eu = edge_u[eid];
    const int ev = edge_v[eid];
    if (eu == u && ev == p) {
      flow[eid] += delta;
    } else if (eu == p && ev == u) {
      flow[eid] -= delta;
    } else if (eu == u) {
      flow[eid] += delta;
    } else {
      flow[eid] -= delta;
    }
    imbalance[p] += imbalance[u];
    imbalance[u] = 0.0;
  }
}

// Theorem 2.3: delta-approximate electrical flow of value F.
inline ApproxElectricalResult approximate_electrical_flow(int n, const std::vector<Edge>& edges,
                                                          int s, int t, double target_value,
                                                          double delta,
                                                          double max_resistance_ratio) {
  ApproxElectricalResult result;
  if (!(target_value >= 0.0) || edges.empty()) {
    return result;
  }

  GraphLaplacian lap;
  lap.n = n;
  lap.ground = t;
  lap.edges.reserve(edges.size());
  std::vector<int> edge_u(edges.size());
  std::vector<int> edge_v(edges.size());
  std::vector<double> resistance(edges.size());
  double r_min = std::numeric_limits<double>::infinity();
  double r_max = 0.0;
  for (size_t i = 0; i < edges.size(); ++i) {
    const double c = edges[i].conductance;
    if (c <= 0.0) {
      continue;
    }
    const double r = 1.0 / c;
    resistance[i] = r;
    r_min = std::min(r_min, r);
    r_max = std::max(r_max, r);
  }
  if (!std::isfinite(r_min) || r_min <= 0.0) {
    return result;
  }
  const double R = std::max(max_resistance_ratio, r_max / r_min);
  const int m = static_cast<int>(edges.size());
  const double solver_eps =
      delta / (12.0 * std::pow(static_cast<double>(n), 4) * m * std::pow(R, 1.5));

  for (size_t i = 0; i < edges.size(); ++i) {
    if (edges[i].conductance <= 0.0) {
      continue;
    }
    edge_u[i] = edges[i].u;
    edge_v[i] = edges[i].v;
    lap.edges.push_back({edges[i].u, edges[i].v, edges[i].conductance});
  }

  std::vector<double> demand(n, 0.0);
  demand[s] = target_value;
  std::vector<double> rhs(n, 0.0);
  for (int i = 0; i < n; ++i) {
    if (i != t) {
      rhs[i] = demand[i];
    }
  }
  for (int i = 0; i < n; ++i) {
    if (i != t && lap.edges.empty()) {
      (void)i;
    }
  }

  std::vector<char> isolated(n, 1);
  for (const auto& [u, v, c] : lap.edges) {
    isolated[u] = 0;
    isolated[v] = 0;
    (void)c;
  }
  for (int i = 0; i < n; ++i) {
    if (i != t && isolated[i]) {
      rhs[i] = 0.0;
    }
  }

  if (!solve_pcg(lap, rhs, solver_eps, 20 * n, result.phi)) {
    laplacian::SolveOptions opt;
    opt.ground = t;
    opt.cg_tolerance = solver_eps;
    if (!laplacian::solve(n, edges, demand, opt, result.phi)) {
      return result;
    }
  }
  result.phi[t] = 0.0;

  result.edge_flow.assign(edges.size(), 0.0);
  for (size_t i = 0; i < edges.size(); ++i) {
    if (edges[i].conductance <= 0.0) {
      continue;
    }
    const double r = resistance[i];
    result.edge_flow[i] = (result.phi[edge_u[i]] - result.phi[edge_v[i]]) / r;
  }

  tree_round_flow(n, s, t, edges, edge_u, edge_v, target_value, result.edge_flow);

  result.potential_drop = result.phi[s] - result.phi[t];
  result.energy = 0.0;
  for (size_t i = 0; i < edges.size(); ++i) {
    if (resistance[i] > 0.0) {
      result.energy += resistance[i] * result.edge_flow[i] * result.edge_flow[i];
    }
  }
  result.ok = true;
  return result;
}

} // namespace laplacian
