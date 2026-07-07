# AHC009 — Robust Memory of Commuting Routes

[Contest](https://atcoder.jp/contests/ahc009) · [Task A](https://atcoder.jp/contests/ahc009/tasks/ahc009_a)

Find a route string (≤200 of `U`/`D`/`L`/`R`) from home to office on a 20×20 grid with internal walls. Each turn, with probability `p` the move is forgotten (stay); otherwise move if no wall. Maximize expected score `round(250000 × E[S])` where `S = 401 − t` on arrival at turn `t`, else `0`.

## Files

| Path | Description |
|------|-------------|
| `a_robust_memory.cc` | Solution: random-weight Dijkstra + duplicate padding + SA |
| `tools/` | Official input generator and visualizer (Rust) |

## Build

```bash
g++ -std=c++20 -O2 -Wall -Wextra -I../.. -o build/a_robust_memory a_robust_memory.cc
```

Or run all 100 seeds:

```bash
./test.sh
```

## Run one case

```bash
./build/a_robust_memory < tools/in/0000.txt
```

## Score with visualizer

```bash
cd tools && cargo run --release --bin vis in/0000.txt ../out.txt
```

## Batch test (all 100 seeds)

See `test.sh` (same pattern as `atcoder/ahc012/test.sh`).

## Approach

1. **Base path** — Dijkstra from start to goal with random edge weights (1…W) for route diversity.
2. **Padding** — Insert duplicate characters until length 200 (robust to forgotten moves).
3. **Refinement** — Simulated annealing with copy/insert/delete/change/move neighbors; score via forward probability DP matching the official judge.
