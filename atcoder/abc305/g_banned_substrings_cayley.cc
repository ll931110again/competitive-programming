// ABC305 G - Banned Substrings (Cayley–Hamilton / companion matrix route)
// https://atcoder.jp/contests/abc305/tasks/abc305_g
//
// Automaton DP on the last up-to-6 characters. Transition matrix T has size m ≤ 64;
// answer vector v gives counts after n steps. Cayley–Hamilton: T^n = polynomial in T of
// degree < m, implemented here as binary matrix exponentiation on v.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int MOD = 998244353;
constexpr int LIM = 6;

i64 mod_pow(i64 a, i64 e) {
  i64 r = 1;
  a %= MOD;
  while (e > 0) {
    if (e & 1)
      r = r * a % MOD;
    a = a * a % MOD;
    e >>= 1;
  }
  return r;
}

int encode(const string& s) {
  int mask = 0;
  for (char c : s)
    mask = mask * 2 + (c == 'b');
  return mask;
}

string decode(int mask, int len) {
  string s(len, 'a');
  for (int i = len - 1; i >= 0; --i) {
    s[i] = (mask & 1) ? 'b' : 'a';
    mask >>= 1;
  }
  return s;
}

bool contains_banned(const string& t, const vector<string>& bad) {
  for (const string& s : bad)
    if (t.find(s) != string::npos)
      return true;
  return false;
}

struct Mat {
  int n;
  vector<vector<int>> a;

  Mat(int n, bool ident = false) : n(n), a(n, vector<int>(n, 0)) {
    if (ident)
      for (int i = 0; i < n; ++i)
        a[i][i] = 1;
  }

  Mat mul(const Mat& x, const Mat& y) {
    Mat z(x.n);
    for (int i = 0; i < x.n; ++i)
      for (int k = 0; k < x.n; ++k)
        if (x.a[i][k])
          for (int j = 0; j < x.n; ++j)
            z.a[i][j] = (z.a[i][j] + (i64)x.a[i][k] * y.a[k][j]) % MOD;
    return z;
  }

  Mat pow(Mat base, i64 e) {
    Mat res(base.n, true);
    while (e > 0) {
      if (e & 1)
        res = mul(res, base);
      base = mul(base, base);
      e >>= 1;
    }
    return res;
  }

  vector<int> apply(const vector<int>& v) const {
    vector<int> r(n);
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        r[i] = (r[i] + (i64)a[i][j] * v[j]) % MOD;
    return r;
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 N;
  int M;
  cin >> N >> M;
  vector<string> bad(M);
  for (int i = 0; i < M; ++i)
    cin >> bad[i];

  vector<int> states;
  vector<int> len_of;
  for (int len = 0; len <= LIM; ++len) {
    const int tot = 1 << len;
    for (int mask = 0; mask < tot; ++mask) {
      string t = decode(mask, len);
      if (!contains_banned(t, bad)) {
        states.push_back(mask);
        len_of.push_back(len);
      }
    }
  }

  const int m = (int)states.size();
  map<pair<int, int>, int> id;
  for (int i = 0; i < m; ++i)
    id[{states[i], len_of[i]}] = i;

  Mat trans(m);
  for (int i = 0; i < m; ++i) {
    int mask = states[i];
    int len = len_of[i];
    for (char c : {'a', 'b'}) {
      int nmask, nlen;
      if (len < LIM) {
        nlen = len + 1;
        nmask = mask * 2 + (c == 'b');
      } else {
        nlen = LIM;
        nmask = (mask * 2 + (c == 'b')) & ((1 << LIM) - 1);
      }
      string t = decode(nmask, nlen);
      if (contains_banned(t, bad))
        continue;
      int j = id.at({nmask, nlen});
      trans.a[j][i] = (trans.a[j][i] + 1) % MOD;
    }
  }

  vector<int> vec(m);
  vec[id.at({0, 0})] = 1;

  if (N > 0)
    vec = Mat::pow(trans, N).apply(vec);

  i64 ans = 0;
  for (int x : vec)
    ans += x;
  cout << ans % MOD << '\n';
  return 0;
}
