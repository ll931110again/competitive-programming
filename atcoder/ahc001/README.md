# AHC001 — AtCoder Ad

Problem: [AHC001 A](https://atcoder.jp/contests/ahc001/tasks/ahc001_a)

## Solution

`a_atcoder_ad.cc` — area-balanced binary space partition (BSP) over company points. Each recursive split balances target areas between halves and picks a cut line that keeps every desired point on the correct side. Leaf cells become output rectangles, tiling the board without overlap.

## Build

```bash
g++-15 -std=c++17 -O2 -o atcoder/ahc001/a_atcoder_ad atcoder/ahc001/a_atcoder_ad.cc
```

On Linux or with GCC installed as `g++`:

```bash
g++ -std=c++17 -O2 -o atcoder/ahc001/a_atcoder_ad atcoder/ahc001/a_atcoder_ad.cc
```

## Scores (50 provisional tests)

Run all: `./test.sh` or `../../scripts/ahc_calibrate.sh ahc001`

| | Total | Avg |
|--|--:|--:|
| Current (32 BSP variants) | 35,662,446,811 | 713,248,936 |

