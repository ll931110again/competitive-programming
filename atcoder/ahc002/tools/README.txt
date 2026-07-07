# Usage

See `README.md` in this directory for the official Rust visualizer and input generator.

## Local C++ verifier

From `atcoder/ahc002/`:

```
g++ -std=c++20 -O2 -I../.. -o build/verify tools/verify.cc
./build/a_walking_on_tiles < tools/in/0000.txt > /tmp/out.txt
(cat tools/in/0000.txt && cat /tmp/out.txt) | ./build/verify
```

Or run `./test.sh 0 1 2 3 4`.
