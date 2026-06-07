// CF 455D — Serega and Fun
// https://codeforces.com/contest/455/problem/D
//
// Sqrt decomposition with deques + per-block frequency counts.
// Same "block + rebuild" family as IOI 2011 Elephants (sqrt solution).

#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <deque>
#include <iostream>
#include <vector>

using namespace std;

struct Block {
  deque<int> vals;
  vector<int> cnt;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<int> a(n + 1);
  for (int i = 1; i <= n; ++i) {
    cin >> a[i];
  }

  const int bsz = max(1, (int)sqrt(n) + 1);
  const int nb = (n + bsz - 1) / bsz;
  vector<Block> blocks(nb);

  auto rebuild = [&]() {
    for (int b = 0; b < nb; ++b) {
      blocks[b].vals.clear();
      blocks[b].cnt.assign(n + 1, 0);
      const int l = b * bsz + 1;
      const int r = min(n, (b + 1) * bsz);
      for (int i = l; i <= r; ++i) {
        blocks[b].vals.push_back(a[i]);
        ++blocks[b].cnt[a[i]];
      }
    }
  };
  rebuild();

  auto locate = [&](int pos, int& b, int& idx) {
    b = (pos - 1) / bsz;
    idx = pos - b * bsz - 1;
  };

  auto shift = [&](int l, int r) {
    int bl, br, il, ir;
    locate(l, bl, il);
    locate(r, br, ir);

    if (bl == br) {
      int x = blocks[bl].vals[ir];
      blocks[bl].vals.erase(blocks[bl].vals.begin() + ir);
      blocks[bl].vals.insert(blocks[bl].vals.begin() + il, x);
      return;
    }

    int x = blocks[br].vals[ir];
    blocks[br].vals.pop_back();
    --blocks[br].cnt[x];

    for (int b = br - 1; b > bl; --b) {
      int y = blocks[b].vals.back();
      blocks[b].vals.pop_back();
      --blocks[b].cnt[y];
      blocks[b].vals.push_front(x);
      ++blocks[b].cnt[x];
      x = y;
    }

    int y = blocks[bl].vals.back();
    blocks[bl].vals.pop_back();
    --blocks[bl].cnt[y];
    blocks[bl].vals.insert(blocks[bl].vals.begin() + il, x);
    ++blocks[bl].cnt[x];

    blocks[br].vals.push_front(y);
    ++blocks[br].cnt[y];
  };

  auto count_range = [&](int l, int r, int k) {
    int bl, br, il, ir;
    locate(l, bl, il);
    locate(r, br, ir);

    int ans = 0;
    if (bl == br) {
      for (int i = il; i <= ir; ++i) {
        ans += blocks[bl].vals[i] == k;
      }
      return ans;
    }

    for (int i = il; i < (int)blocks[bl].vals.size(); ++i) {
      ans += blocks[bl].vals[i] == k;
    }
    for (int b = bl + 1; b < br; ++b) {
      ans += blocks[b].cnt[k];
    }
    for (int i = 0; i <= ir; ++i) {
      ans += blocks[br].vals[i] == k;
    }
    return ans;
  };

  int q;
  cin >> q;
  int last = 0;
  int updates = 0;
  while (q--) {
    int t, li, ri;
    cin >> t >> li >> ri;
    int l = ((li + last - 1) % n) + 1;
    int r = ((ri + last - 1) % n) + 1;
    if (l > r) {
      swap(l, r);
    }

    if (t == 1) {
      shift(l, r);
      if (++updates == bsz) {
        for (int i = 1; i <= n; ++i) {
          int b = (i - 1) / bsz;
          int idx = i - b * bsz - 1;
          a[i] = blocks[b].vals[idx];
        }
        rebuild();
        updates = 0;
      }
    } else {
      int ki;
      cin >> ki;
      int k = ((ki + last - 1) % n) + 1;
      last = count_range(l, r, k);
      cout << last << '\n';
    }
  }

  return 0;
}
