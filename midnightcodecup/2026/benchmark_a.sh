#!/usr/bin/env bash
# Score tests 01..08 against outputs already in a_submit_tmp/*.out (does not run the solver).
# Refresh outputs first: ./build_beutsche_zip.sh  (rebuilds binaries + overwrites a_submit_tmp).
# Usage:
#   ./benchmark_a.sh                           # unzip problem-a-inputs.zip next to script
#   ./benchmark_a.sh path/to/problem-a-inputs.zip
set -euo pipefail
MID="$(cd "$(dirname "$0")" && pwd)"
ZIP="${1:-$MID/problem-a-inputs.zip}"
TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT
unzip -q -o "$ZIP" -d "$TMP/in"

python3 "$MID/judge_beutsche.py" --batch "$TMP/in" "$MID/a_submit_tmp"
