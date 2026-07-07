# AHC018 — Excavation

[Problem A](https://atcoder.jp/contests/ahc018/tasks/ahc018_a) · [Contest](https://atcoder.jp/contests/ahc018)

Interactive grid excavation: break bedrock so water from sources reaches every house. Each dig at `(y,x)` with power `P` costs `C + P` stamina and reduces sturdiness by `P`. Bedrock hardness is hidden during submission.

## Solution

`a_excavation.cc` — diamond-grid routing graph, Dijkstra to pick probe targets, then sequential rectilinear digging with hardness bounds (`dig_lo` / `dig_hi`) and C-dependent power tuning (based on [Psyho's AHC018 approach](https://github.com/FakePsyho/cpcontests/blob/master/atcoder/ahc018/approach.md)).

## Tools

Official Rust tools live in `tools/` (downloaded via `python3 scripts/download_ahc_tools.py 18`).

```bash
cd atcoder/ahc018/tools
cargo build --release
```

### Test one seed

```bash
# repo root on -I for bits/stdc++.h
g++ -std=c++20 -O2 -I../../.. -o ../a_excavation ../a_excavation.cc
cargo run --release --bin tester in/0000.txt ../a_excavation
# stderr: Total Cost = ...
```

If `cargo run --bin tester` cannot spawn the binary (some sandboxes), use the Python mirror:

```bash
python3 test_local.py in/0000.txt ../a_excavation
```

### Test all 100 provisional seeds

```bash
g++ -std=c++20 -O2 -I../../.. -o ../a_excavation ../a_excavation.cc
total=0
for f in in/*.txt; do
  cost=$(python3 test_local.py "$f" ../a_excavation 2>&1 | awk '/Total Cost/ {print $4}')
  total=$((total + cost))
done
echo "sum=$total"   # reference run: 9790856
```

### Visualize

```bash
cargo run --release --bin tester in/0000.txt /tmp/a_excavation > /tmp/out.txt
cargo run --release --bin vis in/0000.txt /tmp/out.txt
open out.svg   # or vis.html
```

Web visualizer: https://img.atcoder.jp/ahc018/6bada50282.html
