#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT"
mkdir -p build/out

g++ -std=c++17 -O2 -I../../ -o build/a_sliding_tree_puzzle a_sliding_tree_puzzle.cc
g++ -std=c++17 -O2 -I../../ -o build/verify tools/verify.cc

total=0
count=0
for f in tools/in/*.txt; do
  out="build/out/$(basename "$f")"
  ./build/a_sliding_tree_puzzle < "$f" > "$out"
  line="$((cat "$f"; cat "$out") | ./build/verify)"
  score="${line##*score=}"
  total=$((total + score))
  count=$((count + 1))
done
echo "cases=$count total_score=$total avg=$((total / count))"
