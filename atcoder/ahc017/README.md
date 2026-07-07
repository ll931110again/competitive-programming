# AHC017 — Road Repair

[Problem A](https://atcoder.jp/contests/ahc017/tasks/ahc017_a) · [Contest](https://atcoder.jp/contests/ahc017)

Schedule `M` road repairs across `D` days (at most `K` per day) on a planar road network. Minimize average daily increase in all-pairs shortest-path distance (frustration; lower is better).

## Layout

| Path | Description |
|------|-------------|
| `a_road_repair.cc` | C++ solution (greedy + beam search + hill climbing) |
| `tools/` | Official Rust generator / visualizer / scorer |
| `tools/verify.cc` | Standalone C++ scorer (no Rust required) |
| `tools/in/` | Seeds 0–99 |

## Build

```bash
g++ -std=c++20 -O2 -pipe -I../../ -o a_road_repair a_road_repair.cc
g++ -std=c++20 -O2 -pipe -I../../ -o verify tools/verify.cc
```

## Run one case

```bash
./a_road_repair < tools/in/0000.txt > out.txt
./verify tools/in/0000.txt out.txt
# or (Rust scorer + HTML viz):
cd tools && cargo run -r --bin vis in/0000.txt ../out.txt
```

## Batch test (seeds 0–99)

```bash
cd atcoder/ahc017
sum=0
for i in $(seq -w 0 99); do
  ./a_road_repair < tools/in/${i}.txt > /tmp/out.txt
  s=$(./verify tools/in/${i}.txt /tmp/out.txt | awk -F= '{print $2}')
  sum=$((sum + s))
  echo "seed $i frustration=$s"
done
echo "total_frustration=$sum"
```

Relative AtCoder score per case is `round(1e9 * best / yours)`; local frustration sums are useful for regression only.

## Approach

1. **Sample sources** — spread `~16` vertices across the map for fast frustration estimates.
2. **Sort edges** — descending single-edge removal impact (dynamic bucket-Dijkstra).
3. **Greedy + beam** — assign each edge to the day with lowest estimated added frustration, using progressively larger source subsets; penalize sparse days to spread load.
4. **Hill climbing** — randomly move edges between days until the time limit (~5.85 s).

Based on the greedy + dynamic-Dijkstra pattern from [FakePsyho's writeup](https://github.com/FakePsyho/cpcontests/blob/master/atcoder/ahc017/approach.md).
