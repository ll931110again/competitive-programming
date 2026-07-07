# AHC008 — Territory

[Contest](https://atcoder.jp/contests/ahc008) · [Task A](https://atcoder.jp/contests/ahc008/tasks/ahc008_a)

On a 30×30 grid with `N` pets and `M` humans, play 300 interactive turns. Each human may stay (`.`), place a wall on an adjacent cell (`u`/`d`/`l`/`r`), or move (`U`/`D`/`L`/`R`). Score is `round(10^8 × (1/M) Σ |R_i|/900 × 2^{-n_i})` where `R_i` is the passable region reachable from human `i` and `n_i` is pets inside it.

## Files

| Path | Role |
|------|------|
| `a_territory.cc` | C++ solution |
| `tools/` | Official Rust generator / tester / visualizer |
| `test.sh` | Build + score all `tools/in/*.txt` |

## Build

```bash
g++ -std=c++17 -O2 -I../.. -o build/a_territory a_territory.cc
mkdir -p build
```

## Run one case

```bash
./build/a_territory < tools/in/0000.txt   # interactive — use tester instead
cd tools && cargo run --release --bin tester ../build/a_territory < in/0000.txt > /tmp/out.txt
```

## Score all seeds

```bash
./test.sh
```

## Approach

1. Assign each human a horizontal band of rows and precompute perimeter wall targets.
2. Each turn, evaluate all legal actions per human on a copy of the grid.
3. Pick the action that maximizes estimated team score (BFS reachable area with pet penalty `2^{-n}`), with a small bonus for placing band perimeter walls.
