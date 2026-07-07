# AHC011 — Sliding Tree Puzzle

[Problem A](https://atcoder.jp/contests/ahc011/tasks/ahc011_a): slide tiles on an `N×N` board (`N=6..10`) to maximize the largest tree formed by tile connections. Full spanning tree of size `N²−1` scores `round(500000 × (2 − K/T))` with `T=2N³`; partial trees score by size fraction.

## Files

| Path | Role |
|------|------|
| `a_sliding_tree_puzzle.cc` | C++ solution |
| `tools/` | Official Rust generator / visualizer (downloaded) |
| `tools/verify.cc` | Local C++ scorer (same rules as `tools/src/lib.rs`) |
| `test.sh` | Build + score all `tools/in/*.txt` |

## Build

```bash
g++ -std=c++17 -O2 -I../.. -o build/a_sliding_tree_puzzle a_sliding_tree_puzzle.cc
g++ -std=c++17 -O2 -I../.. -o build/verify tools/verify.cc
mkdir -p build
```

## Run one case

```bash
./build/a_sliding_tree_puzzle < tools/in/0000.txt > /tmp/out.txt
(cat tools/in/0000.txt /tmp/out.txt) | ./build/verify
# or official vis:
(cd tools && cargo run --release --bin vis in/0000.txt /tmp/out.txt)
```

## Score all seeds

```bash
./test.sh
```

## Strategy

1. **Target layout** — sample random grid spanning trees (Kruskal); keep those whose tile-mask multiset matches the input. Assign tiles to cells with the same mask, ordered by position to minimize travel cost.
2. **Slide solver** — beam search toward the target layout using squared Euclidean distance as heuristic (width scales down for larger `N`).
3. **Fallback** — greedy moves that maximize current tree size if layout search or beam search underperforms.
