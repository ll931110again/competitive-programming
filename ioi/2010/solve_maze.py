#!/usr/bin/env python3
"""
IOI 2010 Day 2 Problem G — Maze (output-only heuristic).

Maximize P = max shortest-path distance (in cells) from the unique perimeter '.'
(entrance) to any reachable '.' cell.

The grader places the entrance on the unique boundary '.' and the core is the
farthest '.' by BFS. For a *simple path* (no extra branches), P equals the
number of '.' on that path; extra '.' off the shortest route only add shortcuts
and lower P. So we only leave '.' on one long simple path in the largest '#'
component.

Rules from the statement: exactly one '.' on the field border (entrance); every
other '.' must be strictly inside the rectangle (not on the outer row/column).

We use the largest '#' component that touches the border and has an entrance
adjacent to interior '#'.  Vertices are interior cells plus one chosen entrance.
Heuristics: interior rectangle serpentine (when applicable), global geodesic
from the entrance, Warnsdorff extension, and many random simple walks.  Phase 1
samples up to 72 entrances and picks one with the longest geodesic; phase 2
runs the heavier heuristics only for that entrance.
"""

from __future__ import annotations

import argparse
import random
from collections import deque
from typing import Dict, List, Optional, Sequence, Set, Tuple

Coord = Tuple[int, int]

DIRS = ((1, 0), (-1, 0), (0, 1), (0, -1))


def neighbors(r: int, c: int, n: int, m: int) -> List[Coord]:
    out: List[Coord] = []
    for dr, dc in DIRS:
        nr, nc = r + dr, c + dc
        if 0 <= nr < n and 0 <= nc < m:
            out.append((nr, nc))
    return out


def parse_grid(lines: List[str]) -> Tuple[List[str], int, int]:
    g = [ln.rstrip("\n") for ln in lines if ln.strip() != ""]
    n, m = len(g), len(g[0])
    for row in g:
        assert len(row) == m
    return g, n, m


def is_perimeter(i: int, j: int, n: int, m: int) -> bool:
    return i == 0 or i == n - 1 or j == 0 or j == m - 1


def build_masks(g: List[str], n: int, m: int) -> Tuple[int, List[Coord], List[Coord]]:
    """Positions where input is '#': can become '.'; sides = perimeter among those."""
    all_hash: List[Coord] = []
    sides: List[Coord] = []
    for i in range(n):
        for j in range(m):
            if g[i][j] == "#":
                all_hash.append((i, j))
                if is_perimeter(i, j, n, m):
                    sides.append((i, j))
    return len(all_hash), all_hash, sides


def largest_component_cells(g: List[str], n: int, m: int) -> Set[Coord]:
    """Largest 4-connected component of '#' cells."""
    best: Set[Coord] = set()
    seen: Set[Coord] = set()
    for i in range(n):
        for j in range(m):
            if g[i][j] != "#" or (i, j) in seen:
                continue
            stack = [(i, j)]
            seen.add((i, j))
            comp: Set[Coord] = {(i, j)}
            while stack:
                r, c = stack.pop()
                for nr, nc in neighbors(r, c, n, m):
                    if g[nr][nc] != "#" or (nr, nc) in seen:
                        continue
                    seen.add((nr, nc))
                    comp.add((nr, nc))
                    stack.append((nr, nc))
            if len(comp) > len(best):
                best = comp
    return best


def interior_cells(comp: Set[Coord], n: int, m: int) -> Set[Coord]:
    """Cells of comp that are strictly inside the field (not on the outer border)."""
    return {(i, j) for i, j in comp if not is_perimeter(i, j, n, m)}


def valid_entrances(comp: Set[Coord], n: int, m: int) -> List[Coord]:
    """
    Perimeter '#' cells that touch at least one interior '#' (so the maze can
    enter the field).  IOI requires every '.' except the entrance to be interior.
    """
    inside = interior_cells(comp, n, m)
    out: List[Coord] = []
    for i, j in comp:
        if not is_perimeter(i, j, n, m):
            continue
        for ni, nj in neighbors(i, j, n, m):
            if (ni, nj) in inside:
                out.append((i, j))
                break
    return out


