#!/usr/bin/env bash
# Usage: ./build_beutsche_zip.sh [path/to/problem-a-inputs.zip]
# Default: ./problem-a-inputs.zip next to this script.
# Expects inputs at tests/01 … tests/08 inside the zip (Midnight Code Cup layout).
set -euo pipefail
MID="$(cd "$(dirname "$0")" && pwd)"
ZIP="${1:-$MID/problem-a-inputs.zip}"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

g++ -std=c++20 -O2 -o "$TMP/bd" "$MID/beutsche_dahn.cc"

unzip -q -o "$ZIP" -d "$TMP/in"

OUT="$MID/a_submit_tmp"
rm -rf "$OUT"
mkdir -p "$OUT"

for i in 01 02 03 04 05 06 07 08; do
  if [[ -f "$TMP/in/tests/$i" ]]; then
    "$TMP/bd" <"$TMP/in/tests/$i" >"$OUT/$i.out"
  elif [[ -f "$TMP/in/$i" ]]; then
    "$TMP/bd" <"$TMP/in/$i" >"$OUT/$i.out"
  elif [[ -f "$TMP/in/$i.in" ]]; then
    "$TMP/bd" <"$TMP/in/$i.in" >"$OUT/$i.out"
  fi
done

(
  cd "$OUT"
  zip -q "$MID/problem-a-outputs.zip" ./*.out 2>/dev/null || true
)
echo "Wrote $MID/problem-a-outputs.zip"
