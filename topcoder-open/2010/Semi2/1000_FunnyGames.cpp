#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class FunnyGames {
  struct Fenwick {
    vector<i64> f;
    explicit Fenwick(int n) : f(n + 1) {}
    void add(int i, i64 v) {
      for (++i; i < (int)f.size(); i += i & -i)
        f[i] += v;
    }
    i64 sumPrefix(int i) const {
      i64 s = 0;
      for (++i; i > 0; i -= i & -i)
        s += f[i];
      return s;
    }
    i64 sumAbove(i64 y, const vector<i64>& ys) const {
      int i = upper_bound(ys.begin(), ys.end(), y) - ys.begin();
      if (i >= (int)ys.size())
        return 0;
      return sumPrefix((int)ys.size() - 1) - (i ? sumPrefix(i - 1) : 0);
    }
  };

  static i64 countWinningAnySize(const vector<i64>& dx, const vector<i64>& dy) {
    const int n = (int)dx.size();
    const int n1 = n / 2;
    const int n2 = n - n1;

    map<pair<i64, i64>, i64> left;
    for (int mask = 0; mask < (1 << n1); mask++) {
      i64 x = 0, y = 0;
      for (int i = 0; i < n1; i++)
        if (mask & (1 << i)) {
          x += dx[i];
          y += dy[i];
        }
      left[{x, y}]++;
    }

    vector<i64> ys;
    ys.reserve(left.size());
    for (const auto& kv : left)
      ys.push_back(kv.first.second);
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    struct Pt {
      i64 x, y;
      i64 cnt;
    };
    struct Q {
      i64 tx, ty;
    };

    vector<Pt> pts;
    pts.reserve(left.size());
    for (const auto& kv : left)
      pts.push_back({kv.first.first, kv.first.second, kv.second});
    sort(pts.begin(), pts.end(), [](const Pt& a, const Pt& b) { return a.x > b.x; });

    vector<Q> qs;
    qs.reserve(1 << n2);
    for (int mask = 0; mask < (1 << n2); mask++) {
      i64 x = 0, y = 0;
      for (int i = 0; i < n2; i++)
        if (mask & (1 << i)) {
          x += dx[n1 + i];
          y += dy[n1 + i];
        }
      qs.push_back({-x, -y});
    }
    sort(qs.begin(), qs.end(), [](const Q& a, const Q& b) { return a.tx > b.tx; });

    Fenwick bit((int)ys.size());
    i64 ans = 0;
    size_t pi = 0, qi = 0;
    while (qi < qs.size()) {
      i64 curTx = qs[qi].tx;
      while (pi < pts.size() && pts[pi].x > curTx) {
        int id = lower_bound(ys.begin(), ys.end(), pts[pi].y) - ys.begin();
        bit.add(id, pts[pi].cnt);
        pi++;
      }
      while (qi < qs.size() && qs[qi].tx == curTx) {
        ans += bit.sumAbove(qs[qi].ty, ys);
        qi++;
      }
    }
    return ans;
  }

  static i64 countWinningSmall(const vector<i64>& dx, const vector<i64>& dy, int k) {
    const int n = (int)dx.size();
    i64 ans = 0;
    function<void(int, int, i64, i64)> dfs = [&](int i, int sz, i64 x, i64 y) {
      if (i == n) {
        if (sz < k && x > 0 && y > 0)
          ans++;
        return;
      }
      dfs(i + 1, sz, x, y);
      dfs(i + 1, sz + 1, x + dx[i], y + dy[i]);
    };
    dfs(0, 0, 0, 0);
    return ans;
  }

public:
  i64 countWays(vector<int> A, vector<int> B, vector<int> C, int k) {
    const int n = (int)A.size();
    vector<i64> dx(n), dy(n);
    for (int i = 0; i < n; i++) {
      dx[i] = (i64)A[i] - B[i];
      dy[i] = (i64)A[i] - C[i];
    }
    return countWinningAnySize(dx, dy) - countWinningSmall(dx, dy, k);
  }
};
