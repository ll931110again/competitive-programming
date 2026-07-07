# AHC005 local tools

Official contest bundle provides seed inputs under `in/` (0000.txt–0099.txt).
There is no bundled Rust visualizer for this contest; use `verify.cc` to score outputs.

## Build

From `atcoder/ahc005/`:

```
g++ -std=c++20 -O2 -I../.. -o build/a_patrolling a_patrolling.cc
g++ -std=c++20 -O2 -I../.. -o build/verify tools/verify.cc
mkdir -p build
```

## Run one seed

```
./build/a_patrolling < tools/in/0000.txt > /tmp/out.txt
(cat tools/in/0000.txt && cat /tmp/out.txt) | ./build/verify
```

## Batch

```
./test.sh
```
