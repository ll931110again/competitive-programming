#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT"
mkdir -p build
g++ -std=c++17 -O2 -I../../ -o build/sol a_probabilistic_waste_sorting.cc
total=0
for i in $(seq -w 0 4); do
  ./build/sol < "tools/in/${i}.txt" > build/out.txt
  score=$(cd tools && cargo run -r --bin vis "in/${i}.txt" ../build/out.txt 2>/dev/null | awk -F= '/Score/{print $2}')
  total=$((total + score))
  echo "seed $i score=$score"
done
echo "total=$total avg=$((total / 5))"
