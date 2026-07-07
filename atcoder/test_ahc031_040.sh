#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
CXX="g++ -std=c++20 -O2 -Wall -Wextra -I$ROOT"

run_one() {
  local num="$1" src="$2"
  local dir="$ROOT/atcoder/ahc${num}"
  local bin="$dir/build/sol"
  mkdir -p "$dir/build"
  $CXX -o "$bin" "$dir/$src"
  cd "$dir/tools"
  for seed in 0 1 2 3 4; do
    local in="in/$(printf '%04d' "$seed").txt"
    local out="/tmp/ahc${num}_${seed}.txt"
    "$bin" < "$in" > "$out"
    local score
    score=$(cargo run -q --release --bin vis "$in" "$out" 2>&1 | tail -1)
    echo "ahc${num} seed${seed}: ${score}"
  done
}

run_one 031 a_event_hall.cc
run_one 032 a_mod_stamp.cc
run_one 033 a_container_handling.cc
run_one 034 a_excavation_plan.cc
run_one 035 a_crop_rotation.cc
run_one 036 a_roadside_advertisement.cc
run_one 037 a_sorting_machine.cc
run_one 038 a_takoyaki_robot.cc
run_one 039 a_christmas_tree.cc
run_one 040 a_packing.cc
