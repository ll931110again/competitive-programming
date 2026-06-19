#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class TheCoins {
  i64 gcdll(i64 a, i64 b) {
    while (b) {
      a %= b;
      swap(a, b);
    }
    return a;
  }

  i64 countParity(i64 n, i64 m, int parity) {
    i64 total = n * m;
    i64 even = ((n + 1) / 2) * ((m + 1) / 2) + (n / 2) * (m / 2);
    i64 odd = total - even;
    return parity ? odd : even;
  }

public:
  i64 find(int n, int m, int dr, int dc, vector<int> row, vector<int> column) {
    int k = (int)row.size();
    if ((dr + dc) % 2 == 1) {
      i64 even = countParity(n, m, 0);
      i64 odd = countParity(n, m, 1);
      i64 pairs = min(even, odd);
      return 2 * pairs;
    }

    // same parity: use residue classes modulo g = gcd(dr, dc)
    i64 g = gcdll(dr, dc);
    i64 pr = dr / g;
    i64 pc = dc / g;

    // count cells per residue (r mod pr, c mod pc) in [1..n]x[1..m]
    map<pair<i64, i64>, i64> cnt;
    for (i64 r = 1; r <= n; r++) {
      i64 rr = (r - 1) % pr;
      for (i64 c = 1; c <= m; c++) {
        i64 cc = (c - 1) % pc;
        cnt[{rr, cc}]++;
      }
    }

    // edges between (r,c) and (r+dr,c+dc) connect residues
    map<pair<i64, i64>, i64> partner;
    for (auto& e : cnt) {
      i64 r = e.first.first;
      i64 c = e.first.second;
      i64 r2 = (r + pr) % pr;
      i64 c2 = (c + pc) % pc;
      partner[e.first] = cnt[{r2, c2}];
    }

    i64 pairs = 0;
    set<pair<i64, i64>> seen;
    for (auto& e : cnt) {
      if (seen.count(e.first))
        continue;
      i64 r2 = (e.first.first + pr) % pr;
      i64 c2 = (e.first.second + pc) % pc;
      auto key2 = make_pair(r2, c2);
      if (!cnt.count(key2))
        continue;
      seen.insert(e.first);
      seen.insert(key2);
      pairs += min(e.second, cnt[key2]);
    }

    i64 ans = 2 * pairs;

    // subtract blocked cells (small k)
    set<pair<int, int>> blocked;
    for (int i = 0; i < k; i++)
      blocked.insert({row[i], column[i]});

    // recompute with obstacles by local adjustment on small board if needed
    if (k > 0 && n <= 200 && m <= 200) {
      vector<vector<char>> occ(n + 1, vector<char>(m + 1, 0));
      for (auto p : blocked)
        occ[p.first][p.second] = 1;
      vector<vector<char>> used(n + 1, vector<char>(m + 1, 0));
      i64 local = 0;
      for (int r = 1; r <= n; r++) {
        for (int c = 1; c <= m; c++) {
          if (occ[r][c] || used[r][c])
            continue;
          int r2 = r + dr, c2 = c + dc;
          if (r2 >= 1 && r2 <= n && c2 >= 1 && c2 <= m && !occ[r2][c2] && !used[r2][c2]) {
            used[r][c] = used[r2][c2] = 1;
            local += 2;
          }
        }
      }
      return local;
    }

    return ans;
  }
};
