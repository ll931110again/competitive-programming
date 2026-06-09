// L – Odd Rubble
// https://open.kattis.com/problems/oddrubble

#include <bits/stdc++.h>
using namespace std;

using int64 = long long;
using i128 = __int128_t;

namespace {

int trailing_pow2(int64 v) {
  int t = 0;
  while (v % 2 == 0) {
    v /= 2;
    ++t;
  }
  return t;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, q;
  cin >> n >> q;
  vector<int64> a(n);
  for (int i = 0; i < n; ++i)
    cin >> a[i];

  vector<i128> cnt_leaf(n);
  i128 total = 0;
  for (int i = 0; i < n; ++i) {
    int t = trailing_pow2(a[i]);
    cnt_leaf[i] = (i128)1 << t;
    total += cnt_leaf[i];
  }

  while (q--) {
    i128 x;
    {
      unsigned long long x0;
      cin >> x0;
      x = (i128)x0;
    }
    if (x < 1 || x > total) {
      cout << -1 << '\n';
      continue;
    }
    i128 pref = 0;
    int pos = -1;
    for (int i = 0; i < n; ++i) {
      if (x <= pref + cnt_leaf[i]) {
        pos = i;
        break;
      }
      pref += cnt_leaf[i];
    }
    int64 v = a[pos];
    int t = trailing_pow2(v);
    int64 odd = v;
    for (int j = 0; j < t; ++j)
      odd /= 2;
    cout << odd << '\n';
  }
  return 0;
}
