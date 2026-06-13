// Laplacian linear system solvers for electrical flows.
//
// Solves L x = b where L is a weighted graph Laplacian (symmetric diagonally
// dominant, PSD). One vertex is pinned to zero (ground) to fix the null space.
//
// Reference: Christiano et al., "Electrical Flows, Laplacian Systems, and Faster
// Approximation of Maximum Flow in Undirected Graphs" (arXiv:1010.2921).
#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <vector>

namespace laplacian {

struct Edge {
  int u = 0;
  int v = 0;
  double conductance = 0.0; // c_e = 1 / r_e
};

struct SolveOptions {
  int ground = 0;
  double cg_tolerance = 1e-8;
  int cg_max_iterations = 0;  // 0 => 10 * n
  int direct_threshold = 256; // use dense elimination when n <= this
};

inline void build_laplacian(int n, const std::vector<Edge>& edges, int ground,
                            std::vector<std::vector<double>>& matrix, std::vector<double>& rhs) {
  matrix.assign(n, std::vector<double>(n, 0.0));
  rhs.assign(n, 0.0);
  for (const Edge& e : edges) {
    if (e.conductance <= 0.0) {
      continue;
    }
    const double c = e.conductance;
    matrix[e.u][e.u] += c;
    matrix[e.v][e.v] += c;
    matrix[e.u][e.v] -= c;
    matrix[e.v][e.u] -= c;
  }
  matrix[ground][ground] = 1.0;
  for (int i = 0; i < n; ++i) {
    if (i != ground) {
      matrix[ground][i] = 0.0;
      matrix[i][ground] = 0.0;
    }
  }
  rhs[ground] = 0.0;
}

inline bool solve_dense(std::vector<std::vector<double>> a, std::vector<double> b,
                        std::vector<double>& x) {
  const int n = static_cast<int>(a.size());
  x.assign(n, 0.0);
  for (int col = 0; col < n; ++col) {
    int pivot = col;
    double best = std::abs(a[col][col]);
    for (int row = col + 1; row < n; ++row) {
      const double v = std::abs(a[row][col]);
      if (v > best) {
        best = v;
        pivot = row;
      }
    }
    if (best < 1e-14) {
      return false;
    }
    if (pivot != col) {
      std::swap(a[pivot], a[col]);
      std::swap(b[pivot], b[col]);
    }
    const double inv = 1.0 / a[col][col];
    for (int row = col + 1; row < n; ++row) {
      const double factor = a[row][col] * inv;
      if (factor == 0.0) {
        continue;
      }
      for (int k = col; k < n; ++k) {
        a[row][k] -= factor * a[col][k];
      }
      b[row] -= factor * b[col];
    }
  }
  for (int row = n - 1; row >= 0; --row) {
    double sum = b[row];
    for (int col = row + 1; col < n; ++col) {
      sum -= a[row][col] * x[col];
    }
    if (std::abs(a[row][row]) < 1e-14) {
      return false;
    }
    x[row] = sum / a[row][row];
  }
  return true;
}

inline double dot(const std::vector<double>& a, const std::vector<double>& b) {
  double s = 0.0;
  for (size_t i = 0; i < a.size(); ++i) {
    s += a[i] * b[i];
  }
  return s;
}

inline void matvec(const std::vector<std::vector<double>>& matrix, const std::vector<double>& x,
                   std::vector<double>& y) {
  const int n = static_cast<int>(matrix.size());
  y.assign(n, 0.0);
  for (int i = 0; i < n; ++i) {
    double s = 0.0;
    for (int j = 0; j < n; ++j) {
      s += matrix[i][j] * x[j];
    }
    y[i] = s;
  }
}

inline bool solve_cg(const std::vector<std::vector<double>>& matrix, const std::vector<double>& rhs,
                     double tolerance, int max_iterations, std::vector<double>& x) {
  const int n = static_cast<int>(matrix.size());
  x.assign(n, 0.0);
  std::vector<double> r(n), p(n), ap(n), z(n);
  matvec(matrix, x, r);
  for (int i = 0; i < n; ++i) {
    r[i] = rhs[i] - r[i];
    z[i] = matrix[i][i] > 0.0 ? r[i] / matrix[i][i] : r[i];
    p[i] = z[i];
  }
  double rz = dot(r, z);
  const double norm_b = std::sqrt(dot(rhs, rhs));
  const double stop = tolerance * std::max(1.0, norm_b);
  if (stop == 0.0) {
    return true;
  }
  for (int iter = 0; iter < max_iterations; ++iter) {
    matvec(matrix, p, ap);
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
    for (int i = 0; i < n; ++i) {
      z[i] = matrix[i][i] > 0.0 ? r[i] / matrix[i][i] : r[i];
    }
    const double rz_new = dot(r, z);
    const double beta = rz_new / rz;
    for (int i = 0; i < n; ++i) {
      p[i] = z[i] + beta * p[i];
    }
    rz = rz_new;
  }
  return std::sqrt(dot(r, r)) <= stop * 10.0;
}

// Solve L phi = demand with phi[ground] = 0.
inline bool solve(int n, const std::vector<Edge>& edges, const std::vector<double>& demand,
                  const SolveOptions& options, std::vector<double>& phi) {
  std::vector<std::vector<double>> matrix;
  std::vector<double> rhs;
  build_laplacian(n, edges, options.ground, matrix, rhs);
  for (int i = 0; i < n; ++i) {
    if (i != options.ground) {
      rhs[i] += demand[i];
    }
  }
  // Isolated vertices (not the ground) would leave a zero row/column and make
  // the pinned Laplacian singular. Pin them to zero potential.
  for (int i = 0; i < n; ++i) {
    if (i == options.ground) {
      continue;
    }
    if (matrix[i][i] == 0.0) {
      matrix[i][i] = 1.0;
      rhs[i] = 0.0;
    }
  }
  phi.assign(n, 0.0);
  if (n <= options.direct_threshold) {
    if (solve_dense(matrix, rhs, phi)) {
      return true;
    }
  }
  const int max_iter = options.cg_max_iterations > 0 ? options.cg_max_iterations : 10 * n;
  return solve_cg(matrix, rhs, options.cg_tolerance, max_iter, phi);
}

} // namespace laplacian
