// ARC170 E - BDFS (Cayley–Hamilton / Berlekamp–Massey route)
// https://atcoder.jp/contests/arc170/tasks/arc170_e
//
// Expected sum a_N is a linear combination of entries of A^N (pair-DP matrix), so {a_N}
// satisfies a constant-coefficient recurrence (Cayley–Hamilton). Prefix terms are filled
// from the pair decomposition below; Berlekamp–Massey recovers coeffs, kth_linear_recurrence
// evaluates a_N in O(d^3 log N). Define BDFS_BRUTE_PREFIX to seed from exact small-N DP.

#include <bits/stdc++.h>
using namespace std;

static constexpr int MOD = 998244353;

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
    return a * mod_pow(b, P - 2);
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

template <int P>
ModInt<P> kth_linear_recurrence(vector<ModInt<P>> init, const vector<ModInt<P>>& trans,
                                long long k) {
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
  long long e = k - n + 1;
  M powM = base;
  while (e > 0) {
    if (e & 1)
      res = mat_mul(res, powM);
    powM = mat_mul(powM, powM);
    e >>= 1;
  }

  ModInt<P> ans(0);
  for (int i = 0; i < n; ++i)
    ans += res[0][i] * init[n - 1 - i];
  return ans;
}

using Mint = ModInt<MOD>;

static Mint mod_inv(Mint a) {
  return mod_pow(a, MOD - 2);
}

// Closed form for a_N (valid for all N); used to build the BM prefix quickly.
static Mint prefix_term(long long N, int P) {
  long long n = N - 1;
  Mint inv2 = Mint((MOD + 1) / 2);
  Mint r = Mint(2) * Mint(P) * mod_inv(Mint(100)) - Mint(1);
  Mint nn = Mint(n % MOD), n1 = Mint((n - 1) % MOD);
  Mint ans = nn * (nn + Mint(1)) * inv2;
  Mint rn = mod_pow(r, n);
  Mint geom, weighted;
  if (r == Mint(1)) {
    geom = nn;
    weighted = n1 * (n1 + Mint(MOD - 1)) * inv2;
  } else {
    Mint denom = r + Mint(MOD - 1);
    geom = (rn + Mint(MOD - 1)) * mod_inv(denom);
    weighted = (n1 * rn + Mint(1) - geom) * mod_inv(denom);
  }
  ans = ans + nn * geom;
  ans = ans - weighted;
  ans = ans * inv2;
  return ans;
}

#ifdef BDFS_BRUTE_PREFIX
static Mint brute_expected(int N, int P) {
  const Mint prob_front = Mint(P) * mod_pow(Mint(100), MOD - 2);
  const Mint prob_back = Mint(1) - prob_front;

  function<pair<Mint, Mint>(vector<int>, deque<pair<int, int>>)> dfs =
      [&](vector<int> D, deque<pair<int, int>> Q) -> pair<Mint, Mint> {
    if (Q.empty()) {
      Mint sum(0);
      for (int v : D)
        sum += Mint(v);
      return {sum, Mint(1)};
    }
    int v = Q.front().first, d = Q.front().second;
    Q.pop_front();
    if (D[v] != -1)
      return dfs(std::move(D), std::move(Q));
    D[v] = d;
    vector<int> nbrs;
    for (int dx : {-1, 1}) {
      int x = v + dx;
      if (x < 0)
        x += N;
      if (x >= N)
        x -= N;
      if (D[x] == -1)
        nbrs.push_back(x);
    }
    sort(nbrs.begin(), nbrs.end());
    Mint sum_exp(0);
    function<void(int, deque<pair<int, int>>, Mint)> gen = [&](int i, deque<pair<int, int>> curQ,
                                                               Mint prob) {
      if (i == (int)nbrs.size()) {
        sum_exp += dfs(D, std::move(curQ)).first * prob;
        return;
      }
      int x = nbrs[i];
      auto qf = curQ;
      qf.push_front({x, d + 1});
      gen(i + 1, std::move(qf), prob * prob_front);
      auto qb = curQ;
      qb.push_back({x, d + 1});
      gen(i + 1, std::move(qb), prob * prob_back);
    };
    gen(0, std::move(Q), Mint(1));
    return {sum_exp, Mint(1)};
  };

  vector<int> D(N, -1);
  deque<pair<int, int>> Q;
  Q.emplace_back(0, 0);
  return dfs(D, std::move(Q)).first;
}
#endif

static Mint solve_bm(long long N, int P) {
  static unordered_map<int, pair<vector<Mint>, vector<Mint>>> cache;
  auto& entry = cache[P];
  if (entry.first.empty()) {
    constexpr int PREFIX = 64;
    entry.first.reserve(PREFIX);
    for (int n = 3; n < 3 + PREFIX; ++n) {
#ifdef BDFS_BRUTE_PREFIX
      entry.first.push_back(brute_expected(n, P));
#else
      entry.first.push_back(prefix_term(n, P));
#endif
    }
    entry.second = berlekamp_massey(entry.first);
  }
  return kth_linear_recurrence(entry.first, entry.second, N - 3);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--) {
    long long N;
    int P;
    cin >> N >> P;
    cout << solve_bm(N, P).x << '\n';
  }
  return 0;
}
