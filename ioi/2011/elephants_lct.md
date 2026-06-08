# IOI 2011 Day 2 — Dancing Elephants (Link-Cut Tree)

Implementation: [`elephants_lct.cc`](elephants_lct.cc)  
Alternative (sqrt buckets): [`elephants.cc`](elephants.cc)

## Problem (compressed)

There are `N` elephants on a number line at positions `X[0], …, X[N−1]` (**points**, not
intervals). A **camera** captures a segment of length `L` (endpoints included). One camera
can photograph a group of elephants iff all their positions lie in **some** segment of
length `L`.

After each `update(i, new_x)`, return the **minimum number of cameras** needed to cover
**all** points.

Official statement: [DMOJ IOI 2011 P5](https://dmoj.ca/problem/ioi11p5).

### Static greedy (optimal in 1D)

Sort positions. Repeatedly take the **leftmost uncovered** point `x_j`, place a camera on
`[x_j, x_j + L]`, mark all points in that segment covered. **`O(N)`** after sort.

[`elephants.cc`](elephants.cc) implements this with sqrt decomposition for dynamic updates.

## LCT formulation (not “elephants are intervals”)

The code uses auxiliary events at `X[i]` and `X[i]+L`. These are **not** elephant bodies;
they encode the greedy cover in a timeline:

| Event kind | Coordinate | Node id | Edge weight |
|------------|------------|---------|-------------|
| “Open” for elephant `i` | `X[i]` | `2i` | **1** |
| “Close” for elephant `i` | `X[i] + L` | `2i + 1` | **0** |

Intuition: placing a camera with left endpoint at the leftmost uncovered point opens an
obligation spanning `[x_j, x_j+L]`; the timeline scan counts how many such obligations are
needed.

Sort all `2N` events; link each to the next in order (with per-elephant links `2i → 2i+1`).
The weighted path sum from a left sentinel equals the **minimum number of cameras**.

## Dynamic part

Only **timeline successor links** change when one point moves. Links `2i → 2i+1` never
change.

Add sentinels `2N`, `2N+1` at `-∞` and `+∞`. After each update:

```text
return LCT::query(tree[2N]);   // min # cameras
```

## Why `std::set` + local relink is enough

Store events in `std::set<pair<position, id>>`.

- Start `2i` at `(X[i], 2i)`.
- End `2i+1` at `(X[i] + L, 2i+1 + OFFSET)` so ends sort after starts at equal coordinates.

On `update(elephant, new_x)`: cut/erase old pair, insert new pair, relink **`O(1)`**
neighbors → **`O(log N)`** LCT work.

## Link-cut tree used here

Merge/split treap LCT (`namespace LCT`): `access`, `link`, `cut`, `query` on path sums.

## Complexity

| | |
|---|---|
| Per update | `O(log N)` |
| Memory | `O(N)` |
| `init` | `O(N log N)` |

Sqrt version: **`O(√N)`** amortized ([`elephants.cc`](elephants.cc)).

## References

- [DMOJ problem + editorial](https://dmoj.ca/problem/ioi11p5)
- [Codeforces blog — Elephants LCT](https://codeforces.com/blog/entry/56486)
- [Kazuma8128 writeup](https://kazuma8128.hatenablog.com/entry/2018/02/18/014946)
- [oj.uz #55017](https://oj.uz/submission/55017)

## File map

```text
init(N, L, X[])
  create nodes 2i, 2i+1; link 2i → 2i+1
  insert timeline pairs; relink_all()

update(i, new_x)
  cut + erase old (start, end); insert new; relink local iterators
  return LCT::query(tree[2N])
```
