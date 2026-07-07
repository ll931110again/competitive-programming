# AHC012 — AtCoder 10th Anniversary

[Problem](https://atcoder.jp/contests/ahc012/tasks/ahc012_a) · [Contest](https://atcoder.jp/contests/ahc012)

Cut a circular cake (radius 10 000) with at most 100 straight lines. Each attendee with AtCoder experience `d` years wants a piece containing exactly `d` strawberries. Score is `1e6 * satisfied / total_attendees`.

## Solution

`a_atcoder_10th_anniversary.cc` — horizontal/vertical grid cuts with simulated annealing. Lines are slightly skewed (`x`→`x+1`, `y`→`y+1`) so no strawberry center lies on a cut. Early SA uses a weighted objective that favors larger `d`, then converges to the official score.

## Build

```bash
mkdir -p build
g++ -std=c++20 -O2 -Wall -Wextra -I../.. -o build/a_atcoder_10th_anniversary a_atcoder_10th_anniversary.cc
```

## Run one case

```bash
./build/a_atcoder_10th_anniversary < tools/in/0000.txt > /tmp/out.txt
cd tools && cargo run --release --bin vis in/0000.txt /tmp/out.txt
```

## Test all seeds (0–99)

```bash
./test.sh
```

## Tools

Official local tools are under `tools/` (Rust). See `tools/README.md` for `gen` and `vis`.

Download or refresh tools from repo root:

```bash
python3 scripts/download_ahc_tools.py 12
```
