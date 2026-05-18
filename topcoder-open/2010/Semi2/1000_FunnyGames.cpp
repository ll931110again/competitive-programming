#include <bits/stdc++.h>
using namespace std;

class FunnyGames {
  struct Fenwick {
    vector<long long> f;
    explicit Fenwick(int n) : f(n + 1) {}
    void add(int i, long long v) {
      for (++i; i < (int)f.size(); i += i & -i)
        f[i] += v;
    }
    long long sumPrefix(int i) const {
      long long s = 0;
      for (++i; i > 0; i -= i & -i)
        s += f[i];
      return s;
    }
    long long sumAbove(long long y, const vector<long long>& ys) const {
      int i = upper_bound(ys.begin(), ys.end(), y) - ys.begin();
      if (i >= (int)ys.size())
        return 0;
      return sumPrefix((int)ys.size() - 1) - (i ? sumPrefix(i - 1) : 0);
    }
  };

  static long long countWinningAnySize(const vector<long long>& dx, const vector<long long>& dy) {
    const int n = (int)dx.size();
    const int n1 = n / 2;
    const int n2 = n - n1;

    map<pair<long long, long long>, long long> left;
    for (int mask = 0; mask < (1 << n1); mask++) {
      long long x = 0, y = 0;
      for (int i = 0; i < n1; i++)
        if (mask & (1 << i)) {
          x += dx[i];
          y += dy[i];
        }
      left[{x, y}]++;
    }

    vector<long long> ys;
    ys.reserve(left.size());
    for (const auto& kv : left)
      ys.push_back(kv.first.second);
    sort(ys.begin(), ys.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    struct Pt {
      long long x, y;
      long long cnt;
    };
    struct Q {
      long long tx, ty;
    };

    vector<Pt> pts;
    pts.reserve(left.size());
    for (const auto& kv : left)
      pts.push_back({kv.first.first, kv.first.second, kv.second});
    sort(pts.begin(), pts.end(), [](const Pt& a, const Pt& b) { return a.x > b.x; });

    vector<Q> qs;
    qs.reserve(1 << n2);
    for (int mask = 0; mask < (1 << n2); mask++) {
      long long x = 0, y = 0;
      for (int i = 0; i < n2; i++)
        if (mask & (1 << i)) {
          x += dx[n1 + i];
          y += dy[n1 + i];
        }
      qs.push_back({-x, -y});
    }
    sort(qs.begin(), qs.end(), [](const Q& a, const Q& b) { return a.tx > b.tx; });

    Fenwick bit((int)ys.size());
    long long ans = 0;
    size_t pi = 0, qi = 0;
    while (qi < qs.size()) {
      long long curTx = qs[qi].tx;
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

  static long long countWinningSmall(const vector<long long>& dx, const vector<long long>& dy,
                                     int k) {
    const int n = (int)dx.size();
    long long ans = 0;
    function<void(int, int, long long, long long)> dfs = [&](int i, int sz, long long x,
                                                             long long y) {
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
  long long countWays(vector<int> A, vector<int> B, vector<int> C, int k) {
    const int n = (int)A.size();
    vector<long long> dx(n), dy(n);
    for (int i = 0; i < n; i++) {
      dx[i] = (long long)A[i] - B[i];
      dy[i] = (long long)A[i] - C[i];
    }
    return countWinningAnySize(dx, dy) - countWinningSmall(dx, dy, k);
  }
};
