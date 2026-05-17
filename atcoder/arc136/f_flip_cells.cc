// ARC136 F - Flip Cells: https://atcoder.jp/contests/arc136/tasks/arc136_f
//
// g_n = (n!/H^n) · Σ_{t1+…+tH=n} Π_i (T^{ti})_{Ai,Bi}/ti!;  h same with Bi=Ai;  F·H=G.
// f = deconv(g,h);  E = F'(1) from OGF P/Q.  T^k entries share order-(W+1) recurrence
// (Cayley–Hamilton); only T^0..T^W are matrix-multipled.  Row series product via NTT.

#include <bits/stdc++.h>
using namespace std;

static constexpr int MOD = 998244353;
static constexpr int G = 3;

static int mod_pow_int(int a, long long e) {
  long long r = 1;
  a %= MOD;
  while (e > 0) {
    if (e & 1)
      r = r * a % MOD;
    a = (int)((long long)a * a % MOD);
    e >>= 1;
  }
  return (int)r;
}

template <int P> struct ModInt;
template <int P> ModInt<P> mod_pow(ModInt<P> a, long long e);

template <int P> struct ModInt {
  int x;
  ModInt(long long v = 0) : x(int((v % P + P) % P)) {}
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
    x = int((long long)x * o.x % P);
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
    return a * mod_pow(b, (long long)P - 2);
  }
  bool operator==(ModInt o) const {
    return x == o.x;
  }
  bool operator!=(ModInt o) const {
    return x != o.x;
  }
};

template <int P> ModInt<P> mod_pow(ModInt<P> a, long long e) {
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
    B = std::move(T);
    b = d;
    m = 1;
  }
  vector<ModInt<P>> res(L);
  for (int j = 0; j < L; ++j)
    res[j] = ModInt<P>(0) - C[j + 1];
  return res;
}

using MI = ModInt<MOD>;
using Mat = vector<vector<MI>>;

static void ntt(vector<int>& a, bool invert) {
  const int n = (int)a.size();
  static vector<int> rev;
  if ((int)rev.size() != n) {
    const int lg = __builtin_ctz(n);
    rev.assign(n, 0);
    for (int i = 0; i < n; ++i)
      rev[i] = (rev[i >> 1] >> 1) | ((i & 1) << (lg - 1));
  }
  for (int i = 0; i < n; ++i)
    if (i < rev[i])
      swap(a[i], a[rev[i]]);

  for (int len = 1; len < n; len <<= 1) {
    int wlen = mod_pow_int(G, (MOD - 1) / (len << 1));
    if (invert)
      wlen = mod_pow_int(wlen, MOD - 2);
    for (int i = 0; i < n; i += len << 1) {
      int w = 1;
      for (int j = 0; j < len; ++j) {
        const int u = a[i + j];
        const int v = (int)((long long)a[i + j + len] * w % MOD);
        int x = u + v;
        if (x >= MOD)
          x -= MOD;
        int y = u - v;
        if (y < 0)
          y += MOD;
        a[i + j] = x;
        a[i + j + len] = y;
        w = (int)((long long)w * wlen % MOD);
      }
    }
  }
  if (invert) {
    const int inv_n = mod_pow_int(n, MOD - 2);
    for (int& x : a)
      x = (int)((long long)x * inv_n % MOD);
  }
}

static vector<int> poly_conv(const vector<int>& a, const vector<int>& b, int lim) {
  if (a.empty() || b.empty())
    return {};
  int need = min(lim, (int)a.size() + (int)b.size() - 1);
  int n = 1;
  while (n < (int)a.size() + (int)b.size() - 1)
    n <<= 1;
  vector<int> fa(a.begin(), a.end()), fb(b.begin(), b.end());
  fa.resize(n);
  fb.resize(n);
  ntt(fa, false);
  ntt(fb, false);
  for (int i = 0; i < n; ++i)
    fa[i] = (int)((long long)fa[i] * fb[i] % MOD);
  ntt(fa, true);
  fa.resize(need);
  return fa;
}

static vector<vector<MI>> mat_mul(const vector<vector<MI>>& a, const vector<vector<MI>>& b) {
  const int n = (int)a.size();
  vector<vector<MI>> c(n, vector<MI>(n));
  for (int i = 0; i < n; ++i)
    for (int k = 0; k < n; ++k)
      if (a[i][k] != MI(0))
        for (int j = 0; j < n; ++j)
          c[i][j] = c[i][j] + a[i][k] * b[k][j];
  return c;
}

static vector<MI> extend_entry(const vector<Mat>& pw, int from, int to, const vector<MI>& trans,
                               int D) {
  const int L = (int)trans.size();
  vector<MI> row(D);
  for (int t = 0; t < L && t < D; ++t)
    row[t] = pw[t][to][from];
  for (int t = L; t < D; ++t) {
    MI v(0);
    for (int j = 0; j < L; ++j)
      v = v + trans[j] * row[t - j - 1];
    row[t] = v;
  }
  return row;
}

