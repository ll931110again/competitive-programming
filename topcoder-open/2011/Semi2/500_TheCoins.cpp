#include <bits/stdc++.h>
using namespace std;

class TheCoins {
  long long gcdll(long long a, long long b) {
    while (b) {
      a %= b;
      swap(a, b);
    }
    return a;
  }

  long long countParity(long long n, long long m, int parity) {
    long long total = n * m;
    long long even = ((n + 1) / 2) * ((m + 1) / 2) + (n / 2) * (m / 2);
    long long odd = total - even;
    return parity ? odd : even;
  }

public:
  long long find(int n, int m, int dr, int dc, vector<int> row, vector<int> column) {
    int k = (int)row.size();
    if ((dr + dc) % 2 == 1) {
      long long even = countParity(n, m, 0);
      long long odd = countParity(n, m, 1);
      long long pairs = min(even, odd);
      return 2 * pairs;
    }

    // same parity: use residue classes modulo g = gcd(dr, dc)
    long long g = gcdll(dr, dc);
    long long pr = dr / g;
    long long pc = dc / g;

    // count cells per residue (r mod pr, c mod pc) in [1..n]x[1..m]
    map<pair<long long, long long>, long long> cnt;
    for (long long r = 1; r <= n; r++) {
      long long rr = (r - 1) % pr;
      for (long long c = 1; c <= m; c++) {
        long long cc = (c - 1) % pc;
        cnt[{rr, cc}]++;
      }
    }

    // edges between (r,c) and (r+dr,c+dc) connect residues
    map<pair<long long, long long>, long long> partner;
    for (auto& e : cnt) {
      long long r = e.first.first;
      long long c = e.first.second;
      long long r2 = (r + pr) % pr;
      long long c2 = (c + pc) % pc;
      partner[e.first] = cnt[{r2, c2}];
    }

    long long pairs = 0;
    set<pair<long long, long long>> seen;
    for (auto& e : cnt) {
      if (seen.count(e.first))
        continue;
      long long r2 = (e.first.first + pr) % pr;
      long long c2 = (e.first.second + pc) % pc;
      auto key2 = make_pair(r2, c2);
      if (!cnt.count(key2))
        continue;
      seen.insert(e.first);
      seen.insert(key2);
      pairs += min(e.second, cnt[key2]);
    }

    long long ans = 2 * pairs;

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
      long long local = 0;
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
