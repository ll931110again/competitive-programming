# AtCoder Heuristic Contest (AHC 001–067)

SWE-style solutions under `atcoder/ahcNNN/`. Official tools live in `atcoder/ahcNNN/tools/`.

Calibrate: `scripts/ahc_calibrate.sh ahcNNN` (provisional counts in `AHC_PROVISIONAL_TESTS.md`).

## Tier-B upgrades (top approaches incorporated)

| # | Problem | Solution | Provisional notes |
|---|---------|----------|-------------------|
| 001 | AtCoder Ad | `a_atcoder_ad.cc` | kmyk border SA; 50 seeds avg ~949M |
| 002 | Walking on Tiles | `a_walking_on_tiles.cc` | kmyk splice SA; 100 seeds avg ~62k |
| 003 | Shortest Path Queries | `a_shortest_path_queries.cc` | kmyk M1/M2 + refit; ~942M (5-seed) |
| 004 | Alien Genome | `a_alien_genome_assembly.cc` | merge + adaptive greedy + HC; 100 seeds avg ~67M |
| 005 | Patrolling | `a_patrolling.cc` | multi-start greedy |
| 006 | Food Delivery | `a_food_delivery.cc` | order SA + multi-start; 5-seed avg ~8381 |
| 007 | Online MST | `a_online_mst.cc` | MC accept/reject; 5-seed avg ~44.07M |
| 010 | Loop Lines | `a_loop_lines.cc` | multi-restart SA + 280ms local search; 5-seed avg ~2651 |
| 060 | Ice Cream Shop Tour | `a_ice_cream_shop_tour.cc` | BFS tree + LCA paths; 5-seed avg ~24 |
| 065 | Conveyor Design | `a_conveyor_design.cc` | time-budget Warnsdorff; 5-seed avg ~868k |
| 066 | Macro Controller | `a_macro_controller.cc` | beam order (M>12) + SA; 5-seed avg ~291 |
| 067 | Castle Renovation | `a_castle_renovation.cc` | Gray-code tree + 9-link ring; 10-seed avg ~1.42M |

## SWE pass complete (005–059)

All `a_*.cc` in this range now have `using i64 = long long` and anonymous `namespace` where helpers/globals exist (27 files updated). Main-only solutions (041–042, 046, 049) keep logic in `main` with `i64` alias.

## Interactive / recent

| # | Problem | Solution | Notes |
|---|---------|----------|-------|
| 061 | Multi-Player Territory | `a_multi_player_territory_game.cc` | 1-ply joint-turn sim; calibrate via stdin tester |
| 062 | King's Tour | `a_kings_tour.cc` | serpentine + validated SA; 5-seed avg ~400M |
| 063 | Colorful Ouroboros | `a_colorful_ouroboros.cc` | BFS fruit order |
| 064 | Railcar Rearrangement | `a_non_crossing_railcar_rearrangement.cc` | siding algorithm |

## Next priorities

1. **067** — Gray-code / nested tree embedding for 10⁷+ scores
2. **061** — deeper opponent model (official AI weights) + calibrate on provisional
3. **010** — equal-time multi-restart SA (current single long restart)
4. **005–059** — tier-B SA/beam per `AHC_REFERENCES.md` where scores lag

Download tools: `python3 scripts/download_ahc_tools.py 1-67`