def largest_boundary_component(g: List[str], n: int, m: int) -> Set[Coord]:
    """Prefer the largest '#' component that touches the field boundary (entrance must live there)."""
    best_touch: Set[Coord] = set()
    best_any: Set[Coord] = set()
    seen: Set[Coord] = set()
    for i in range(n):
        for j in range(m):
            if g[i][j] != "#" or (i, j) in seen:
                continue
            stack = [(i, j)]
            seen.add((i, j))
            comp: Set[Coord] = {(i, j)}
            touches = False
            while stack:
                r, c = stack.pop()
                if is_perimeter(r, c, n, m):
                    touches = True
                for nr, nc in neighbors(r, c, n, m):
                    if g[nr][nc] != "#" or (nr, nc) in seen:
                        continue
                    seen.add((nr, nc))
                    comp.add((nr, nc))
                    stack.append((nr, nc))
            if len(comp) > len(best_any):
                best_any = comp
            if touches and len(comp) > len(best_touch):
                best_touch = comp
    return best_touch if best_touch else best_any


def adjacency_for_cells(cells: Set[Coord], n: int, m: int) -> Dict[Coord, List[Coord]]:
    adj: Dict[Coord, List[Coord]] = {}
    cell_set = cells
    for r, c in cells:
        adj[(r, c)] = []
        for nr, nc in neighbors(r, c, n, m):
            if (nr, nc) in cell_set:
                adj[(r, c)].append((nr, nc))
    return adj


def recover_path(parent: Dict[Coord, Optional[Coord]], src: Coord, dst: Coord) -> List[Coord]:
    path = [dst]
    cur = dst
    while cur != src:
        p = parent[cur]
        assert p is not None
        cur = p
        path.append(cur)
    path.reverse()
    return path


def warnsdorff_extend_path(
    adj: Dict[Coord, List[Coord]],
    start: Coord,
    cells: Set[Coord],
    rng: random.Random,
) -> List[Coord]:
    """Greedy non-backtracking walk: prefer neighbor with smallest forward degree."""
    visited: Set[Coord] = {start}
    path = [start]
    cur = start
    while True:
        cand = [v for v in adj[cur] if v not in visited]
        if not cand:
            break

        def score(v: Coord) -> Tuple[int, int]:
            fwd = sum(1 for w in adj[v] if w not in visited)
            return (fwd, rng.randrange(1 << 30))

        nxt = min(cand, key=score)
        visited.add(nxt)
        path.append(nxt)
        cur = nxt
    return path


def warnsdorff_extend_path_lookahead(
    adj: Dict[Coord, List[Coord]],
    start: Coord,
    rng: random.Random,
) -> List[Coord]:
    """
    Two-step Warnsdorff: at cur, each neighbor v gets score
    (deg_unvisited(v), min_{w unvisited neighbor of v} deg_unvisited(w)),
    minimize lexicographically (ties random).
    """
    visited: Set[Coord] = {start}
    path = [start]
    cur = start

    def unvisited_deg(u: Coord) -> int:
        return sum(1 for w in adj[u] if w not in visited)

    def next_step_min_deg(v: Coord) -> int:
        """Assume we step to v; min forward degree among v's unvisited neighbors."""
        visited.add(v)
        m = 999
        for w in adj[v]:
            if w in visited:
                continue
            m = min(m, unvisited_deg(w))
        visited.remove(v)
        return m if m < 999 else 0

    while True:
        cand = [v for v in adj[cur] if v not in visited]
        if not cand:
            break
        best = min(
            cand,
            key=lambda v: (
                unvisited_deg(v),
                next_step_min_deg(v),
                rng.randrange(1 << 25),
            ),
        )
        visited.add(best)
        path.append(best)
        cur = best
    return path


def best_boundary_geodesic_path(
    adj: Dict[Coord, List[Coord]], boundary_sources: Sequence[Coord]
) -> List[Coord]:
    """Longest shortest-path (fewest edges) from any boundary start to its farthest vertex."""
    best: List[Coord] = []
    for start in boundary_sources:
        parent: Dict[Coord, Optional[Coord]] = {start: None}
        dist: Dict[Coord, int] = {start: 0}
        q: deque[Coord] = deque([start])
        while q:
            u = q.popleft()
            du = dist[u]
            for v in adj[u]:
                if v not in parent:
                    parent[v] = u
                    dist[v] = du + 1
                    q.append(v)
        far = max(dist, key=lambda x: dist[x])
        path = recover_path(parent, start, far)
        if len(path) > len(best):
            best = path
    return best


