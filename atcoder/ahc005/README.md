# AHC005 — Patrolling

[Problem A](https://atcoder.jp/contests/ahc005/tasks/ahc005_a): from a start road cell, find a cycle of `U`/`D`/`L`/`R` moves that returns home and puts every road cell in line-of-sight at least once. Minimize total move time `t`; full coverage (`v = r`) scores `1e6 + round(1e6 * log2(r/t))`.

## Files

| Path | Role |
|------|------|
| `a_patrolling.cc` | C++ solution |
| `tools/in/` | Official seed inputs (100 cases) |
| `tools/verify.cc` | Local C++ scorer |
| `test.sh` | Build + score all seeds |

## Build

```bash
g++ -std=c++20 -O2 -pipe -I../.. -o build/a_patrolling a_patrolling.cc
g++ -std=c++20 -O2 -pipe -I../.. -o build/verify tools/verify.cc
mkdir -p build
```

## Run one case

```bash
./build/a_patrolling < tools/in/0000.txt > /tmp/out.txt
(cat tools/in/0000.txt /tmp/out.txt) | ./build/verify
```

## Score all seeds

```bash
chmod +x test.sh
./test.sh
```

## Strategy

1. Contract the road grid to nodes (intersections, dead ends, corners, start).
2. Enumerate maximal straight corridors between nodes; visiting each once covers all roads.
3. All-pairs shortest paths between nodes (Dijkstra).
4. Greedy corridor tour + 2-opt hill climb + simulated annealing (2-opt reversals).
5. Emit shortest paths between consecutive corridor endpoints and return to start.
