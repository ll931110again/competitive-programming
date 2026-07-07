#!/usr/bin/env bash
# Build and score a_conveyor_design.cc on official inputs (seeds FIRST..LAST).
#
# Usage (from repo root or this directory):
#   ./tools/run_local.sh
#   FIRST=0 LAST=4 ./tools/run_local.sh
set -euo pipefail

TOOLS="$(cd "$(dirname "$0")" && pwd)"
AHC="$(cd "$TOOLS/.." && pwd)"
REPO="$(cd "$AHC/../.." && pwd)"
FIRST="${FIRST:-0}"
LAST="${LAST:-99}"

mkdir -p "$AHC/build"
g++ -std=c++17 -O2 -Wall -Wextra -I "$REPO" \
  -o "$AHC/build/a_conveyor_design" "$AHC/a_conveyor_design.cc"
g++ -std=c++17 -O2 -Wall -Wextra -I "$REPO" \
  -o "$AHC/build/verify" "$TOOLS/verify.cc"

BIN="$AHC/build/a_conveyor_design"
VER="$AHC/build/verify"

total=0
count=0
min=999999999
max=0
for ((s = FIRST; s <= LAST; s++)); do
  in=$(printf "$TOOLS/in/%04d.txt" "$s")
  out="$(mktemp)"
  "$BIN" < "$in" > "$out"
  line="$((cat "$in"; cat "$out") | "$VER")"
  score="${line##*score=}"
  total=$((total + score))
  count=$((count + 1))
  if ((score < min)); then min=$score; fi
  if ((score > max)); then max=$score; fi
  printf "seed=%02d %s\n" "$s" "$line"
  rm -f "$out"
done

echo "---"
echo "seeds=$count total=$total avg=$((total / count)) min=$min max=$max"