static MI eval_poly(const vector<MI>& p, MI x) {
  MI r(0);
  for (int i = (int)p.size() - 1; i >= 0; --i)
    r = r * x + p[i];
  return r;
}

static vector<MI> deriv_poly(const vector<MI>& p) {
  if ((int)p.size() <= 1)
    return {};
  vector<MI> q(p.size() - 1);
  for (int i = 1; i < (int)p.size(); ++i)
    q[i - 1] = p[i] * MI(i);
  return q;
}

static MI expected_from_recurrence(const vector<MI>& f, const vector<MI>& trans) {
  const int L = (int)trans.size();
  vector<MI> den(L + 1);
  den[0] = MI(1);
  for (int i = 0; i < L; ++i)
    den[i + 1] = MI(0) - trans[i];

  vector<MI> num(L);
  for (int i = 0; i < L; ++i)
    for (int j = 0; j <= i; ++j)
      num[i] = num[i] + f[j] * den[i - j];

  const MI Q1 = eval_poly(den, MI(1));
  if (Q1 == MI(0))
    return MI(0);
  const MI P1 = eval_poly(num, MI(1));
  const MI Pp = eval_poly(deriv_poly(num), MI(1));
  const MI Qp = eval_poly(deriv_poly(den), MI(1));
  return (Pp * Q1 - P1 * Qp) / (Q1 * Q1);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int H, W;
  cin >> H >> W;
  vector<string> rows(H);
  for (int i = 0; i < H; ++i)
    cin >> rows[i];
  vector<int> A(H);
  for (int i = 0; i < H; ++i)
    cin >> A[i];
  vector<int> B(H);
  for (int i = 0; i < H; ++i)
    B[i] = (int)count(rows[i].begin(), rows[i].end(), '1');

  for (int i = 0; i < H; ++i) {
    if (A[i] != B[i])
      break;
    if (i + 1 == H) {
      cout << 0 << '\n';
      return 0;
    }
  }

  const int n = W + 1;
  Mat T(n, vector<MI>(n));
  for (int j = 0; j <= W; ++j) {
    if (j > 0)
      T[j - 1][j] = MI(j) / MI(W);
    if (j < W)
      T[j + 1][j] = MI(W - j) / MI(W);
  }

  const int D = 2 * H * W + 50;
  vector<Mat> pw(n, Mat(n, vector<MI>(n)));
  for (int i = 0; i < n; ++i)
    pw[0][i][i] = MI(1);
  for (int t = 1; t < n; ++t)
    pw[t] = mat_mul(pw[t - 1], T);

  const int boot = 2 * n + 5;
  vector<MI> ref(boot);
  Mat cur = pw[0], nxt;
  for (int t = 0; t < boot; ++t) {
    ref[t] = cur[0][0];
    nxt = mat_mul(cur, T);
    cur = std::move(nxt);
  }
  const vector<MI> trans = berlekamp_massey(ref);

  vector<MI> fact(D + 1, MI(1)), inv_fact(D + 1, MI(1));
  for (int i = 1; i <= D; ++i)
    fact[i] = fact[i - 1] * MI(i);
  inv_fact[D] = mod_pow(fact[D], MOD - 2);
  for (int i = D; i >= 1; --i)
    inv_fact[i - 1] = inv_fact[i] * MI(i);

  const MI invH = mod_pow(MI(H), MOD - 2);

  auto build_g = [&](bool from_target) {
    vector<int> prod(1, 1);
    for (int ri = 0; ri < H; ++ri) {
      const int from = from_target ? A[ri] : B[ri];
      const int to = A[ri];
      const vector<MI> row = extend_entry(pw, from, to, trans, D);
      vector<int> coeff(D);
      for (int t = 0; t < D; ++t)
        coeff[t] = (row[t] * inv_fact[t]).x;
      prod = poly_conv(prod, coeff, D);
    }
    vector<MI> g(D);
    MI hn(1);
    for (int t = 0; t < D; ++t) {
      g[t] = fact[t] * hn * MI(prod[t]);
      hn = hn * invH;
    }
    return g;
  };

  const vector<MI> gg = build_g(false);
  const vector<MI> hh = build_g(true);

  vector<MI> f(D);
  for (int i = 0; i < D; ++i) {
    f[i] = gg[i];
    for (int j = 1; j < i; ++j)
      f[i] = f[i] - f[j] * hh[i - j];
  }

  const int pref = min(D, 2 * H * W + 30);
  const vector<MI> fbm(f.begin(), f.begin() + pref);
  const vector<MI> rec = berlekamp_massey(fbm);
  cout << expected_from_recurrence(fbm, rec).x << '\n';
  return 0;
}
