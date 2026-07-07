# AHC002 — Walking on Tiles

Solution: [`a_walking_on_tiles.cc`](a_walking_on_tiles.cc)  
Problem: [AHC002 A](https://atcoder.jp/contests/ahc002/tasks/ahc002_a)

Time-limited DFS initialization, then **simulated annealing** with splice-and-reconnect
neighborhood ([kmyk AHC002](https://github.com/kmyk/atcoder-heuristic-contest-002)).

## Build

```bash
./test.sh
```

Or manually:

```bash
g++ -std=c++20 -O2 -I../.. -o build/a_walking_on_tiles a_walking_on_tiles.cc
g++ -std=c++20 -O2 -I../.. -o build/verify tools/verify.cc
```

Official Rust tools live in [`tools/`](tools/) (`tools/README.md`). Local scoring: [`tools/verify.cc`](tools/verify.cc).

## Scores (100 provisional tests)

Run: `./test.sh` (all 100) or `./test.sh 0 1 2` for a subset.

| | Total | Avg |
|--|--:|--:|
| Current (DFS init + splice SA) | 6,180,142 | 61,801 |
| Previous beam-500 (seeds 0–4 only) | 135,587 | ~27,117 |

## Sample seeds 0–4

| Seed | Score | Steps |
|------|------:|------:|
| 0 | 62,484 | 1154 |
| 1 | 62,363 | 1142 |
| 2 | 61,177 | 1139 |
| 3 | 62,165 | 1149 |
| 4 | 61,186 | 1136 |
| **Total** | **309,375** | |

Official visualizer:

```bash
./build/a_walking_on_tiles < tools/in/0000.txt > /tmp/out.txt
(cd tools && cargo run --release --bin vis in/0000.txt /tmp/out.txt)
```
