#!/usr/bin/env python3
"""
Simulated annealing for IOI 2010 Maze (output-only), following the Codeforces
methodology: random # <-> . flips, energy = max BFS distance (in cells) from
the unique perimeter '.', Boltzmann acceptance, geometric cooling.

IOI statement constraints (stricter than the original CF snippet):
  * Obstacles X stay fixed.
  * Exactly one '.' on the outer border (entrance); every other '.' lies in
    the strict interior (rows/cols not on the field edge).

Neighbour moves:
  * Interior corn cell: toggle '#' <-> '.'.
  * Perimeter corn cell: relocate the entrance (old entrance '#', chosen cell '.'),
    with the same corner case as the reference code when the sampled cell is the
    current entrance (pick another random side).

calc_energy: multi-source BFS from entrance — maximum shortest-path length in
cells (entrance counts as 1), matching ``grade_maze`` / ``eccentricity_energy``.
"""

from __future__ import annotations

import argparse
import math
import random
import sys
import time
from collections import deque
from pathlib import Path
from typing import List, Optional, Sequence, Tuple

Coord = Tuple[int, int]

DIRS = ((1, 0), (-1, 0), (0, 1), (0, -1))


def parse_grid(lines: List[str]) -> Tuple[List[str], int, int]:
    g = [ln.rstrip("\r\n") for ln in lines if ln.strip() != ""]
    n, m = len(g), len(g[0])
    return g, n, m


def is_perimeter(i: int, j: int, n: int, m: int) -> bool:
    return i == 0 or i == n - 1 or j == 0 or j == m - 1


def strict_interior(i: int, j: int, n: int, m: int) -> bool:
    return 1 <= i <= n - 2 and 1 <= j <= m - 2


def is_corner(i: int, j: int, n: int, m: int) -> bool:
    return (i == 0 and j == 0) or (i == 0 and j == m - 1) or (
        i == n - 1 and j == 0
    ) or (i == n - 1 and j == m - 1)


def build_sa_masks(
    g_in: Sequence[str], n: int, m: int
) -> Tuple[List[Coord], List[Coord], List[Coord]]:
    """
    CF blog (entry 118853): ``sides`` = perimeter corn excluding corners;
    ``all`` = every non-corner corn cell (interior + boundary).
    """
    sides: List[Coord] = []
    all_cells: List[Coord] = []
    for i in range(1, n - 1):
        for j in (0, m - 1):
            if g_in[i][j] == "#":
                sides.append((i, j))
    for j in range(1, m - 1):
        for i in (0, n - 1):
            if g_in[i][j] == "#":
                sides.append((i, j))
    for i in range(n):
        for j in range(m):
            if is_corner(i, j, n, m) or g_in[i][j] != "#":
                continue
            all_cells.append((i, j))
    interior = [c for c in all_cells if strict_interior(c[0], c[1], n, m)]
    return sides, interior, all_cells


def random_initial_state(
    g_in: List[str],
    n: int,
    m: int,
    sides: Sequence[Coord],
    interior: Sequence[Coord],
    rng: random.Random,
    interior_dot_prob: float = 0.7,
) -> Tuple[List[List[str]], Coord]:
    """Random entrance on perimeter + random interior '.' fraction (blog-style)."""
    if not sides:
        raise ValueError("no valid entrance (perimeter corn)")
    src = rng.choice(sides)
    g = [list(row) for row in g_in]
    for i, j in interior:
        if rng.random() < interior_dot_prob:
            g[i][j] = "."
        else:
            g[i][j] = "#"
    er, ec = src
    g[er][ec] = "."
    return g, src


def load_state_from_output(
    g_in: List[str],
    path: Path,
) -> Optional[Tuple[List[List[str]], Coord]]:
    """Load maze output; return None if invalid."""
    text = path.read_text(encoding="utf-8")
    lines = [ln.rstrip("\n") for ln in text.splitlines() if ln.strip() != ""]
    if len(lines) != len(g_in):
        return None
    n, m = len(g_in), len(g_in[0])
    g = [list(row) for row in lines]
    ent: Optional[Coord] = None
    for i in range(n):
        if len(g[i]) != m:
            return None
        for j in range(m):
            if g_in[i][j] == "X" and g[i][j] != "X":
                return None
            if g_in[i][j] == "#":
                if g[i][j] not in "#.":
                    return None
            elif g_in[i][j] == "X":
                if g[i][j] != "X":
                    return None
            if g[i][j] == "." and is_perimeter(i, j, n, m):
                if ent is not None:
                    return None
                ent = (i, j)
    if ent is None:
        return None
    return g, ent


def calc_energy(g: List[List[str]], n: int, m: int, src: Coord) -> int:
    """Max BFS distance from entrance (cells), or 0 if entrance missing."""
    si, sj = src
    if g[si][sj] != ".":
        return 0
    dist = [[-1] * m for _ in range(n)]
    dist[si][sj] = 1
    q: deque[Coord] = deque([(si, sj)])
    best = 1
    while q:
        i, j = q.popleft()
        d0 = dist[i][j]
        best = max(best, d0)
        for di, dj in DIRS:
            ni, nj = i + di, j + dj
            if not (0 <= ni < n and 0 <= nj < m):
                continue
            if g[ni][nj] != ".":
                continue
            if dist[ni][nj] != -1:
                continue
            dist[ni][nj] = d0 + 1
            q.append((ni, nj))
    return best


def grid_to_lines(g: List[List[str]]) -> List[str]:
    return ["".join(row) for row in g]


