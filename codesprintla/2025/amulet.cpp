// A – The Amulet
// https://open.kattis.com/problems/amulet
//
// Rolling hash gives distinct row/column counts per hash. Candidate hashes have
// row count x and column count y. Lexicographically smallest valid code is the
// min actual substring among horizontal windows with that hash (not merely the
// first occurrence stored per hash). Single exact verification at the end; rare
// hash collisions handled by checking deduped candidates in sorted order.

#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

namespace {

const ull B1 = 911382323ULL;
const ull B2 = 972663749ULL;

struct H2 {
  ull h1, h2;
  bool operator==(const H2& o) const {
    return h1 == o.h1 && h2 == o.h2;
  }
};

struct H2Hash {
  size_t operator()(const H2& x) const {
    return x.h1 ^ (x.h2 + 0x9e3779b97f4a7c15ULL + (x.h1 << 6) + (x.h1 >> 2));
  }
};

inline int code(char ch) {
  return (int)(unsigned char)ch + 1;
}

int count_row_substr(const vector<string>& g, int R, int C, int L, const string& t) {
  if ((int)t.size() != L)
    return -1;
  int cnt = 0;
  for (int r = 0; r < R; ++r) {
    const string& s = g[r];
    for (int c = 0; c + L <= C; ++c) {
      if (s.compare(c, L, t) == 0) {
        ++cnt;
        break;
      }
    }
  }
  return cnt;
}

int count_col_substr(const vector<string>& g, int R, int C, int L, const string& t) {
  if ((int)t.size() != L)
    return -1;
  int cnt = 0;
  for (int c = 0; c < C; ++c) {
    for (int r = 0; r + L <= R; ++r) {
      bool ok = true;
      for (int k = 0; k < L; ++k) {
        if (g[r + k][c] != t[k]) {
          ok = false;
          break;
        }
      }
      if (ok) {
        ++cnt;
        break;
      }
    }
  }
  return cnt;
}

bool ok_counts(const vector<string>& g, int R, int C, int L, int x, int y, const string& sub) {
  return count_row_substr(g, R, C, L, sub) == x && count_col_substr(g, R, C, L, sub) == y;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int R, C, x, y;
  if (!(cin >> R >> C >> x >> y))
    return 0;
  vector<string> g(R);
  for (int i = 0; i < R; ++i)
    cin >> g[i];

  int max_l = min(R, C);
  vector<ull> pow1(max_l + 2), pow2(max_l + 2);
  pow1[0] = pow2[0] = 1;
  for (int i = 1; i <= max_l + 1; ++i) {
    pow1[i] = pow1[i - 1] * B1;
    pow2[i] = pow2[i - 1] * B2;
  }

  for (int L = max_l; L >= 1; --L) {
    unordered_map<H2, int, H2Hash> row_cover;
    unordered_map<H2, int, H2Hash> last_row;

    auto touch_row = [&](const H2& key, int r) {
      int tag = r + 1;
      auto it = last_row.find(key);
      if (it == last_row.end() || it->second != tag) {
        row_cover[key]++;
        last_row[key] = tag;
      }
    };

    for (int r = 0; r < R; ++r) {
      const string& s = g[r];
      if ((int)s.size() < L)
        continue;
      vector<ull> p1(C + 1), p2(C + 1);
      for (int i = 0; i < C; ++i) {
        int v = code(s[i]);
        p1[i + 1] = p1[i] * B1 + v;
        p2[i + 1] = p2[i] * B2 + v;
      }
      for (int c = 0; c + L <= C; ++c) {
        ull h1 = p1[c + L] - p1[c] * pow1[L];
        ull h2 = p2[c + L] - p2[c] * pow2[L];
        touch_row(H2{h1, h2}, r);
      }
    }

    unordered_map<H2, int, H2Hash> col_cover;
    unordered_map<H2, int, H2Hash> last_col;

    auto touch_col = [&](const H2& key, int c) {
      int tag = c + 1;
      auto it = last_col.find(key);
      if (it == last_col.end() || it->second != tag) {
        col_cover[key]++;
        last_col[key] = tag;
      }
    };

    for (int c = 0; c < C; ++c) {
      vector<ull> p1(R + 1), p2(R + 1);
      for (int i = 0; i < R; ++i) {
        int v = code(g[i][c]);
        p1[i + 1] = p1[i] * B1 + v;
        p2[i + 1] = p2[i] * B2 + v;
      }
      for (int r = 0; r + L <= R; ++r) {
        ull h1 = p1[r + L] - p1[r] * pow1[L];
        ull h2 = p2[r + L] - p2[r] * pow2[L];
        touch_col(H2{h1, h2}, c);
      }
    }

    unordered_set<H2, H2Hash> cand;
    for (const auto& kv : row_cover) {
      if (kv.second != x)
        continue;
      auto itc = col_cover.find(kv.first);
      if (itc != col_cover.end() && itc->second == y)
        cand.insert(kv.first);
    }

    if (cand.empty())
      continue;

    string best;
    bool got = false;
    for (int r = 0; r < R; ++r) {
      const string& s = g[r];
      if ((int)s.size() < L)
        continue;
      vector<ull> p1(C + 1), p2(C + 1);
      for (int i = 0; i < C; ++i) {
        int v = code(s[i]);
        p1[i + 1] = p1[i] * B1 + v;
        p2[i + 1] = p2[i] * B2 + v;
      }
      for (int c = 0; c + L <= C; ++c) {
        ull h1 = p1[c + L] - p1[c] * pow1[L];
        ull h2 = p2[c + L] - p2[c] * pow2[L];
        H2 key{h1, h2};
        if (!cand.count(key))
          continue;
        string sub = s.substr(c, L);
        if (!got || sub < best) {
          best = move(sub);
          got = true;
        }
      }
    }

    if (got && ok_counts(g, R, C, L, x, y, best)) {
      cout << best << '\n';
      return 0;
    }

    // Hash collision: aggregated counts matched x,y but best substring did not.
    vector<string> pool;
    unordered_set<string> seen;
    pool.reserve(R * max(0, C - L + 1));
    for (int r = 0; r < R; ++r) {
      const string& s = g[r];
      if ((int)s.size() < L)
        continue;
      vector<ull> p1(C + 1), p2(C + 1);
      for (int i = 0; i < C; ++i) {
        int v = code(s[i]);
        p1[i + 1] = p1[i] * B1 + v;
        p2[i + 1] = p2[i] * B2 + v;
      }
      for (int c = 0; c + L <= C; ++c) {
        ull h1 = p1[c + L] - p1[c] * pow1[L];
        ull h2 = p2[c + L] - p2[c] * pow2[L];
        if (!cand.count(H2{h1, h2}))
          continue;
        string sub = s.substr(c, L);
        if (seen.insert(sub).second)
          pool.push_back(move(sub));
      }
    }
    sort(pool.begin(), pool.end());
    for (const string& sub : pool) {
      if (ok_counts(g, R, C, L, x, y, sub)) {
        cout << sub << '\n';
        return 0;
      }
    }
  }

  cout << -1 << '\n';
  return 0;
}
