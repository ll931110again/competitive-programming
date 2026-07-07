#!/usr/bin/env bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
REPO="$(cd "$ROOT/../.." && pwd)"
BIN="$ROOT/build/a_alien_genome_assembly"
TOOLS="$ROOT/tools"
OUT="/tmp/ahc004_test_out.txt"

mkdir -p "$ROOT/build"
g++ -std=c++20 -O2 -Wall -Wextra -I"$REPO" -o "$BIN" "$ROOT/a_alien_genome_assembly.cc"

pushd "$TOOLS" >/dev/null
cargo build --release --quiet

total=0
count=0
min_score=100000000
max_score=0

for seed in $(seq -f '%04g' 0 99); do
  in="$TOOLS/in/${seed}.txt"
  "$BIN" < "$in" > "$OUT"
  score=$(cargo run --release --bin vis "$in" "$OUT" 2>/dev/null | awk '/^Score =/ {print $3}')
  total=$((total + score))
  count=$((count + 1))
  (( score < min_score )) && min_score=$score
  (( score > max_score )) && max_score=$score
  printf 'seed %s: %s\n' "$seed" "$score"
done

popd >/dev/null
echo "---"
echo "cases: $count"
echo "total: $total"
echo "avg:   $((total / count))"
echo "min:   $min_score"
echo "max:   $max_score"
