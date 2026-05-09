#!/usr/bin/env bash
# Usage: ./build_lost_cursor_zip.sh path/to/problem-b-inputs.zip
# Requires: python3, pillow (pip install pillow)
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
ZIP="${1:?usage: $0 problem-b-inputs.zip}"
OUT="$ROOT/midnightcodecup/2026/b_submit_tmp"
rm -rf "$OUT"
mkdir -p "$OUT"
python3 "$ROOT/midnightcodecup/2026/b_solve.py" -o "$OUT" "$ZIP"
(
  cd "$OUT"
  zip -q "$ROOT/midnightcodecup/2026/lost_cursor_submission.zip" ./*.out 2>/dev/null || true
)
echo "Wrote $ROOT/midnightcodecup/2026/lost_cursor_submission.zip"