def random_simple_walk(
    adj: Dict[Coord, List[Coord]], start: Coord, rng: random.Random
) -> List[Coord]:
    visited: Set[Coord] = {start}
    path = [start]
    cur = start
    while True:
        cand = [v for v in adj[cur] if v not in visited]
        if not cand:
            break
        cur = rng.choice(cand)
        visited.add(cur)
        path.append(cur)
    return path


def random_simple_walk_degree_biased(
    adj: Dict[Coord, List[Coord]], start: Coord, rng: random.Random
) -> List[Coord]:
    """Prefer stepping into vertices that still have many unvisited neighbors."""
    visited: Set[Coord] = {start}
    path = [start]
    cur = start

    def uw_deg(u: Coord) -> int:
        return sum(1 for w in adj[u] if w not in visited)

    while True:
        cand = [v for v in adj[cur] if v not in visited]
        if not cand:
            break
        weights = [max(1, uw_deg(v)) ** 2 for v in cand]
        s = sum(weights)
        t = rng.random() * s
        acc = 0.0
        nxt = cand[-1]
        for v, w in zip(cand, weights):
            acc += w
            if t <= acc:
                nxt = v
                break
        cur = nxt
        visited.add(cur)
        path.append(cur)
    return path


def multi_random_walks(
    adj: Dict[Coord, List[Coord]],
    boundary_sources: Sequence[Coord],
    rng: random.Random,
    trials: int,
    *,
    biased: bool = True,
) -> List[Coord]:
    best: List[Coord] = []
    verts = list(adj.keys())
    picker = random_simple_walk_degree_biased if biased else random_simple_walk
    for _ in range(trials):
        start = rng.choice(boundary_sources) if boundary_sources else rng.choice(verts)
        p = picker(adj, start, rng)
        if len(p) > len(best):
            best = p
    return best


