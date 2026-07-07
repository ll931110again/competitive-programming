# AHC003 — Shortest Path Queries

[Problem A](https://atcoder.jp/contests/ahc003/tasks/ahc003_a) · [Contest](https://atcoder.jp/contests/ahc003)

Interactive 30×30 grid routing: edge lengths are hidden. For each of 1000 queries, read `(si, sj, ti, tj)`, output a simple `ULDR` path, then read noisy measured travel time `round(length × e)` with `e ∈ [0.9, 1.1]`. Score rewards paths close to the true shortest path; later queries are weighted more heavily.

## Solution

`a_shortest_path_queries.cc` — Dijkstra on learned weights:

1. Start every edge at weight 5000.
2. After each query, scale weights on traversed edges by `measured / estimated_path_length`.
3. Refit each row and column as a 1- or 2-segment piecewise-constant (matches the generator’s `M ∈ {1, 2}` structure).

## Build

```bash
mkdir -p build
g++ -std=c++17 -O2 -I../../ -o build/a_shortest_path_queries a_shortest_path_queries.cc
```

## Official tools

```bash
# download (from repo root)
python3 scripts/download_ahc_tools.py 3

# generate inputs (100 seeds in tools/seeds.txt)
cd tools && cargo run --release --bin gen seeds.txt

# single case
cd tools
cargo run --release --bin tester in/0000.txt ../build/a_shortest_path_queries

# all 100 seeds
./test.sh
```

The tester pipes only `(si,sj,ti,tj)` and feedback per query (no edge lengths on stdin), matching the online judge.

## Local scores (100 provisional tests)

Run: `./test.sh`

| Metric | Value |
|--------|------:|
| Cases | 100 |
| Total | 92,145,324,533 |
| Average | 921,453,245 |
| Min | 858,112,632 |
| Max | 973,454,540 |

Example (seed 0): **939,463,178**

## Files

| File | Purpose |
|------|---------|
| `a_shortest_path_queries.cc` | Solution |
| `test.sh` | Batch runner over `tools/in/*.txt` |
| `tools/` | Official Rust gen / tester / vis |
