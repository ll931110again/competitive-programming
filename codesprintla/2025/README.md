# CodeSprint LA 2025 – Open

Contest index: [codesprintla25open/problems](https://codesprintla25.kattis.com/contests/codesprintla25open/problems).

**Problem statements (offline):** see [`statements/INDEX.md`](statements/INDEX.md) (`*.md` = extracted `problembody` + link; `*.html.full` = full page). Refresh with `python3 codesprintla/tools/fetch_kattis_statements.py`.

Build with the repo’s `bits/stdc++.h`:

```bash
g++ -std=c++17 -O2 -I "$(git rev-parse --show-toplevel)" amulet.cpp -o amulet
```

## Implemented solutions

| File | Problem | Notes |
|------|---------|--------|
| `amulet.cpp` | [The Amulet](https://open.kattis.com/problems/amulet) | Rolling hash, decreasing length |
| `arcanesecret.cpp` | [Arcane Secret](https://open.kattis.com/problems/arcanesecret) | Exact brute for tractable sizes; tail heuristic |
| `balancingthecouncil.cpp` | [Balancing the Council](https://open.kattis.com/problems/balancingthecouncil) | Yes iff `M ∉ {1,N−1}` |
| `chemtechcontagion.cpp` | [Chemtech Contagion](https://open.kattis.com/problems/chemtechcontagion) | Close vents crossing tree path–outside boundary |
| `hextechordnance.cpp` | [Hextech Ordnance](https://open.kattis.com/problems/hextechordnance) | Monotone deques per threshold `k`; if `k ≥ max−min`, count `n(n−1)/2` |
| `mappingtheundercity.cpp` | [Mapping the Undercity](https://open.kattis.com/problems/mappingtheundercity) | Spine/buckets + rooted trees with fixed depths per attachment |
| `olympicceremony.cpp` | [Olympic Ceremony](https://open.kattis.com/problems/olympicceremony) | Interactive: binary search maximal segment with one distinct district |
| `topsidevszaun.cpp` | [Topside vs Zaun](https://open.kattis.com/problems/topsidevszaun) | Bitset DP over sum difference (~512 MiB) |

## Stubs / incomplete

| File | Problem |
|------|---------|
| `evasion.cpp` | [Evasion](https://open.kattis.com/problems/evasion) |
| `getjinxed.cpp` | [Get Jinxed!](https://open.kattis.com/problems/getjinxed) |
| `popcornbutton.cpp` | [Popcorn Button](https://open.kattis.com/problems/popcornbutton) |
| `teamup2.cpp` | [Team Up!](https://open.kattis.com/problems/teamup2) |
