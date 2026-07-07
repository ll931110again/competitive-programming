#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT"
mkdir -p build

g++ -std=c++17 -O2 -I../../ -o build/a_food_delivery a_food_delivery.cc
g++ -std=c++17 -O2 -I../../ -o build/verify tools/verify.cc

total=0
count=0
for f in tools/in/*.txt; do
  out="$(mktemp)"
  ./build/a_food_delivery < "$f" > "$out"
  line="$((cat "$f"; cat "$out") | ./build/verify)"
  score="${line##*score=}"
  total=$((total + score))
  count=$((count + 1))
  rm -f "$out"
done
echo "cases=$count total_score=$total avg=$((total / count))"