def run_sa(
    g_in: List[str],
    rng: random.Random,
    t0: float,
    alpha: float,
    max_seconds: Optional[float],
    max_iters: Optional[int],
    save_every: int,
    out_path: Optional[Path],
    warm_path: Optional[Path],
    log_every: int,
) -> Tuple[List[str], int]:
    n, m = len(g_in), len(g_in[0])
    sides, interior, all_cells = build_sa_masks(g_in, n, m)
    if not sides:
        raise SystemExit("No perimeter corn — cannot place entrance.")

    flat_pick: List[Coord] = all_cells

    if warm_path and warm_path.is_file():
        loaded = load_state_from_output(g_in, warm_path)
        if loaded:
            g, src = loaded
            print(f"Warm start from {warm_path}", file=sys.stderr)
        else:
            g, src = random_initial_state(g_in, n, m, sides, interior, rng)
            print("Warm start invalid; using random initial state", file=sys.stderr)
    else:
        g, src = random_initial_state(g_in, n, m, sides, interior, rng)

    src_ref: List[Coord] = [src]
    cur_e = calc_energy(g, n, m, src_ref[0])
    best_e = cur_e
    best_grid = [row[:] for row in g]
    best_src = src_ref[0]

    t = t0
    it = 0
    t_start = time.monotonic()

    while True:
        if max_iters is not None and it >= max_iters:
            break
        if max_seconds is not None and (time.monotonic() - t_start) >= max_seconds:
            break
        it += 1
        old_src = src_ref[0]
        i, j = rng.choice(flat_pick)

        if not is_perimeter(i, j, n, m):
            old_ch = g[i][j]
            g[i][j] = "#" if old_ch == "." else "."
        else:
            g[old_src[0]][old_src[1]] = "#"
            if old_src == (i, j):
                src_ref[0] = rng.choice(sides)
            else:
                src_ref[0] = (i, j)
            g[src_ref[0][0]][src_ref[0][1]] = "."

        new_e = calc_energy(g, n, m, src_ref[0])

        if new_e >= best_e:
            best_e = new_e
            best_grid = [row[:] for row in g]
            best_src = src_ref[0]

        accept = new_e >= cur_e or (
            t > 1e-300 and rng.random() < math.exp((new_e - cur_e) / t)
        )

        if accept:
            cur_e = new_e
        else:
            if not is_perimeter(i, j, n, m):
                g[i][j] = old_ch
            else:
                g[src_ref[0][0]][src_ref[0][1]] = "#"
                src_ref[0] = old_src
                g[old_src[0]][old_src[1]] = "."

        t *= alpha

        if log_every > 0 and it % log_every == 0:
            elapsed = time.monotonic() - t_start
            print(
                f"iter={it}  t={t:.6g}  curE={cur_e}  bestE={best_e}  elapsed={elapsed:.1f}s",
                file=sys.stderr,
            )

        if save_every > 0 and out_path and it % save_every == 0:
            _write_output(out_path, best_grid, g_in, n, m, best_src)

    # final write uses best maze
    lines = _finalize_lines(best_grid, g_in, n, m, best_src)
    if out_path:
        out_path.write_text("\n".join(lines) + "\n", encoding="utf-8")
    return lines, best_e


def _write_output(
    out: Path,
    g: List[List[str]],
    g_in: List[str],
    n: int,
    m: int,
    src: Coord,
) -> None:
    lines = _finalize_lines(g, g_in, n, m, src)
    out.write_text("\n".join(lines) + "\n", encoding="utf-8")


def _finalize_lines(
    g: List[List[str]],
    g_in: List[str],
    n: int,
    m: int,
    src: Coord,
) -> List[str]:
    """Ensure X fixed, corn only #/., single entrance."""
    rows = [list(row) for row in g]
    for i in range(n):
        for j in range(m):
            if g_in[i][j] == "X":
                rows[i][j] = "X"
            elif g_in[i][j] == "#":
                if rows[i][j] not in "#.":
                    rows[i][j] = "#"
    er, ec = src
    for i in range(n):
        for j in range(m):
            if g_in[i][j] != "#":
                continue
            if is_perimeter(i, j, n, m) and (i, j) != (er, ec):
                rows[i][j] = "#"
    rows[er][ec] = "."
    return ["".join(r) for r in rows]


def main() -> None:
    ap = argparse.ArgumentParser(
        description="IOI 2010 Maze — simulated annealing (Boltzmann + geometric cooling).",
        epilog=(
            "Example long run:  python maze_sa.py g_inputs/tests/02 -o 02.out "
            "--alpha 0.999999999 --max-seconds 864000 --warm maze_outputs/02.out "
            "--save-every 1000000 --log-every 500000"
        ),
    )
    ap.add_argument("input", type=Path)
    ap.add_argument("-o", "--output", type=Path, required=True)
    ap.add_argument("--seed", type=int, default=0)
    ap.add_argument("--t0", type=float, default=2.5)
    ap.add_argument("--alpha", type=float, default=0.999999999)
    ap.add_argument("--max-seconds", type=float, default=None)
    ap.add_argument("--max-iters", type=int, default=None)
    ap.add_argument(
        "--save-every",
        type=int,
        default=1 << 23,
        help="write best grid every N iterations (blog uses 2^23)",
    )
    ap.add_argument("--warm", type=Path, default=None, help="optional starting .out")
    ap.add_argument("--log-every", type=int, default=0)
    args = ap.parse_args()

    text = args.input.read_text(encoding="utf-8")
    g_in, _, _ = parse_grid(text.splitlines())

    rng = random.Random(args.seed)
    lines, best_e = run_sa(
        g_in,
        rng,
        t0=args.t0,
        alpha=args.alpha,
        max_seconds=args.max_seconds,
        max_iters=args.max_iters,
        save_every=args.save_every,
        out_path=args.output,
        warm_path=args.warm,
        log_every=args.log_every,
    )
    print(f"best_energy={best_e}  -> {args.output}", file=sys.stderr)


if __name__ == "__main__":
    main()
