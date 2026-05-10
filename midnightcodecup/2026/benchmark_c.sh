#!/usr/bin/env bash
# Profile problem C locally: compile c.cc, run official generator + interactor per seed.
#
# Usage:
#   ./benchmark_c.sh                    # seeds 1..10, g++ c.cc -> ./c_runner
#   ./benchmark_c.sh 20                 # seeds 1..20
#   FIRST=5 LAST=15 ./benchmark_c.sh    # seeds 5..15
#   RFC_MAX_QUERIES=2000 ./benchmark_c.sh   # cap queries per seed (fast iteration)
#
# Pitfall: with a raw pipe, env vars apply only to the left command:
#   RFC_MAX_QUERIES=800 gen | python3 ...   # WRONG — Python never sees RFC_MAX_QUERIES
# Use: export RFC_MAX_QUERIES=800  OR  run this script (children inherit its environment).
#
set -euo pipefail
MID="$(cd "$(dirname "$0")" && pwd)"
MAX_SEED="${1:-10}"
FIRST="${FIRST:-1}"
LAST="${LAST:-$MAX_SEED}"

CXX="${CXX:-g++}"
STD="${CXXSTD:-c++20}"
OPT="${EXTRA_CXXFLAGS:--O2}"
BIN="${BIN:-$MID/c_runner}"
ZIP="${ZIP:-$MID/problem-c-generator.zip}"

if [[ -n "${RFC_MAX_QUERIES:-}" ]]; then
  echo "# benchmark_c: RFC_MAX_QUERIES=${RFC_MAX_QUERIES}" >&2
fi

if [[ ! -f "$ZIP" ]]; then
  echo "missing $ZIP (generator archive)" >&2
  exit 1
fi

TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT
unzip -q -o "$ZIP" -d "$TMP"
"$CXX" -std="$STD" $OPT -Wall -Wextra -o "$BIN" "$MID/c.cc"
GEN_SRC="$TMP/files/generator.cpp"
"$CXX" -std=c++17 -O2 -o "$TMP/gen" "$GEN_SRC" -I"$TMP/files"

echo -e "seed\tqueries\thits\tqueries_per_hit"
tot_q=0
tot_h=0
for ((s = FIRST; s <= LAST; s++)); do
  line="$("$TMP/gen" "$s" 2>/dev/null | python3 "$MID/run_fix_repeat_interactor.py" "$BIN" 2>&1 | grep '^# local_done' || true)"
  if [[ -z "$line" ]]; then
    echo -e "$s\tFAIL\tFAIL\t-"
    continue
  fi
  # line: # local_done queries=10000 hits=42
  q="${line#*queries=}"
  q="${q%% hits*}"
  h="${line##*hits=}"
  echo -e "$s\t$q\t$h\t$(awk -v q="$q" -v h="$h" 'BEGIN { if (h+0>0) printf "%.2f", q/h; else print "inf" }')"
  tot_q=$((tot_q + q))
  tot_h=$((tot_h + h))
done

echo "---"
echo "total_queries=$tot_q total_hits=$tot_h seeds=$((LAST - FIRST + 1))"
if [[ "$tot_h" -gt 0 ]]; then
  awk -v q="$tot_q" -v h="$tot_h" 'BEGIN { printf "avg_queries_per_hit=%.3f\n", q/h }'
fi
