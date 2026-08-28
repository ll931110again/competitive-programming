#!/bin/bash
# Flatten modular headers into a single Codeforces submission.
set -euo pipefail
cd "$(dirname "$0")"
OUT=a_edge_cloud_scheduling.cc

strip_header() {
  sed \
    -e '/^#pragma once$/d' \
    -e '/^namespace edge_cloud {$/d' \
    -e '/^} \/\/ namespace edge_cloud$/d' \
    -e '/^#include /d'
}

{
  cat <<'EOF'
// Codeforces 2251A — Edge–Cloud Collaborative Scheduling
// https://codeforces.com/contest/2251/problem/A
//
// Sketch
// ---
// Interactive edge+cloud LLM scheduler. Each request is prefill then decode;
// each phase is a six-step pipeline (P/D × PRE/PROC/POST). The edge owns PRE
// and POST; clouds own PROC. One uplink and one downlink, both FIFO.
//
// At INIT the instance is classified as SLO / Latency / Throughput from
// (w_tp, w_c, dist_base), plus tp_heavy (w_tp ≥ 0.7) and slow_link (transfer
// dominates decode compute). Per frame we apply ARR/TDN/XDN/FIN and assign at
// most one edge task plus one task per idle cloud.
//
// Edge always prefers P POST. Throughput-heavy uses prefill waves and
// decode-streak admission; SLO/Latency uses a 1-ply edge_utility. Decode
// groups are FIFO prefixes capped at a precomputed k* (amortized throughput,
// plus a pipeline round-trip across K clouds). Optional analytic auto_tune
// picks among ~96 policy variants from the INIT tables. No future arrivals.
//
// Time per frame is O(R log R + K·C·R) with small C; R ≤ 2000, K ≤ 8.
#include <bits/stdc++.h>

namespace edge_cloud {

EOF
  strip_header < task_table.hh
  echo
  strip_header < scheduler.hh
  cat <<'EOF'

} // namespace edge_cloud

int main() {
  edge_cloud::Scheduler sched;
  sched.run_stdio();
  return 0;
}
EOF
} > "$OUT"
echo "Wrote $OUT ($(wc -l < "$OUT") lines)"
