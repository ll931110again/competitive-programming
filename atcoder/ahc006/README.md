# AHC006 — Food Delivery

[Problem A](https://atcoder.jp/contests/ahc006/tasks/ahc006_a): pick 50 of 1000 pickup–delivery orders and plan a route from the AtCoder office `(400, 400)` that visits each restaurant before its destination. Minimize total Manhattan travel time `T`; score is `round(1e8 / (1000 + T))` per test case (100 cases).

## Files

| Path | Role |
|------|------|
| `a_food_delivery.cc` | C++ solution |
| `tools/` | Official Rust generator / visualizer (downloaded) |
| `tools/verify.cc` | Local C++ scorer (same rules as `tools/src/lib.rs`) |
| `test.sh` | Build + score all `tools/in/*.txt` |

## Build

```bash
g++ -std=c++17 -O2 -I../.. -o build/a_food_delivery a_food_delivery.cc
g++ -std=c++17 -O2 -I../.. -o build/verify tools/verify.cc
mkdir -p build
```

## Run one case

```bash
./build/a_food_delivery < tools/in/0000.txt > /tmp/out.txt
(cat tools/in/0000.txt /tmp/out.txt) | ./build/verify
# or official vis:
(cd tools && cargo run --release --bin vis in/0000.txt /tmp/out.txt)
```

## Score all seeds

```bash
./test.sh
```

## Strategy

1. Rank orders by standalone round-trip cost `office → pickup → drop → office` and take the 50 cheapest.
2. Build a pickup-before-delivery visit order with greedy nearest-neighbor, then improve with local search (swap / reverse / relocate) while keeping precedence.
3. Try random single-order swaps against the remaining pool and keep the best set.
