# AHC solution quality roadmap

Baseline (2026-07): all 67 contests have a first-pass SWE-style solution — mostly greedy,
beam, or single-pass heuristics. Target: **tier B** (solid local search / SA) on each,
**tier A** (editorial-level) on selected contests.

## Quality tiers

| Tier | Typical techniques | Goal |
|------|-------------------|------|
| D | Naive greedy, fixed beam | ~~current default~~ |
| C | Multi-start greedy, time-limited DFS | minimum bar |
| B | SA / hill-climbing with problem-specific neighborhood | **default target** |
| A | Beam + SA, MCTS, LP/flow subroutines, heavy tuning | top ~25% local |

## Priority queue (improve first)

1. **Recent / high visibility:** 067, 065, 060–066
2. **Classic teaching value:** 001 (SA on cuts), 002 (splice SA), 003 (better learning)
3. **Interactive / rich structure:** 018, 009, 011, 014

## In progress

| # | Problem | Was | Now | Notes |
|---|---------|-----|-----|-------|
| 004 | Alien Genome | greedy shuffle | merge + adaptive greedy + HC | 100 prov. avg **67.1M** (was ~50M); refs in `AHC_REFERENCES.md` |
| 001 | AtCoder Ad | BSP 32 variants | kmyk border SA | 50 prov. avg **949M** (was ~713M) |
| 003 | Shortest Path Queries | Dijkstra + 2-seg fit | hybrid kmyk M1/M2 + refit | 5-seed avg **942M** (was ~921M) |
| 002 | Walking on Tiles | beam 500 | DFS init + splice SA | 100 prov. total 6,180,142 (avg 61,801) |
| 066 | Macro Controller | greedy + compress | multi-pattern macro + order SA | 5-seed avg **291** |
| 006 | Food Delivery | greedy + SA | order SA + multi-start select | 5-seed avg **~8381** |
| 060 | Ice Cream Shop Tour | DFS from 0 (broken nav) | BFS tree + backtrack fix | 5-seed avg **~24** |
| 062 | King's Tour | multi-start serpentine + time SA | consecutive-row serpentine + valid SA | 5-seed avg **~400M** |
| 065 | Conveyor Design | fixed Warnsdorff | time-budget path search | 5-seed avg **~868k** |
| 067 | Castle Renovation | ring + tree branches | Gray-code tree (3–9 sw) + 9-link ring | 10-seed avg **~1.42M** (best ~1.89M) |
| 007 | Online MST | MC 12–28 samples | MC 40–120 + adaptive margin | 5-seed avg **~44.07M** |
| 010 | Loop Lines | SA + 150ms LS | SA + **280ms** hill-climb | 5-seed avg **~2651** (high variance) |
| 061 | Multi-Player Territory | greedy tie-break | 1-ply joint-turn sim (top-12) | 5-seed avg **~97k**; stdin tester |
| 062 | King's Tour | broken row-perm serpentine | consecutive-row serpentine + valid SA | 5-seed avg **~400M** |
| calibrate | stdin testers broken | `ahc_calibrate.sh` stdin vs file-arg | fixes AHC061/045/016/etc. |

## Per-contest checklist (when upgrading)

- [ ] Read editorial / top GitHub repos (see [`AHC_REFERENCES.md`](AHC_REFERENCES.md))
- [ ] Implement tier-B neighborhood + time-budget SA
- [ ] Score on all **provisional** tests (`scripts/ahc_calibrate.sh ahcNNN`; counts in `AHC_PROVISIONAL_TESTS.md`)
- [ ] Keep SWE structure (named types, `namespace`, header comment)

Run local tests with concurrency capped: `scripts/max_parallel_jobs.sh`

## Calibration

Official **provisional** test counts vary by contest (50–300). See [`AHC_PROVISIONAL_TESTS.md`](AHC_PROVISIONAL_TESTS.md).

```bash
# All provisional seeds for one contest (0 .. N-1)
scripts/ahc_calibrate.sh ahc067

# Quick subset
scripts/ahc_calibrate.sh ahc002 0 1 2

# Refresh counts from AtCoder (slow)
python3 scripts/scrape_ahc_provisional_counts.py
```

Per-contest `./test.sh` scripts delegate to `ahc_calibrate.sh`. When `tools/in/` has fewer files than the official count, generate more with `cargo run -r --bin gen seeds.txt` in `tools/`.
