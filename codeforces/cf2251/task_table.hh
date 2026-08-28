#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <utility>
#include <vector>

namespace edge_cloud {

enum class Step {
  PrefillPre,
  PrefillProc,
  PrefillPost,
  DecodePre,
  DecodeProc,
  DecodePost,
};

struct TaskTable {
  std::vector<std::pair<int, double>> points[6];

  void add_row(int batch_size, const double* cols) {
    for (int c = 0; c < 6; c++) {
      if (cols[c] >= 0) {
        points[c].push_back({batch_size, cols[c]});
      }
    }
  }

  void finalize() {
    for (int c = 0; c < 6; c++) {
      std::sort(points[c].begin(), points[c].end());
    }
  }

  double lookup(Step step, int size) const {
    const auto& pts = points[static_cast<int>(step)];
    if (pts.empty()) {
      return 1.0;
    }
    if (size <= pts.front().first) {
      return pts.front().second;
    }
    if (size >= pts.back().first) {
      return pts.back().second;
    }
    int lo = 0;
    int hi = static_cast<int>(pts.size()) - 1;
    while (lo < hi) {
      const int mid = (lo + hi) / 2;
      if (pts[mid].first >= size) {
        hi = mid;
      } else {
        lo = mid + 1;
      }
    }
    const int x0 = pts[lo - 1].first;
    const int x1 = pts[lo].first;
    const double y0 = pts[lo - 1].second;
    const double y1 = pts[lo].second;
    if (x1 == x0) {
      return y0;
    }
    const double t = double(size - x0) / double(x1 - x0);
    return y0 + t * (y1 - y0);
  }
};

inline double
transfer_ms(double latency_ms, double bandwidth_gbps, int64_t bytes_per_token, int len) {
  const double data_bytes = double(len) * double(bytes_per_token);
  return latency_ms + 8.0 * data_bytes / (bandwidth_gbps * 1e6);
}

inline double clamp01(double x, double base, double target) {
  if (target == base) {
    return x >= target ? 1.0 : 0.0;
  }
  const double u = (x - base) / (target - base);
  return std::max(0.0, std::min(1.0, u));
}

struct ScoreBreakdown {
  double tp = 0;
  double tdr = 0;
  double tpot = 0;
  double dist = 0;
  double score = 0;
};

inline ScoreBreakdown compute_score(double tp_base,
                                    double tp_ub,
                                    double slo1,
                                    double slo2,
                                    double dist_base,
                                    double w_tp,
                                    double w_c,
                                    double earliest_arr,
                                    double last_token,
                                    double mean_tdr,
                                    double mean_tpot,
                                    int64_t total_tokens) {
  ScoreBreakdown out;
  const double elapsed = std::max(1e-12, last_token - earliest_arr);
  out.tp = double(total_tokens) / elapsed;
  out.tdr = mean_tdr;
  out.tpot = mean_tpot;
  const double excess_tdr = std::max(0.0, (mean_tdr - slo1) / slo1);
  const double excess_tpot = std::max(0.0, (mean_tpot - slo2) / slo2);
  out.dist = std::sqrt(excess_tdr * excess_tdr + excess_tpot * excess_tpot);
  const double c_tp = clamp01(out.tp, tp_base, tp_ub);
  double c_wait = 0;
  if (dist_base > 0) {
    c_wait = std::max(0.0, 1.0 - out.dist / dist_base);
  } else {
    c_wait = (out.dist == 0 ? 1.0 : 0.0);
  }
  out.score = 1000.0 * (w_tp * c_tp + w_c * c_wait);
  return out;
}

} // namespace edge_cloud
