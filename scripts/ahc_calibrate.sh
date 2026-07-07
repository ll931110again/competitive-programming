#!/usr/bin/env bash
# Score an AHC solution on official provisional tests (see atcoder/AHC_PROVISIONAL_TESTS.json).
#
# Interactive contests: most `tester` binaries read input from stdin and take
# `<command> [<args>]`. AHC003-style testers take `<input> <command>`.
#
# Usage:
#   scripts/ahc_calibrate.sh ahc067           # all provisional seeds 0..N-1
#   scripts/ahc_calibrate.sh ahc002 0 1 2     # subset only (quick check)
set -euo pipefail

CONTEST=${1:?usage: ahc_calibrate.sh ahcNNN [seed ...]}
shift

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DIR="$ROOT/atcoder/$CONTEST"
JSON="$ROOT/atcoder/AHC_PROVISIONAL_TESTS.json"
TOOLS="$DIR/tools"

if [[ ! -d "$TOOLS" ]]; then
  echo "missing tools/: $TOOLS" >&2
  exit 1
fi

read -r PROVISIONAL LOCAL_IN <<<"$(python3 - "$CONTEST" "$JSON" <<'PY'
import json, sys
cid, path = sys.argv[1], sys.argv[2]
e = json.load(open(path))[cid]
print(e.get("provisional", 0), e.get("local_in", 0))
PY
)"

if [[ "$PROVISIONAL" == "0" || "$PROVISIONAL" == "None" ]]; then
  echo "no provisional count for $CONTEST in $JSON" >&2
  exit 1
fi

if [[ $# -gt 0 ]]; then
  SEEDS=("$@")
else
  SEEDS=()
  for ((i = 0; i < PROVISIONAL; i++)); do SEEDS+=("$i"); done
fi

if (( LOCAL_IN < PROVISIONAL )); then
  echo "warning: $CONTEST has $LOCAL_IN local inputs but $PROVISIONAL provisional tests" >&2
fi

shopt -s nullglob
SOL_CC=( "$DIR"/a_*.cc )
if (( ${#SOL_CC[@]} == 0 )); then
  echo "no a_*.cc in $DIR" >&2
  exit 1
fi
SOL="${SOL_CC[0]}"
BIN_NAME="$(basename "$SOL" .cc)"
BUILD="$DIR/build"
mkdir -p "$BUILD"

CXX=${CXX:-g++}
STD=${AHC_CXX_STD:-c++17}
"$CXX" -std="$STD" -O2 -I"$ROOT" -o "$BUILD/$BIN_NAME" "$SOL"

score_one() {
  local seed=$1
  local inp="$TOOLS/in/$(printf '%04d.txt' "$seed")"
  if [[ ! -f "$inp" ]]; then
    echo "seed $seed: MISSING $inp" >&2
    return 0
  fi

  if [[ -f "$TOOLS/verify.cc" ]]; then
    if [[ ! -x "$BUILD/verify" ]]; then
      "$CXX" -std="$STD" -O2 -I"$ROOT" -o "$BUILD/verify" "$TOOLS/verify.cc"
    fi
    local out
    out="$(mktemp)"
    "$BUILD/$BIN_NAME" < "$inp" > "$out"
    local line score
    line="$((cat "$inp"; cat "$out") | "$BUILD/verify")"
    score="${line##*score=}"
    rm -f "$out"
    echo "$score"
    return 0
  fi

  if [[ -f "$TOOLS/src/bin/tester.rs" ]]; then
    local log score
    if grep -q '<input> <command>' "$TOOLS/src/bin/tester.rs" 2>/dev/null; then
      log="$(cd "$TOOLS" && cargo run -q -r --bin tester "$inp" "$BUILD/$BIN_NAME" 2>&1)" || true
    else
      log="$(cd "$TOOLS" && cargo run -q -r --bin tester "$BUILD/$BIN_NAME" 2>&1 < "$inp")" || true
    fi
    score="$(printf '%s\n' "$log" | awk '/Score =/{print $3; exit}')"
    if [[ -z "$score" ]]; then
      score="$(printf '%s\n' "$log" | grep -E '^[0-9]+$' | tail -1)"
    fi
    if [[ -z "$score" ]]; then
      score="$(printf '%s\n' "$log" | tail -1)"
    fi
    echo "$score"
    return 0
  fi

  local out
  out="$(mktemp)"
  "$BUILD/$BIN_NAME" < "$inp" > "$out"
  local log score
  log="$(cd "$TOOLS" && cargo run -q -r --bin vis "$inp" "$out" 2>&1)" || true
  score="$(printf '%s\n' "$log" | awk '/Score =/{print $3; exit}')"
  if [[ -z "$score" ]]; then
    score="$(printf '%s\n' "$log" | tail -1)"
  fi
  rm -f "$out"
  echo "$score"
}

total=0
count=0
min=999999999999999
max=0
for seed in "${SEEDS[@]}"; do
  sc="$(score_one "$seed")"
  if [[ "$sc" =~ ^[0-9]+$ ]]; then
    total=$((total + sc))
    count=$((count + 1))
    if (( sc < min )); then min=$sc; fi
    if (( sc > max )); then max=$sc; fi
  fi
  echo "seed $seed: $sc"
done

echo "contest=$CONTEST provisional=$PROVISIONAL cases=$count total=$total avg=$(( count > 0 ? total / count : 0 )) min=$min max=$max"
