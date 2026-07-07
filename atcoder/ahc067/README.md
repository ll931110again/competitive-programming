# AHC067 — Castle Renovation with Linked Doors

- Problem: https://atcoder.jp/contests/ahc067/tasks/ahc067_a
- Solution: `a_castle_renovation.cc`

## Approach

Primary layout: **exponential tree** on a BFS spanning tree (throne- or start-rooted).
`spine_path` walks the unique start→goal tree path (LCA join). Switches 0..8 sit on
distinct side branches (0 near start, higher indices toward throne), complementary
split doors plus ring gates. Switch 9 unused → door 19 walls; `seal_shortcuts` blocks
non-tree edges. Adaptive switch count (3–9) rejects weak plans.

Fallback: on-spine **9-link ring**, branch / hybrid layouts, hill-climb on ring positions.

```bash
scripts/ahc_calibrate.sh ahc067 0 1 2 3 4   # 5-seed quick check
```

| | 5-seed avg | Min | Max |
|--|--:|--:|--:|
| Current (exp tree + ring fallback) | ~1.48M | ~0.96M | ~2.25M |

100-local full run (~1.73M avg historically): `./test.sh`