def try_long_path(
    adj: Dict[Coord, List[Coord]],
    cells: Set[Coord],
    boundary_in_cells: List[Coord],
    rng: random.Random,
    warnsdorff_repeats: int,
    *,
    use_lookahead: bool = True,
) -> List[Coord]:
    """Many seeded Warnsdorff runs (standard + optional lookahead)."""
    best: List[Coord] = []
    cell_list = list(cells)
    la_reps = max(1, warnsdorff_repeats // 2)
    for rep in range(warnsdorff_repeats):
        r2 = random.Random(rng.randrange(1 << 31) ^ rep)
        if boundary_in_cells:
            s = r2.choice(boundary_in_cells)
        else:
            s = r2.choice(cell_list)
        p = warnsdorff_extend_path(adj, s, cells, r2)
        if len(p) > len(best):
            best = p
    if use_lookahead:
        for rep in range(la_reps):
            r3 = random.Random(rng.randrange(1 << 30) ^ rep ^ 0x9E37)
            if boundary_in_cells:
                s = r3.choice(boundary_in_cells)
            else:
                s = r3.choice(cell_list)
            q = warnsdorff_extend_path_lookahead(adj, s, r3)
            if len(q) > len(best):
                best = q
    return best


def grid_from_path(
    g_in: List[str],
    n: int,
    m: int,
    path_cells: Set[Coord],
    entrance: Coord,
) -> List[str]:
    rows = [list(row) for row in g_in]
    for i in range(n):
        for j in range(m):
            if g_in[i][j] == "#":
                rows[i][j] = "." if (i, j) in path_cells else "#"
            else:
                rows[i][j] = g_in[i][j]
    er, ec = entrance
    rows[er][ec] = "."
    return ["".join(r) for r in rows]


def count_boundary_dots(rows: List[str], n: int, m: int) -> int:
    c = 0
    for i in range(n):
        for j in range(m):
            if rows[i][j] != ".":
                continue
            if is_perimeter(i, j, n, m):
                c += 1
    return c


def eccentricity_energy(rows: List[str], n: int, m: int) -> Tuple[int, Coord]:
    """Return (max BFS distance in cells from entrance, entrance coord)."""
    ent: Optional[Coord] = None
    for i in range(n):
        for j in range(m):
            if rows[i][j] == "." and is_perimeter(i, j, n, m):
                ent = (i, j)
                break
        if ent is not None:
            break
    if ent is None:
        return -10**9, (0, 0)

    si, sj = ent
    dist = [[-1] * m for _ in range(n)]
    dist[si][sj] = 1
    q: deque[Coord] = deque([(si, sj)])
    best = 1
    while q:
        i, j = q.popleft()
        d0 = dist[i][j]
        best = max(best, d0)
        for ni, nj in neighbors(i, j, n, m):
            if rows[ni][nj] != ".":
                continue
            if dist[ni][nj] != -1:
                continue
            dist[ni][nj] = d0 + 1
            q.append((ni, nj))
    return best, ent


def allowed_vertices_for_entrance(
    comp: Set[Coord], n: int, m: int, entrance: Coord
) -> Set[Coord]:
    """Interior crushed cells plus the single entrance on the field perimeter."""
    return interior_cells(comp, n, m) | {entrance}


def interior_rectangle_snake(comp: Set[Coord], n: int, m: int) -> Optional[List[Coord]]:
    """
    Serpentine through interior(comp) if it fills a complete axis-aligned rectangle
    of strictly interior cells (does not use the outer field border).
    """
    ins = interior_cells(comp, n, m)
    if not ins:
        return None
    rs = [r for r, _ in ins]
    cs = [c for _, c in ins]
    r0, r1 = min(rs), max(rs)
    c0, c1 = min(cs), max(cs)
    if len(ins) != (r1 - r0 + 1) * (c1 - c0 + 1):
        return None
    for r in range(r0, r1 + 1):
        for c in range(c0, c1 + 1):
            if is_perimeter(r, c, n, m):
                return None
            if (r, c) not in ins:
                return None
    path: List[Coord] = []
    for idx, r in enumerate(range(r0, r1 + 1)):
        cols = range(c0, c1 + 1) if idx % 2 == 0 else range(c1, c0 - 1, -1)
        for c in cols:
            path.append((r, c))
    return path


def greedy_lex_path(
    adj: Dict[Coord, List[Coord]],
    entrance: Coord,
    allowed: Set[Coord],
    rng: random.Random,
    *,
    row_major: bool,
) -> List[Coord]:
    """
    Greedy walk: each step picks a neighbor minimizing rank in sorted interior
    order (row-major or column-major).
    """
    key = ((lambda x: (x[0], x[1])) if row_major else (lambda x: (x[1], x[0])))
    interior_sorted = sorted((x for x in allowed if x != entrance), key=key)
    rank: Dict[Coord, int] = {c: i for i, c in enumerate(interior_sorted)}
    visited: Set[Coord] = {entrance}
    path: List[Coord] = [entrance]
    cur = entrance
    while True:
        cand = [v for v in adj[cur] if v in allowed and v not in visited]
        if not cand:
            break
        pick = min(
            cand,
            key=lambda v: (rank.get(v, 10**9), rng.randrange(1 << 25)),
        )
        visited.add(pick)
        path.append(pick)
        cur = pick
    return path


def prepend_entrance_to_interior_path(
    entrance: Coord, interior_path: List[Coord], adj: Dict[Coord, List[Coord]]
) -> Optional[List[Coord]]:
    """Return [entrance]+interior_path oriented so the first step is legal, or None."""
    if not interior_path:
        return [entrance]
    nbr = adj.get(entrance, [])
    if interior_path[0] in nbr:
        return [entrance] + interior_path
    if interior_path[-1] in nbr:
        return [entrance] + interior_path[::-1]
    return None


def path_eccentricity(
    g_in: List[str],
    n: int,
    m: int,
    path: Sequence[Coord],
    entrance: Coord,
) -> int:
    """Score a candidate path by grader energy (not raw vertex count)."""
    p, _ = eccentricity_energy(
        grid_from_path(g_in, n, m, set(path), entrance),
        n,
        m,
    )
    return p


def best_paths_for_entrance(
    g_in: List[str],
    comp: Set[Coord],
    n: int,
    m: int,
    entrance: Coord,
    rng: random.Random,
    random_walk_trials: int,
    warnsdorff_repeats: int,
) -> List[Coord]:
    allowed = allowed_vertices_for_entrance(comp, n, m, entrance)
    adj = adjacency_for_cells(allowed, n, m)
    if entrance not in adj:
        return [entrance]

    candidates: List[List[Coord]] = []

    ins_snake = interior_rectangle_snake(comp, n, m)
    if ins_snake:
        prep = prepend_entrance_to_interior_path(entrance, ins_snake, adj)
        if prep:
            candidates.append(prep)

    for rm in (True, False):
        gl = greedy_lex_path(adj, entrance, allowed, rng, row_major=rm)
        if gl:
            candidates.append(gl)

    biased_trials = max(random_walk_trials, 5_000) * 2
    biased_trials = min(72_000, biased_trials)
    rw = multi_random_walks(adj, [entrance], rng, biased_trials, biased=True)
    rw_u = multi_random_walks(
        adj,
        [entrance],
        rng,
        max(4_000, random_walk_trials // 2),
        biased=False,
    )
    for chunk in (rw, rw_u):
        if chunk:
            candidates.append(chunk)

    wd = try_long_path(
        adj,
        allowed,
        [entrance],
        rng,
        warnsdorff_repeats=warnsdorff_repeats,
        use_lookahead=True,
    )
    if wd:
        candidates.append(wd)

    if not candidates:
        return [entrance]
    return max(
        candidates,
        key=lambda path: path_eccentricity(g_in, n, m, path, entrance),
    )


def trials_budget(n: int, m: int, n_allowed: int) -> Tuple[int, int, int]:
    """
    (random_walk_trials, warnsdorff_repeats, phase1_entrance_cap).
    Tuned for strong scores vs runtime (~few min for all 10 tests).
    """
    area = n * m
    rw = min(22_000, max(3_800, n_allowed * 9))
    wd = min(240, max(60, n_allowed // 20))
    if area <= 400:
        rw = min(40_000, max(5_500, n_allowed * 26))
        wd = min(420, max(115, n_allowed // 8))
    cap = 130 if area >= 80 * 80 else 105
    return rw, wd, cap


def solve_one(
    g_in: List[str],
    rng: random.Random,
    random_walk_trials: int = -1,
    *,
    warnsdorff_repeats: Optional[int] = None,
    top_entrances: Optional[int] = None,
) -> List[str]:
    n, m = len(g_in), len(g_in[0])

    comp = largest_boundary_component(g_in, n, m)
    if not comp:
        return g_in[:]

    entrances = valid_entrances(comp, n, m)
    if not entrances:
        return g_in[:]

    na = len(interior_cells(comp, n, m)) + 1
    rw_default, wd_default, cap_default = trials_budget(n, m, na)
    rw_limit = rw_default if random_walk_trials < 0 else random_walk_trials
    wd_limit = warnsdorff_repeats if warnsdorff_repeats is not None else wd_default
    top_k = top_entrances if top_entrances is not None else min(6, max(4, cap_default // 28))

    # Phase 1 — rank entrances by graph eccentricity (BFS lower-bound on maze depth).
    phase1 = entrances if len(entrances) <= cap_default else rng.sample(entrances, cap_default)
    ranked: List[Tuple[int, Coord]] = []
    for entrance in phase1:
        allowed = allowed_vertices_for_entrance(comp, n, m, entrance)
        adj = adjacency_for_cells(allowed, n, m)
        if entrance not in adj:
            continue
        geo = best_boundary_geodesic_path(adj, [entrance])
        ranked.append((len(geo), entrance))
    ranked.sort(key=lambda t: -t[0])
    if not ranked:
        return g_in[:]

    pool = [e for _, e in ranked[: max(top_k, 1)]]

    best_score = -1
    best_path: List[Coord] = [pool[0]]
    best_ent: Coord = pool[0]
    for entrance in pool:
        allowed = allowed_vertices_for_entrance(comp, n, m, entrance)
        rw_use = min(rw_limit, max(3500, len(allowed) * 26))
        path_try = best_paths_for_entrance(
            g_in, comp, n, m, entrance, rng, rw_use, wd_limit
        )
        score = path_eccentricity(g_in, n, m, path_try, entrance)
        if score > best_score:
            best_score = score
            best_path = path_try
            best_ent = entrance

    path_set = set(best_path)
    return grid_from_path(g_in, n, m, path_set, best_ent)


def main() -> None:
    ap = argparse.ArgumentParser()
    ap.add_argument("input_path")
    ap.add_argument("-o", "--output", required=True)
    ap.add_argument("--seed", type=int, default=1)
    ap.add_argument(
        "--random-walk-trials",
        type=int,
        default=-1,
        help="Random-walk trials per entrance (-1 = auto from grid size)",
    )
    args = ap.parse_args()

    with open(args.input_path, "r", encoding="utf-8") as f:
        g_in, _, _ = parse_grid(f.readlines())

    rng = random.Random(args.seed)
    out = solve_one(g_in, rng, random_walk_trials=args.random_walk_trials)

    with open(args.output, "w", encoding="utf-8") as f:
        for ln in out:
            f.write(ln + "\n")


if __name__ == "__main__":
    main()
