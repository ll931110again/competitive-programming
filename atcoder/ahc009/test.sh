#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
REPO="$(cd "$ROOT/../.." && pwd)"
BIN="$ROOT/build/a_robust_memory"
TOOLS="$ROOT/tools"

mkdir -p "$ROOT/build"
g++ -std=c++20 -O2 -Wall -Wextra -I"$REPO" -o "$BIN" "$ROOT/a_robust_memory.cc"

pushd "$TOOLS" >/dev/null
cargo build --release --quiet

total=0
count=0
min_score=1000000000
max_score=0

for seed in $(seq 0 99); do
  id=$(printf '%04d' "$seed")
  in="$TOOLS/in/${id}.txt"
  out="$(mktemp)"
  "$BIN" < "$in" > "$out"
  score=$(cargo run --release --bin vis "$in" "$out" 2>/dev/null | awk '/^Score =/ {print $3}')
  rm -f "$out"
  total=$((total + score))
  count=$((count + 1))
  (( score < min_score )) && min_score=$score
  (( score > max_score )) && max_score=$score
  printf 'seed %s: %s\n' "$id" "$score"
done

popd >/dev/null
echo "---"
echo "cases: $count"
echo "total: $total"
echo "avg:   $((total / count))"
echo "min:   $min_score"
echo "max:   $max_score"
