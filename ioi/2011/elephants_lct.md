# IOI 2011 Day 2 — Dancing Elephants (Link-Cut Tree)

Implementation: [`elephants_lct.cc`](elephants_lct.cc)  
Alternative (sqrt buckets): [`elephants.cc`](elephants.cc)

## Problem (compressed)

There are `N` elephants on a number line. Elephant `i` occupies the half-open interval
`[X[i], X[i] + L)`. Intervals are pairwise disjoint.

After each update `update(i, new_x)`, elephant `i` moves to `[new_x, new_x + L)` (still
disjoint from all others). The grader asks for the minimum number of **steps** to visit
every elephant, where one step can cover any contiguous union of intervals that currently
touch or overlap.

Equivalently: treat each interval as a segment; one step must cover a connected component
of the union of segments. The answer is the number of connected components in the union,
**minus one** (one walk covers all components with `components − 1` transitions).

So we need the number of connected components in the union of `N` intervals of length `L`
after each move.

## From components to a path on a forest

Sort all **events** by coordinate:

| Event kind | Coordinate | Node id |
|------------|------------|---------|
| Elephant `i` starts | `X[i]` | `2i` (weight **1**) |
| Elephant `i` ends | `X[i] + L` | `2i + 1` (weight **0**) |

Scanning left to right, maintain whether we are inside at least one elephant interval.
When the scan enters an interval, we start a new “active block”; when it leaves all
intervals, the block ends. Two adjacent blocks in this timeline must be connected by one
walk step, so we link consecutive timeline nodes in a path.

**Static structure**

- For each elephant `i`, always link `2i → 2i + 1` (start to end of the same interval).
- Sort all `2N` timeline events; link each event to the **next** event in sorted order
  (by `(position, id)`).

**Dynamic part**

Only the **timeline order links** change when one elephant moves: the edge from each
event to its successor in the global order. The `2i → 2i + 1` links never change.

Add two sentinel nodes `2N` and `2N + 1` at `-∞` and `+∞` so the timeline path is
closed. The answer after each update equals the **path sum** from sentinel `2N` to the
root of the represented forest (all `2i → 2i + 1` edges contribute `1`, all timeline
edges contribute `0`).

So we maintain a forest with path weights and need:

1. Link / cut edges when timeline successors change.
2. Query weighted path sum from `2N` to root — **O(log N)** per operation with a
   link-cut tree.

## Why `std::set` + local relink is enough

Store timeline events in `std::set<pair<position, id>>`.

- Elephant start `2i` is stored as `(X[i], 2i)`.
- Elephant end `2i + 1` is stored as `(X[i] + L, 2i + 1 + OFFSET)` so ends sort after
  starts at the same coordinate (tie-break by id).

On `update(elephant, new_x)`:

1. Erase the old start and end entries; cut their timeline edges.
2. Cut timeline edges of their **old** predecessors (the events before them in the set).
3. Insert new start/end; relink the affected nodes and their predecessors.

Only **O(1)** set nodes change per update, so only **O(1)** link/cut operations.

## Link-cut tree used here

The code implements **Link-Cut Trees** with a **merge/split treap** (splay-free variant
from the references below). Each LCT node stores:

- `val` — weight on the tree edge to its parent (here: `1` on `2i → 2i + 1`, else `0`).
- `sum` — subtree sum in the auxiliary splay/treap.
- `path_parent` — pointer for the **path link** (preferred-path parent in classic LCT).

Core operations (`namespace LCT`):

| Operation | Meaning |
|-----------|---------|
| `access(u)` | Bring the path from root to `u` to the front; return the root of that path. |
| `link(from, to)` | Connect two trees; `from` becomes a child of `to`. |
| `cut(u)` | Remove the edge from `u` to its parent. |
| `query(u)` | `access(u)` then return `sum` of the path root (total weight root → `u`). |

The treap helpers `merge_nodes`, `split_nodes`, `remove_left`, `remove_right` implement
auxiliary-tree rotations via merge/split by index; `pull` propagates `sum`, `size`, and
`path_parent` through preferred paths.

## `relink(it)` — which timeline edge to set

For set iterator `it` (not the last element):

```text
node u = tree[id(it)]
cut(u)

if id is an elephant start (even, not sentinel):
    parent = tree[id(it) + 1]      // same elephant’s end marker
else:
    parent = tree[id(next(it))]    // next event in global order

link(u, parent)
```

Elephant-start nodes link forward to their own end marker; all other nodes link to the
next timeline event. After `init`, `relink_all()` wires every timeline edge once.

## Complexity

| | |
|---|---|
| Per update | `O(log N)` — constant number of LCT link/cut/query |
| Memory | `O(N)` nodes (`2N + 2` elephants + sentinels) |
| `init` | `O(N log N)` — build set and initial links |

The bucket solution in `elephants.cc` rebuilds periodically and is **`O(√N)` amortized**
per query; the LCT version is **`O(log N)`** worst-case and is the intended full-score
approach discussed on Codeforces and in Japanese IOI writeups.

## References

- [Codeforces blog — IOI 2011 Elephants (LCT?)](https://codeforces.com/blog/entry/56486)
- [Kazuma8128 — IOI2011 2日目 Dancing Elephants 解説](https://kazuma8128.hatenablog.com/entry/2018/02/18/014946)
- [oj.uz submission #55017](https://oj.uz/submission/55017) (reference implementation)

## File map

```text
init(N, L, X[])
  create nodes 2i, 2i+1; link 2i → 2i+1
  insert timeline pairs; relink_all()

update(i, new_x)
  cut + erase old (start, end); insert new; relink 6 local iterators
  return LCT::query(tree[2N])
```

Sentinel `2N` anchors the left end of the timeline. After each update,
`return LCT::query(tree[2N])` is the minimum number of steps the grader expects (same
value as the sqrt-decomposition solution in `elephants.cc` on official tests).
