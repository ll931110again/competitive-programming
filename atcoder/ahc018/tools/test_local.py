#!/usr/bin/env python3
"""Local interactive tester for AHC018 (mirrors tools/src/bin/tester.rs)."""

from __future__ import annotations

import argparse
import re
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path


@dataclass
class Input:
    n: int
    w: int
    k: int
    c: int
    h: list[list[int]]
    sources: list[tuple[int, int]]
    sinks: list[tuple[int, int]]


class UnionFind:
    def __init__(self, n: int) -> None:
        self.par = list(range(n))
        self.sz = [1] * n

    def find(self, x: int) -> int:
        while self.par[x] != x:
            self.par[x] = self.par[self.par[x]]
            x = self.par[x]
        return x

    def unite(self, a: int, b: int) -> None:
        a, b = self.find(a), self.find(b)
        if a == b:
            return
        if self.sz[a] < self.sz[b]:
            a, b = b, a
        self.par[b] = a
        self.sz[a] += self.sz[b]

    def same(self, a: int, b: int) -> bool:
        return self.find(a) == self.find(b)


class Sim:
    def __init__(self, inp: Input) -> None:
        self.n = inp.n
        self.c = inp.c
        self.sources = inp.sources
        self.sinks = inp.sinks
        self.h = [row[:] for row in inp.h]
        self.total = 0
        self.uf = UnionFind(inp.n * inp.n)

    def dig(self, r: int, c: int, p: int) -> int:
        if not (0 <= r < self.n and 0 <= c < self.n and 1 <= p <= 5000):
            raise ValueError(f"invalid operation: ({r}, {c}; {p})")
        if self.h[r][c] == 0:
            raise ValueError(f"invalid operation: ({r}, {c}) already destructed")
        self.h[r][c] = max(0, self.h[r][c] - p)
        self.total += self.c + p
        if self.h[r][c] == 0:
            for dr, dc in ((0, -1), (-1, 0), (0, 1), (1, 0)):
                nr, nc = r + dr, c + dc
                if 0 <= nr < self.n and 0 <= nc < self.n and self.h[nr][nc] == 0:
                    self.uf.unite(r * self.n + c, nr * self.n + nc)
        ok = all(
            any(self.uf.same(a * self.n + b, s * self.n + t) for s, t in self.sources)
            for a, b in self.sinks
        )
        if ok:
            return 2
        return 1 if self.h[r][c] == 0 else 0


def parse_input(path: Path) -> Input:
    it = iter(path.read_text().split())
    n = int(next(it))
    w = int(next(it))
    k = int(next(it))
    c = int(next(it))
    h = [[int(next(it)) for _ in range(n)] for _ in range(n)]
    sources = [(int(next(it)), int(next(it))) for _ in range(w)]
    sinks = [(int(next(it)), int(next(it))) for _ in range(k)]
    return Input(n, w, k, c, h, sources, sinks)


def run(cmd: list[str], inp: Input) -> int:
    proc = subprocess.Popen(
        cmd,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE,
        text=True,
        bufsize=1,
    )
    assert proc.stdin and proc.stdout

    def send(line: str) -> None:
        proc.stdin.write(line + "\n")
        proc.stdin.flush()

    send(f"{inp.n} {inp.w} {inp.k} {inp.c}")
    for r, c in inp.sources:
        send(f"{r} {c}")
    for r, c in inp.sinks:
        send(f"{r} {c}")

    sim = Sim(inp)
    while True:
        line = proc.stdout.readline()
        if not line:
            raise RuntimeError("program terminated unexpectedly")
        sys.stdout.write(line)
        sys.stdout.flush()
        s = line.strip()
        if not s or s.startswith("#"):
            continue
        parts = s.split()
        if len(parts) != 3:
            send("-1")
            raise ValueError(f"invalid output: {line!r}")
        r, c, p = map(int, parts)
        try:
            rv = sim.dig(r, c, p)
        except ValueError as e:
            send("-1")
            raise e
        send(str(rv))
        if rv == 2:
            break

    proc.wait(timeout=1)
    return sim.total


def main() -> int:
    ap = argparse.ArgumentParser(description=__doc__)
    ap.add_argument("input")
    ap.add_argument("command", nargs=argparse.REMAINDER)
    args = ap.parse_args()
    if not args.command:
        ap.error("missing command")
    inp = parse_input(Path(args.input))
    try:
        cost = run(args.command, inp)
    except Exception as e:
        print(e, file=sys.stderr)
        return 1
    print(f"Total Cost = {cost}", file=sys.stderr)
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
