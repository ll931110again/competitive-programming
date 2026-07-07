#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT"
mkdir -p build out

g++ -std=c++17 -O2 -I../../ -o build/a_territory a_territory.cc

total=0
count=0
for f in "$ROOT"/tools/in/*.txt; do
  base="$(basename "$f")"
  out="$ROOT/out/${base%.txt}.out"
  score="$(cd "$ROOT/tools" && cargo run --release --bin tester "$ROOT/build/a_territory" < "in/$base" > "$out" 2>&1 | rg 'Score = ' | tail -1 | rg -o '[0-9]+$' || true)"
  score="${score:-0}"
  total=$((total + score))
  count=$((count + 1))
done
echo "cases=$count total_score=$total avg=$((total / count))"
