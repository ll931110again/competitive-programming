#!/usr/bin/env bash
set -euo pipefail
cd "$(dirname "$0")"

mkdir -p build
g++ -std=c++20 -O2 -Wall -Wextra -I../.. -o build/a_rect_join a_rect_join.cc
g++ -std=c++20 -O2 -I../.. -o build/verify tools/verify.cc

total=0
count=0
for f in tools/in/*.txt; do
  ./build/a_rect_join < "$f" > build/out.txt
  score=$(./build/verify "$f" build/out.txt | head -1)
  printf '%s %s\n' "$score" "$f"
  total=$((total + score))
  count=$((count + 1))
done
printf 'TOTAL %s (%s cases)\n' "$total" "$count"
