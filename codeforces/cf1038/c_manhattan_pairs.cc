
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int T, n;
struct Pts {
  int x, y;
  int idx;
};

pts a[k_max_n];
int flags[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i].x >> a[i].y;
      a[i].idx = i;
      flags[i] = 0;
    }

    sort(a, a + n, [&](const auto& u, const auto& v) { return u.x < v.x; });

    for (int i = 0; i < n; i++) {
      if (i < n / 2) {
        flags[a[i].idx] |= 0;
      } else {
        flags[a[i].idx] |= 1;
      }
    }

    sort(a, a + n, [&](const auto& u, const auto& v) { return u.y < v.y; });

    for (int i = 0; i < n; i++) {
      if (i < n / 2) {
        flags[a[i].idx] |= 0;
      } else {
        flags[a[i].idx] |= 2;
      }
    }

    // matching (Xl, Yl) with (Xr, Yr)
    vector<pair<int, int>> ans;
    vector<int> ll, lr, rl, rr;
    for (int i = 0; i < n; i++) {
      if (flags[i] == 0) {
        ll.push_back(i);
      } else if (flags[i] == 1) {
        lr.push_back(i);
      } else if (flags[i] == 2) {
        rl.push_back(i);
      } else {
        rr.push_back(i);
      }
    }

    for (int i = 0; i < ll.size(); i++) {
      ans.push_back({ll[i], rr[i]});
    }

    for (int i = 0; i < lr.size(); i++) {
      ans.push_back({lr[i], rl[i]});
    }

    for (auto x : ans) {
      cout << x.first + 1 << ' ' << x.second + 1 << endl;
    }
  }
  return 0;
}
