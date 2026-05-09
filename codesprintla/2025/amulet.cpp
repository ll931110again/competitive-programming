// A – The Amulet
// https://codesprintla25.kattis.com/problems/amulet

#include <bits/stdc++.h>
using namespace std;

using ull = unsigned long long;

static const ull B1 = 911382323ULL;
static const ull B2 = 972663749ULL;

struct H2 {
  ull h1, h2;
  bool operator==(const H2 &o) const { return h1 == o.h1 && h2 == o.h2; }
};

struct H2Hash {
  size_t operator()(const H2 &x) const {
    return x.h1 ^ (x.h2 + 0x9e3779b97f4a7c15ULL + (x.h1 << 6) + (x.h1 >> 2));
  }
};

static inline int code(char ch) { return (int)(unsigned char)ch + 1; }

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int R, C, x, y;
  if (!(cin >> R >> C >> x >> y))
    return 0;
  vector<string> g(R);
  for (int i = 0; i < R; ++i)
    cin >> g[i];

  int maxL = min(R, C);
  vector<ull> pow1(maxL + 2), pow2(maxL + 2);
  pow1[0] = pow2[0] = 1;
  for (int i = 1; i <= maxL + 1; ++i) {
    pow1[i] = pow1[i - 1] * B1;
    pow2[i] = pow2[i - 1] * B2;
  }

  for (int L = maxL; L >= 1; --L) {
    unordered_map<H2, int, H2Hash> row_cover;
    unordered_map<H2, int, H2Hash> last_row;
    unordered_map<H2, string, H2Hash> rep;

    auto touch_row = [&](const H2 &key, int r, const string &sub) {
      int tag = r + 1;
      auto it = last_row.find(key);
      if (it == last_row.end() || it->second != tag) {
        row_cover[key]++;
        last_row[key] = tag;
      }
      if (!rep.count(key))
        rep[key] = sub;
    };

    // Horizontal substrings of length L
    for (int r = 0; r < R; ++r) {
      const string &s = g[r];
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
        touch_row(key, r, s.substr(c, L));
      }
    }

    unordered_map<H2, int, H2Hash> col_cover;
    unordered_map<H2, int, H2Hash> last_col;

    auto touch_col = [&](const H2 &key, int c) {
      int tag = c + 1;
      auto it = last_col.find(key);
      if (it == last_col.end() || it->second != tag) {
        col_cover[key]++;
        last_col[key] = tag;
      }
    };

    // Vertical substrings of length L
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
        H2 key{h1, h2};
        touch_col(key, c);
      }
    }

    string best;
    bool found = false;
    for (const auto &kv : row_cover) {
      const H2 &key = kv.first;
      if (kv.second != x)
        continue;
      auto itc = col_cover.find(key);
      if (itc == col_cover.end() || itc->second != y)
        continue;
      const string &s = rep.at(key);
      if (!found || s < best) {
        best = s;
        found = true;
      }
    }
    if (found) {
      cout << best << '\n';
      return 0;
    }
  }

  cout << -1 << '\n';
  return 0;
}
