# AHC065 — Conveyor Design

[Problem A](https://atcoder.jp/contests/ahc065/tasks/ahc065_a): place length-2 conveyor belts (at most two per cell) on an `N×N` grid, then rotate belts to export boxes `0,1,…` in order from the exit `(0, N/2)` within `10^5` turns. Score is `round(10^6 · B / N²)` when not all boxes are exported; a full export adds a turn-count bonus.

## Files

| Path | Role |
|------|------|
| `a_conveyor_design.cc` | C++ solution |
| `tools/` | Official Rust generator / visualizer (downloaded) |
| `tools/verify.cc` | Local C++ scorer (same rules as `tools/src/lib.rs`) |
| `tools/run_local.sh` | Build + score seeds `FIRST..LAST` (default `0..99`) |

## Build

```bash
g++ -std=c++17 -O2 -I../.. -o build/a_conveyor_design a_conveyor_design.cc
g++ -std=c++17 -O2 -I../.. -o build/verify tools/verify.cc
mkdir -p build
```

## Run one case

```bash
./build/a_conveyor_design < tools/in/0000.txt > /tmp/out.txt
(cat tools/in/0000.txt /tmp/out.txt) | ./build/verify
# or official vis (writes vis.html):
(cd tools && cargo run --release --bin vis in/0000.txt /tmp/out.txt)
```

## Score all seeds

```bash
chmod +x tools/run_local.sh
./tools/run_local.sh
FIRST=0 LAST=4 ./tools/run_local.sh   # quick smoke test
```

## Local scores (seeds 0–99)

| Metric | Value |
|--------|------:|
| Total | 86,975,000 |
| Average | 869,750 |
| Min | 850,000 |
| Max | 892,500 |

Per-seed sample (0–4): 877500, 865000, 867500, 860000, 870000.

## Strategy

1. Build a Hamiltonian path ending at the exit with Warnsdorff DFS (256 random tie-break trials; pick the layout minimizing initial box distance sum).
2. Place one length-2 belt on each path edge (≤2 belts per cell on the path).
3. Repeatedly BFS on the belt graph from the current box to the exit, rotate along the shortest belt path, and export when the correct box reaches the exit.
