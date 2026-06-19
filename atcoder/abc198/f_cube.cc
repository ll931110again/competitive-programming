// ABC198 F - Cube
// https://atcoder.jp/contests/abc198/tasks/abc198_f
//
// Burnside: answer = (1/24) * sum over cube rotations of "assignments fixed by rotation".
// Each rotation partitions faces into cycles; fixed assignments = positive solutions to
// sum (cycle_len * a_i) = S. That count as a function of S is a linear recurrence; for each
// of the 5 rotation types, generate a prefix, Berlekamp–Massey, evaluate at S.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int MOD = 998244353;
constexpr int INV24 = 291154603; // 24^{-1} mod MOD
constexpr int MAX_PREFIX = 600;

template <int P> struct ModInt {
  int x;
  ModInt(i64 v = 0) : x(int((v % P + P) % P)) {}
  ModInt& operator+=(ModInt o) {
    x += o.x;
    if (x >= P)
      x -= P;
    return *this;
  }
  ModInt& operator-=(ModInt o) {
    x -= o.x;
    if (x < 0)
      x += P;
    return *this;
  }
  ModInt& operator*=(ModInt o) {
    x = int((i64)x * o.x % P);
    return *this;
  }
  friend ModInt operator+(ModInt a, ModInt b) {
    return a += b;
  }
  friend ModInt operator-(ModInt a, ModInt b) {
    return a -= b;
  }
  friend ModInt operator*(ModInt a, ModInt b) {
    return a *= b;
  }
  friend ModInt operator/(ModInt a, ModInt b) {
    return a * mod_pow(b, P - 2);
  }
  bool operator==(ModInt o) const {
    return x == o.x;
  }
  bool operator!=(ModInt o) const {
    return x != o.x;
  }
};

template <int P> ModInt<P> mod_pow(ModInt<P> a, i64 e) {
  ModInt<P> r(1);
  while (e > 0) {
    if (e & 1)
      r *= a;
    a *= a;
    e >>= 1;
  }
  return r;
}

template <int P> vector<ModInt<P>> berlekamp_massey(const vector<ModInt<P>>& s) {
  const int n = (int)s.size();
  vector<ModInt<P>> C(n), B(n);
  C[0] = B[0] = ModInt<P>(1);
  ModInt<P> b(1);
  int L = 0;
  for (int i = 0, m = 1; i < n; ++i) {
    ModInt<P> d = s[i];
    for (int j = 1; j <= L; ++j)
      d += C[j] * s[i - j];
    if (d == ModInt<P>(0)) {
      ++m;
      continue;
    }
    vector<ModInt<P>> T = C;
    ModInt<P> coef = d / b;
    for (int j = m; j < n; ++j)
      C[j] = C[j] - coef * B[j - m];
    if (2 * L > i) {
      ++m;
      continue;
    }
    L = i + 1 - L;
    B = move(T);
    b = d;
    m = 1;
  }
  vector<ModInt<P>> res(L);
  for (int j = 0; j < L; ++j)
    res[j] = ModInt<P>(0) - C[j + 1];
  return res;
}

template <int P>
ModInt<P> kth_linear_recurrence(vector<ModInt<P>> init, const vector<ModInt<P>>& trans, i64 k) {
  const int n = (int)trans.size();
  if (k < n)
    return init[(int)k];

  using M = vector<vector<ModInt<P>>>;
  M base(n, vector<ModInt<P>>(n));
  for (int i = 0; i < n; ++i)
    base[0][i] = trans[i];
  for (int i = 1; i < n; ++i)
    base[i][i - 1] = ModInt<P>(1);

  auto mat_mul = [&](const M& A, const M& B) {
    M C(n, vector<ModInt<P>>(n));
    for (int i = 0; i < n; ++i)
      for (int j = 0; j < n; ++j)
        for (int t = 0; t < n; ++t)
          C[i][j] += A[i][t] * B[t][j];
    return C;
  };

  M res(n, vector<ModInt<P>>(n));
  for (int i = 0; i < n; ++i)
    res[i][i] = ModInt<P>(1);
  i64 e = k - n + 1;
  M pow_m = base;
  while (e > 0) {
    if (e & 1)
      res = mat_mul(res, pow_m);
    pow_m = mat_mul(pow_m, pow_m);
    e >>= 1;
  }

  ModInt<P> ans(0);
  for (int i = 0; i < n; ++i)
    ans += res[0][i] * init[n - 1 - i];
  return ans;
}

using Mint = ModInt<MOD>;

Mint count_fixed(i64 S, const vector<int>& cycles) {
  const int k = (int)cycles.size();
  const int cap = (int)min<i64>(S, MAX_PREFIX + 50);
  vector<Mint> dp(cap + 1), ndp(cap + 1);
  dp[0] = Mint(1);
  for (int ci : cycles) {
    fill(ndp.begin(), ndp.end(), Mint(0));
    for (int r = 0; r <= cap; ++r) {
      if (dp[r] == Mint(0))
        continue;
      for (int a = 1, nr = r + ci; nr <= cap; ++a, nr += ci) {
        ndp[nr] += dp[r];
      }
    }
    dp.swap(ndp);
  }
  return S <= cap ? dp[(int)S] : Mint(0);
}

Mint eval_type(i64 S, const vector<int>& cycles) {
  if (S <= MAX_PREFIX)
    return count_fixed(S, cycles);
  vector<Mint> seq;
  seq.reserve(MAX_PREFIX);
  for (int s = 0; s < MAX_PREFIX; ++s) {
    seq.push_back(count_fixed(s, cycles));
  }
  const vector<Mint> rec = berlekamp_massey(seq);
  return kth_linear_recurrence(seq, rec, S);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 S;
  cin >> S;

  const vector<int> counts = {1, 6, 3, 8, 6};
  const vector<vector<int>> types = {
      {1, 1, 1, 1, 1, 1}, {4, 1, 1}, {2, 2, 1, 1}, {3, 3}, {2, 2, 2}};

  Mint ans(0);
  for (int i = 0; i < 5; ++i) {
    ans += Mint(counts[i]) * eval_type(S, types[i]);
  }
  ans = ans * Mint(INV24);
  cout << ans.x << '\n';
  return 0;
}
