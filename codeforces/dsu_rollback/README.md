# Reversible (rollback) union–find

**Rollback DSU** records each `unite` on a stack; `rollback(snapshot())` undoes them. Use **union by size** and **find without path compression** (path compression breaks worst-case rollback bounds).

## Implemented solutions

| File | Problem | Technique |
|------|---------|-----------|
| [../spectral/R1/f_building_tree.cc](../spectral/R1/f_building_tree.cc) | [CF 2222F — Building Tree](https://codeforces.com/contest/2222/problem/F) | D&C on weight buckets + rollback graph DSU + MST on colors |
| [../edu_dsu_step3/a_dsu_with_rollback.cc](../edu_dsu_step3/a_dsu_with_rollback.cc) | [EDU DSU step 3A](https://codeforces.com/edu/course/2/lesson/7/3/practice/contest/289392/problem/A) | Batch stack (`union` / `persist` / `rollback`); print only on `union`/`rollback` |
| [../cf763/e_timofey_and_animals.cc](../cf763/e_timofey_and_animals.cc) | [CF 763E — Timofey and animals](https://codeforces.com/contest/763/problem/E) | Mo's on `[l, r]` + rollback DSU for left extension |
| [../cf603/e_pastoral_oddities.cc](../cf603/e_pastoral_oddities.cc) | [CF 603E — Pastoral Oddities](https://codeforces.com/contest/603/problem/E) | D&C on query time + rollback DSU tracking odd component count |
| [../cf1386/c_joker.cc](../cf1386/c_joker.cc) | [CF 1386C — Joker](https://codeforces.com/contest/1386/problem/C) | Bipartite rollback DSU + **DSU queue** ([tutorial](https://codeforces.com/blog/entry/83467)) |
| [../cf892/e_envy.cc](../cf892/e_envy.cc) | [CF 892E — Envy](https://codeforces.com/contest/892/problem/E) | Offline MST queries + rollback DSU |
| [../cf914/e_palindromes_in_tree.cc](../cf914/e_palindromes_in_tree.cc) | [CF 914E — Palindromes in a Tree](https://codeforces.com/contest/914/problem/E) | Centroid decomposition + 20-bit path masks |
| [../cf1217/f_forced_online_queries.cc](../cf1217/f_forced_online_queries.cc) | [CF 1217F — Forced Online Queries](https://codeforces.com/contest/1217/problem/F) | CDQ on time + rollback DSU |
| [../gym100551/a_connect_and_disconnect.cc](../gym100551/a_connect_and_disconnect.cc) | [Gym 100551A](https://codeforces.com/problemset/gymProblem/100551/A) | Segment tree on time + rollback DSU |
| [../cf936/e_iqea.cc](../cf936/e_iqea.cc) | [CF 936E — Iqea](https://codeforces.com/contest/936/problem/E) | D&C on time + rollback multi-source BFS |

## Library

| Header | Purpose |
|--------|---------|
| [`../../lib/dsu_rollback.hh`](../../lib/dsu_rollback.hh) | `DsuRollback`, `DsuRollbackMarked` |
| [`../../lib/dsu_rollback_odd.hh`](../../lib/dsu_rollback_odd.hh) | `DsuRollbackOdd` — odd-sized component count (603E) |
| [`../../lib/dsu.hh`](../../lib/dsu.hh) | Standard DSU with path compression |
| [`../../lib/offline_dynamic_connectivity.hh`](../../lib/offline_dynamic_connectivity.hh) | Segment tree over time + rollback DSU (template) |

## More problems (same ideas, not in repo yet)

| Problem | Link | Technique |
|---------|------|-----------|
| DSU rollback (VNOJ) | [dsu_p](https://oj.vnoi.info/problem/dsu_p) | Same as EDU 3A |

## Technique cheat sheet

| Pattern | When to use |
|---------|-------------|
| **Batch stack** (`persist` / `rollback`) | Explicit checkpoints; undo last segment of unions |
| **Snapshot stack** (`snapshot()` / `rollback(t)`) | D&C, segment tree on time, Mo's left extension |
| **Mo + rollback** | Static edges; range `[l, r]` queries on induced subgraph |
| **D&C on time + weight** | Monotone answers as edges arrive (603E) |
| **DSU queue** | Queue-like undo (oldest merge) for sliding windows (1386C) |
| **Bipartite DSU + rollback** | Odd-cycle / 2-coloring with backtracking |
| **D&C on time + BFS rollback** | Adds in left half answer queries in right; stack `dist` updates (936E) |

## References

- [CP-Algorithms — DSU](https://cp-algorithms.com/data_structures/disjoint_set_union.html)
- [USACO Guide — Offline deletion](https://usaco.guide/adv/offline-del)
- [Noam527 — Queue-like undoing on DS](https://codeforces.com/blog/entry/83467)
- [kmyk — Rollback square decomposition](https://kmyk.github.io/competitive-programming-library/old/rollback-square-decomposition.inc.cpp.html)
