// CF 13E — Holes
// https://codeforces.com/problemset/problem/13/E
//
// Sqrt decomposition with per-block jump pointers (same idea as bucket
// precomputation in IOI 2011 Elephants).

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Block {
  int l = 0;
  int r = 0;
  vector<int> a;
  vector<int> end_hole;
  vector<int> jumps;
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  vector<int> power(n + 1);
  for (int i = 1; i <= n; ++i) {
    cin >> power[i];
  }

  const int bsz = max(1, (int)sqrt(n) + 1);
  const int nb = (n + bsz - 1) / bsz;
  vector<Block> blocks(nb);

  auto rebuild = [&](int b) {
    Block& blk = blocks[b];
    blk.l = b * bsz + 1;
    blk.r = min(n, (b + 1) * bsz);
    const int len = blk.r - blk.l + 1;
    blk.a.assign(len + 1, 0);
    blk.end_hole.assign(len + 1, 0);
    blk.jumps.assign(len + 1, 0);
    for (int i = blk.l; i <= blk.r; ++i) {
      blk.a[i - blk.l + 1] = power[i];
    }
    for (int i = len; i >= 1; --i) {
      const int pos = blk.l + i - 1;
      const int dest = pos + blk.a[i];
      if (dest > n) {
        blk.end_hole[i] = pos;
        blk.jumps[i] = 1;
      } else if (dest > blk.r) {
        const int nb2 = (dest - 1) / bsz;
        const int nj = dest - blocks[nb2].l + 1;
        blk.end_hole[i] = blocks[nb2].end_hole[nj];
        blk.jumps[i] = 1 + blocks[nb2].jumps[nj];
      } else {
        const int j = dest - blk.l + 1;
        blk.end_hole[i] = blk.end_hole[j];
        blk.jumps[i] = 1 + blk.jumps[j];
      }
    }
  };

  for (int b = nb - 1; b >= 0; --b) {
    rebuild(b);
  }

  auto query = [&](int start) {
    const int b = (start - 1) / bsz;
    const Block& blk = blocks[b];
    const int idx = start - blk.l + 1;
    return pair<int, int>{blk.end_hole[idx], blk.jumps[idx]};
  };

  while (m--) {
    int type, a, b;
    cin >> type >> a;
    if (type == 0) {
      cin >> b;
      power[a] = b;
      for (int bb = (a - 1) / bsz; bb >= 0; --bb) {
        rebuild(bb);
      }
    } else {
      auto [last, cnt] = query(a);
      cout << last << ' ' << cnt << '\n';
    }
  }

  return 0;
}
