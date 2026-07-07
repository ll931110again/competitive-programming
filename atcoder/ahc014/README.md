# AHC014 — RectJoin

[Problem A](https://atcoder.jp/contests/ahc014/tasks/ahc014_a): on an `N×N` grid with `M` initial dots, repeatedly place a new dot as the fourth corner of an axis-aligned or 45° rectangle whose other three corners already have dots, draw the perimeter (no edge overlap, no extra dots on the perimeter), and maximize

\[\mathrm{round}\left(10^6 \cdot \frac{N^2}{M} \cdot \frac{\sum_{(x,y)\in Q} w(x,y)}{\sum_{x,y} w(x,y)}\right)\]

where `w(x,y) = (x-c)² + (y-c)² + 1` and `c = (N-1)/2`.

## Files

| Path | Role |
|------|------|
| `a_rect_join.cc` | C++ solution |
| `tools/` | Official Rust generator / visualizer |
| `tools/verify.cc` | Local C++ scorer (mirrors `tools/src/lib.rs`) |
| `test.sh` | Build + score all `tools/in/*.txt` |

## Build

```bash
mkdir -p build
g++ -std=c++20 -O2 -Wall -Wextra -I../.. -o build/a_rect_join a_rect_join.cc
g++ -std=c++20 -O2 -I../.. -o build/verify tools/verify.cc
```

## Run one case

```bash
./build/a_rect_join < tools/in/0000.txt > build/out.txt
./build/verify tools/in/0000.txt build/out.txt
# official visualizer:
(cd tools && cargo run --release --bin vis in/0000.txt ../build/out.txt)
```

## Score all seeds

```bash
./test.sh
```

## Strategy

1. Enumerate legal moves from each existing dot: pick perpendicular ray directions, take the nearest dot on each ray, derive the new corner `p1 = p2 + p4 - p3`.
2. Greedy scoring: `weight(p1) × w_mul − area_penalty × rect_size` with light random tie-break.
3. After each placement, chain all immediate 1×1 axis-aligned and diagonal squares (“ok rects”).
4. Multi-start restarts with varied weights until ~4.6s; keep the best score.

## Tools

Download official inputs/tools:

```bash
python3 scripts/download_ahc_tools.py 14
```

Generate more inputs:

```bash
cd tools && cargo run --release --bin gen seeds.txt
```
