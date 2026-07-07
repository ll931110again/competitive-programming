#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT"
mkdir -p build

g++ -std=c++20 -O2 -pipe -I../.. -o build/a_patrolling a_patrolling.cc
g++ -std=c++20 -O2 -pipe -I../.. -o build/verify tools/verify.cc

total=0
count=0
for f in tools/in/*.txt; do
  out="$(mktemp)"
  ./build/a_patrolling < "$f" > "$out"
  line="$((cat "$f"; cat "$out") | ./build/verify)"
  score="${line##*score=}"
  total=$((total + score))
  count=$((count + 1))
  rm -f "$out"
done
echo "cases=$count total_score=$total avg=$((total / count))"
