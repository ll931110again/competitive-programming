# AHC020 — Broadcasting

[Problem A](https://atcoder.jp/contests/ahc020/tasks/ahc020_a) · [Contest](https://atcoder.jp/contests/ahc020)

Connect broadcast stations with cables (MST) and set transmission power so every resident is covered, minimizing cable weight plus sum of P_i².

## Files

| Path | Description |
|------|-------------|
| `a_broadcasting.cc` | C++ solution (MST + greedy power + hill climbing) |
| `tools/` | Official AtCoder input generator and visualizer (Rust) |

## Build

```bash
g++ -std=c++20 -O2 -o a_broadcasting a_broadcasting.cc
cd tools && cargo build --release
```

## Run one case

```bash
./a_broadcasting < tools/in/0000.txt > /tmp/out.txt
cd tools && cargo run --release --bin vis in/0000.txt /tmp/out.txt
```

The visualizer prints the score and writes `vis.html`.

## Score all seeds 0–99

```bash
cd /Users/linh/code/competitive-programming/atcoder/ahc020
total=0
for i in $(seq -w 0 99); do
  ./a_broadcasting < tools/in/${i}.txt > /tmp/out.txt
  score=$(cargo run --release --manifest-path tools/Cargo.toml --bin vis tools/in/${i}.txt /tmp/out.txt 2>/dev/null | tail -1)
  echo "seed $i: $score"
  total=$((total + score))
done
echo "total: $total"
```

## Approach

1. **Power (`kgreedy`)** — For each uncovered resident, pick the cheapest station (among 10 nearest) to extend power; use a priority queue ordered by marginal cost.
2. **Cables (`mst`)** — Minimum spanning tree over stations that need nonzero power (always including station 1).
3. **Search** — Hill climbing: randomly reduce one station’s power and re-run greedy; prune/add stations by toggling MST membership when edge cost does not increase.

Score (all K residents covered): `round(1e6 * (1 + 1e8 / (cost + 1e7)))`.
