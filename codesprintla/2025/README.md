# CodeSprint LA 2025

Upsolving contest on Kattis: **[CodeSprint LA 2025 – problems](https://codesprintla25.kattis.com/contests/cxeqb3/problems)**.

Stable problem URLs use **`open.kattis.com`** (same statements as the contest hub):

| Letter | Problem | Source file |
| --- | --- | --- |
| **A** | [The Amulet](https://open.kattis.com/problems/amulet) | `amulet.cpp` |
| **B** | [Arcane Secret](https://open.kattis.com/problems/arcanesecret) | `arcanesecret.cpp` |
| **C** | [Balancing the Council](https://open.kattis.com/problems/balancingthecouncil) | `balancingthecouncil.cpp` |
| **D** | [Chemtech Contagion](https://open.kattis.com/problems/chemtechcontagion) | `chemtechcontagion.cpp` |
| **E** | [Dining Disaster](https://open.kattis.com/problems/diningdisaster) | `diningdisaster.cpp` |
| **F** | [Evasion](https://open.kattis.com/problems/evasion) | `evasion.cpp` (stub) |
| **G** | [Get Jinxed!](https://open.kattis.com/problems/getjinxed) | `getjinxed.cpp` (interactive stub) |
| **H** | [Grey Area](https://open.kattis.com/problems/greyarea) | `greyarea.cpp` (stub) |
| **I** | [Hextech Ordnance](https://open.kattis.com/problems/hextechordnance) | `hextechordnance.cpp` (correct \(O(n^2)\); full \(n\) may need faster math) |
| **J** | [If Index](https://open.kattis.com/problems/ifindex) | `ifindex.cpp` (stub) |
| **K** | [Mapping the Undercity](https://open.kattis.com/problems/mappingtheundercity) | `mappingtheundercity.cpp` (stub) |
| **L** | [Odd Rubble](https://open.kattis.com/problems/oddrubble) | `oddrubble.cpp` (stub) |
| **M** | [OK Maps](https://open.kattis.com/problems/okmaps) | `okmaps.cpp` (stub) |
| **N** | [Olympic Ceremony](https://open.kattis.com/problems/olympicceremony) | `olympicceremony.cpp` (interactive stub) |
| **O** | [Plotting the Pilt](https://open.kattis.com/problems/plottingthepilt) | `plottingthepilt.cpp` (stub) |
| **P** | [Popcorn Button](https://open.kattis.com/problems/popcornbutton) | `popcornbutton.cpp` (stub; I/O fixed) |
| **Q** | [Shimmer Robots](https://open.kattis.com/problems/shimmerrobots) | `shimmerrobots.cpp` |
| **R** | [Team Up!](https://open.kattis.com/problems/teamup2) | `teamup2.cpp` (stub) |
| **S** | [Topside vs Zaun](https://open.kattis.com/problems/topsidevszaun) | `topsidevszaun.cpp` |

## Implemented vs partial

- **Done / usable:** A–D, **E** (`diningdisaster.cpp`), C geometry note in `balancingthecouncil.cpp`, `chemtechcontagion.cpp`, **Q** (`shimmerrobots.cpp`), `topsidevszaun.cpp`, H/J/L/O per repo notes.
- **`hextechordnance.cpp`:** Samples and correctness via sliding window per \(k\); worst-case \(O(n^2)\) — may TLE at \(n=10^6\) until a faster characterization is added.
- **Hard / open here:** pursuit–evasion (**F**), interactive (**G**, **N**), counting trees from two distance arrays (**K**), microwave state machine (**P**), game theory (**R**), OK maps construction (**M**), etc.

**Problem statements (offline):** see [`statements/INDEX.md`](statements/INDEX.md) (`*.md` = extracted `problembody` + link; `*.html.full` = full page). Refresh with `python3 codesprintla/tools/fetch_kattis_statements.py`.

Build with the repo’s `bits/stdc++.h`:

```bash
g++ -std=c++17 -O2 -I "$(git rev-parse --show-toplevel)" amulet.cpp -o amulet
```
