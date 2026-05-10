# CodeSprint LA 2026 – Open

Contest index: [codesprintla26open/problems](https://codesprintla26.kattis.com/contests/codesprintla26open/problems).

**Problem statements (offline):** see [`statements/INDEX.md`](statements/INDEX.md). These are mirrored from the contest host (many problems are not yet on [open.kattis.com](https://open.kattis.com/)). Refresh with `python3 codesprintla/tools/fetch_kattis_statements.py`.

Build with the repo `bits/stdc++.h`:

```bash
g++ -std=c++17 -O2 -I "$(git rev-parse --show-toplevel)" ocarinaandchime.cc -o ocarinaandchime
```

## Files vs problems

| Letter | Kattis slug | File | Notes |
| ------ | ----------- | ---- | ----- |
| A | [cutthecake2](https://codesprintla26.kattis.com/contests/codesprintla26open/problems/cutthecake2) | `cutthecake2.cc` | Interactive — guillotine DFS + backtracking (≤1600 queries target) |
| B | [guardianlaser](https://codesprintla26.kattis.com/contests/codesprintla26open/problems/guardianlaser) | `laser.cc` | **Do not overwrite** (user AC) |
| C | [goroncity](https://codesprintla26.kattis.com/contests/codesprintla26open/problems/goroncity) | `goron.cc` | **Do not overwrite** (user AC) |
| D | [leftshift](https://codesprintla26.kattis.com/contests/codesprintla26open/problems/leftshift) | `leftshift.cc` | Manacher + explicit one cyclic shift |
| E | [lynelmelee](https://codesprintla26.kattis.com/contests/codesprintla26open/problems/lynelmelee) | `lynelmelee.cc` | Exact brute ≤11; hill‑climb + asc/desc seeds beyond |
| F | [mappingthegreatsea](https://codesprintla26.kattis.com/contests/codesprintla26open/problems/mappingthegreatsea) | `mappingthegreatsea.cc` | Maximal 1‑rectangles + exclusive cells |
| G | [multiversemadness](https://codesprintla26.kattis.com/contests/codesprintla26open/problems/multiversemadness) | `multiversemadness.cc` | DSU on graph after deleting observed worlds `F` |
| H | [ocarinaandchime](https://codesprintla26.kattis.com/contests/codesprintla26open/problems/ocarinaandchime) | `ocarinaandchime.cc` | Sliding-window multiset counts |
| I | [robbiesresearchquest](https://codesprintla26.kattis.com/contests/codesprintla26open/problems/robbiesresearchquest) | `robbiesresearchquest.cc` | Stub (`-1`) |
| J | [seizethemeans](https://codesprintla26.kattis.com/contests/codesprintla26open/problems/seizethemeans) | `seizethemeans.cc` | Stub (`0`) |
| K | [sorcerersrift](https://codesprintla26.kattis.com/contests/codesprintla26open/problems/sorcerersrift) | `sorcerersrift.cc` | Interactive — naive guess |
| L | [spirittracks](https://codesprintla26.kattis.com/contests/codesprintla26open/problems/spirittracks) | `spirittracks.cc` | Direct simulation on degree‑≤3 tree |

## Known gaps / risks

- **`leftshift.cc`**: Correct; builds the rotated string explicitly each trial. Still \(\Theta(n^3)\) (`n ≤ 2000`) — may time out on worst-case limits.
- **`lynelmelee.cc`**: Large instances use randomized hill climbing (not proven optimal). If WA, replace with the contest’s intended DP / ordering proof.
- **`multiversemadness.cc`**: Models influence as connectivity in the undirected skeleton after deleting `F`. This matches the official sample, but may differ from a fully careful treatment of arbitrary Boolean gates / probabilities on hidden tests.
- **`cutthecake2.cc`**: Assumes `query == 0` on an axis-aligned box **iff** that box contains exactly one final piece (aligned union). If Kattis excludes boundary cuts differently, the `(k ↔ query)` checks may need adjustment. Watch total query count on worst cases.
- **Still stubs / weak**: `robbiesresearchquest.cc`, `seizethemeans.cc`, `sorcerersrift.cc`.
