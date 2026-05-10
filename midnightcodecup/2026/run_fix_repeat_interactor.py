#!/usr/bin/env python3
"""
Local interactor for Midnight Code Cup 2026 — C. Run, Fix, Repeat.

Usage:
  ./generator seed | python3 run_fix_repeat_interactor.py ./your_solution
  python3 run_fix_repeat_interactor.py ./sol --grid-file path   # first line "N T", then N lines

Environment (optional):
  RFC_MAX_QUERIES   Cap interaction length (export before pipe: gen only sees env if placed left).
  RFC_VERBOSE=1     Log first ~40 exchanges to stderr.
  RFC_FORCE_TARGET=r,c   Fix repair point to 1-based cell (sanity check).

The solution must read N T only (same as contest). The grid is hidden.
"""
from __future__ import annotations

import collections
import os
import random
import subprocess
import sys


def load_grid_from_stdin() -> tuple[int, int, list[list[str]]]:
    line = sys.stdin.readline()
    if not line:
        raise SystemExit("no input")
    N, T = map(int, line.split())
    rows = []
    for _ in range(N):
        rows.append(list(sys.stdin.readline().rstrip("\n")))
    if len(rows) != N or any(len(row) != N for row in rows):
        raise SystemExit("bad grid")
    return N, T, rows


def load_grid_file(path: str) -> tuple[int, int, list[list[str]]]:
    with open(path) as f:
        lines = f.read().splitlines()
    N, T = map(int, lines[0].split())
    rows = [list(lines[i + 1]) for i in range(N)]
    return N, T, rows


def bfs_dist(grid: list[list[str]], N: int, sr: int, sc: int, tr: int, tc: int) -> int:
    """0-based indices; passable == '.'"""
    if grid[sr][sc] != "." or grid[tr][tc] != ".":
        return 10**9
    if sr == tr and sc == tc:
        return 0
    dist = [[-1] * N for _ in range(N)]
    q = collections.deque([(sr, sc)])
    dist[sr][sc] = 0
    while q:
        r, c = q.popleft()
        d0 = dist[r][c]
        for dr, dc in ((1, 0), (-1, 0), (0, 1), (0, -1)):
            nr, nc = r + dr, c + dc
            if 0 <= nr < N and 0 <= nc < N and grid[nr][nc] == "." and dist[nr][nc] < 0:
                dist[nr][nc] = d0 + 1
                if nr == tr and nc == tc:
                    return d0 + 1
                q.append((nr, nc))
    return 10**9


def collect_empty(N: int, grid: list[list[str]]) -> list[tuple[int, int]]:
    out = []
    for r in range(N):
        for c in range(N):
            if grid[r][c] == ".":
                out.append((r, c))
    return out


def main() -> None:
    argv = sys.argv[1:]
    grid_file = None
    if "--grid-file" in argv:
        i = argv.index("--grid-file")
        grid_file = argv[i + 1]
        argv = argv[:i] + argv[i + 2 :]
    if len(argv) != 1:
        print(
            "usage: run_fix_repeat_interactor.py [--grid-file f] ./solution",
            file=sys.stderr,
        )
        raise SystemExit(2)

    sol_path = argv[0]
    if grid_file:
        N, T, grid = load_grid_file(grid_file)
    else:
        N, T, grid = load_grid_from_stdin()

    empty = collect_empty(N, grid)
    rng = random.Random(12345)
    forced = os.environ.get("RFC_FORCE_TARGET")
    if forced:
        pr, pc = map(int, forced.replace(",", " ").split())
        tr, tc = pr - 1, pc - 1
        assert grid[tr][tc] == ".", "RFC_FORCE_TARGET must be an empty cell (1-based)"
    else:
        tr, tc = rng.choice(empty)

    _tlim = os.environ.get("RFC_MAX_QUERIES")
    if _tlim:
        T = min(T, int(_tlim))

    env = os.environ.copy()
    proc = subprocess.Popen(
        [sol_path],
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.DEVNULL,
        text=True,
        bufsize=0,
        env=env,
    )
    assert proc.stdin and proc.stdout

    proc.stdin.write(f"{N} {T}\n")
    proc.stdin.flush()

    hits = 0
    queries = 0
    verbose = os.environ.get("RFC_VERBOSE") == "1"

    while queries < T:
        line = proc.stdout.readline()
        if not line:
            break
        parts = line.split()
        if len(parts) != 2:
            print("protocol: expected two ints", line, file=sys.stderr)
            break
        r, c = int(parts[0]), int(parts[1])
        if r == -1 and c == -1:
            break
        if not (1 <= r <= N and 1 <= c <= N):
            print("out of range", r, c, file=sys.stderr)
            break
        queries += 1
        gr, gc = r - 1, c - 1
        if grid[gr][gc] != ".":
            if verbose and queries <= 40:
                print(f"q{queries}: ({r},{c}) -> -1 wall", file=sys.stderr)
            proc.stdin.write("-1\n")
            proc.stdin.flush()
            continue
        if gr == tr and gc == tc:
            hits += 1
            if verbose and queries <= 40:
                print(f"q{queries}: ({r},{c}) -> 0 HIT total_hits={hits}", file=sys.stderr)
            tr, tc = rng.choice(empty)
            proc.stdin.write("0\n")
            proc.stdin.flush()
            continue
        d = bfs_dist(grid, N, gr, gc, tr, tc)
        if verbose and queries <= 40:
            print(f"q{queries}: ({r},{c}) -> dist {d}", file=sys.stderr)
        proc.stdin.write(f"{d}\n")
        proc.stdin.flush()

    proc.stdin.close()
    proc.wait(timeout=5)
    print(f"# local_done queries={queries} hits={hits}", file=sys.stderr)


if __name__ == "__main__":
    main()
