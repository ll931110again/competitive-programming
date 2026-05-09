# CodeSprint LA 2025 – Open

Contest index: [codesprintla25open/problems](https://codesprintla25.kattis.com/contests/codesprintla25open/problems).

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
| `hextechordnance.cpp` | [Hextech Ordnance](https://open.kattis.com/problems/hextechordnance) | O(n²) check — upgrade for `n=10⁶` |
| `topsidevszaun.cpp` | [Topside vs Zaun](https://open.kattis.com/problems/topsidevszaun) | Bitset DP over sum difference (~512 MiB) |

## Partial / stubs

`evasion.cpp`, `getjinxed.cpp`, `mappingtheundercity.cpp`, `olympicceremony.cpp`, `popcornbutton.cpp`, `teamup2.cpp` are placeholders or need full modelling.
