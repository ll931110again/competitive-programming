// CF 506E - Mr. Kitayuta's Gift (Round #287, contest 506)
// https://codeforces.com/contest/506/problem/E
//
// Problem
// -------
// Given a string s (|s| <= 200) and an integer n (1 <= n <= 1e9), insert exactly n lowercase
// letters anywhere into s (including the ends). Count distinct resulting palindromes modulo
// 10007.
//
// Overview
// --------
// Build the final palindrome from both ends inward. Each step either matches two equal
// characters of s (no insertion) or inserts one letter on the left and/or right. The hard
// part is that n is huge, so we cannot simulate insertions one by one.
//
// Step 1 — Palindrome DP on substrings
// ------------------------------------
// dp[i][j][k] = number of ways to turn s[i..j] into a palindrome using exactly k insertions.
//   • s[i] == s[j]: match for free → dp[i][j][k] = dp[i+1][j-1][k].
//   • s[i] != s[j]: must insert on one or both sides →
//         dp[i][j][k] = dp[i+1][j][k-1] + dp[i][j-1][k-1].
// Base cases handle length 1 and 2. Two variants (type 1 / type 2) differ in whether a
// single-character interval is allowed; type 2 is used only for the odd-length correction.
// After the DP, dp[1][n][*] tells how many “chains” need each number of insertions before
// the automaton phase.
//
// Step 2 — Chain compression and half the insertions
// --------------------------------------------------
// Picture each maximal run of matched characters from s as red nodes on a chain; green
// nodes are inserted letters between them. All chains of the same length behave the same,
// so only O(|s|) chain types matter. If the final palindrome has total length |s| + n,
// exactly K = (|s| + n + 1) / 2 insertions are made on the left half (the right half is
// forced by symmetry). The answer is the number of ways to walk this compressed automaton
// for K steps and finish in an accepting state.
//
// Step 3 — Transition matrix M
// ------------------------------
// States 1 .. |s| + ha (ha = ceil(|s|/2)) encode progress along a chain; state 0 is the
// sink (goal). One step of the walk:
//   • self-loop on state i with weight 24 or 25 = choose a free inserted letter (26 if the
//     walk may stop at the goal with one more letter);
//   • edge i → i-1 with weight 1 = finish the current green block and move to the next red
//     node;
//   • edges into state 0 weighted by t[i][*], the DP-derived counts for how many chains
//     still need a given number of insertions.
// Starting vector has a 1 in state 1. The desired count is the goal component of M^K v.
//
// Step 4 — Fast (M^K v) and Berlekamp–Massey
// -----------------------------------------
// dim(M) = O(|s|) <= 300, but K can be 1e9. The sequence f_k = (M^k v)_goal satisfies a
// recurrence of order <= dim(M) (Cayley–Hamilton); Berlekamp–Massey recovers that
// recurrence from 2·dim samples. In practice we evaluate (M^K v)_goal by binary
// exponentiation on M (int mat–vec + mat–mul, O(dim^3 log K)), which is fast enough.
// berlekamp_massey + kth_linear_recurrence_int below implement the BM route explicitly.
//
// Step 5 — Odd final length
// -------------------------
// If |s| + n is odd, the center character cannot be an arbitrary insertion; it must come
// from s. Subtract palindromes that illegally put a free letter in the center:
//   • |s| = 2 with s[0] == s[1]: subtract 25^(K-1);
//   • otherwise: rerun solve on s[2..|s|-1] with K-1 steps (type-2 DP).
// |s| = 1 or even |s| + n needs no correction.
//
// Complexity: O(|s|^3) for DP + automaton setup, O(|s|^2 · |s|) for BM prefix,
// O(|s|^3 log K) for the K-th term — well within limits for |s| <= 200.

#include <bits/stdc++.h>
using namespace std;

static constexpr int MOD = 10007;

struct Mint;

static Mint mod_pow(Mint a, long long e);

struct Mint {
  int x;
  Mint(long long v = 0) : x(int((v % MOD + MOD) % MOD)) {}
  Mint& operator+=(Mint o) {
    x += o.x;
    if (x >= MOD)
      x -= MOD;
    return *this;
  }
  Mint& operator-=(Mint o) {
    x -= o.x;
    if (x < 0)
      x += MOD;
    return *this;
  }
  Mint& operator*=(Mint o) {
    x = int((long long)x * o.x % MOD);
    return *this;
  }
  friend Mint operator+(Mint a, Mint b) {
    return a += b;
  }
  friend Mint operator-(Mint a, Mint b) {
    return a -= b;
  }
  friend Mint operator*(Mint a, Mint b) {
    return a *= b;
  }
  friend Mint operator/(Mint a, Mint b) {
    return a * mod_pow(b, MOD - 2);
  }
  bool operator==(Mint o) const {
    return x == o.x;
  }
  bool operator!=(Mint o) const {
    return x != o.x;
  }
};

static Mint mod_pow(Mint a, long long e) {
  Mint r(1);
  while (e > 0) {
    if (e & 1)
      r *= a;
    a *= a;
    e >>= 1;
  }
  return r;
}

// s_i = sum_{j=0}^{L-1} trans[j] * s_{i-j-1}
static vector<Mint> berlekamp_massey(const vector<Mint>& s) {
  const int n = (int)s.size();
  vector<Mint> C(n), B(n);
  C[0] = B[0] = Mint(1);
  Mint b(1);
  int L = 0;
  for (int i = 0, m = 1; i < n; ++i) {
    Mint d = s[i];
    for (int j = 1; j <= L; ++j)
      d = d + C[j] * s[i - j];
    if (d == Mint(0)) {
      ++m;
      continue;
    }
    vector<Mint> T = C;
    Mint coef = d / b;
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
  vector<Mint> res(L);
  for (int j = 0; j < L; ++j)
    res[j] = Mint(0) - C[j + 1];
  return res;
}

static void mat_mul_dense_int(int n, vector<vector<int>>& A, vector<vector<int>>& B,
                              vector<vector<int>>& C) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      long long sum = 0;
      for (int t = 0; t < n; ++t)
        sum += (long long)A[i][t] * B[t][j];
      C[i][j] = (int)(sum % MOD);
    }
  }
}

static int kth_linear_recurrence_int(const vector<int>& init, const vector<int>& trans,
                                     long long k) {
  const int n = (int)trans.size();
  if (k < n)
    return init[(int)k];
  if (n == 0)
    return 0;

  vector<vector<int>> base(n, vector<int>(n)), res(n, vector<int>(n)), tmp(n, vector<int>(n));
  for (int j = 0; j < n; ++j)
    base[0][j] = trans[j];
  for (int i = 1; i < n; ++i)
    base[i][i - 1] = 1;
  for (int i = 0; i < n; ++i)
    res[i][i] = 1;

  long long e = k - n + 1;
  while (e > 0) {
    if (e & 1) {
      mat_mul_dense_int(n, res, base, tmp);
      res.swap(tmp);
    }
    mat_mul_dense_int(n, base, base, tmp);
    base.swap(tmp);
    e >>= 1;
  }

  long long ans = 0;
  for (int i = 0; i < n; ++i)
    ans += (long long)res[0][i] * init[n - 1 - i];
  return (int)(ans % MOD);
}

static constexpr int MAXN = 202;

static int mod_pow_int(int x, int n) {
  if (n <= 1)
    return n ? x : 1;
  int t = mod_pow_int(x, n / 2);
  t = (int)((long long)t * t % MOD);
  if (n & 1)
    t = (int)((long long)t * x % MOD);
  return t;
}

struct Mat {
  int n{};
  int a[MAXN * 3 / 2][MAXN * 3 / 2]{};
};

// One step v' = M v on the fixed automaton M (banded except row 0).
static void mat_vec_auto(const Mat& p, const vector<Mint>& in, vector<Mint>& out) {
  const int dim = p.n;
  Mint s0(0);
  for (int k = 0; k < dim; ++k) {
    if (p.a[0][k])
      s0 = s0 + Mint(p.a[0][k]) * in[k];
  }
  out[0] = s0;
  for (int i = 1; i < dim; ++i) {
    out[i] = Mint(0);
    if (p.a[i][i])
      out[i] = out[i] + Mint(p.a[i][i]) * in[i];
    if (p.a[i][i - 1])
      out[i] = out[i] + Mint(p.a[i][i - 1]) * in[i - 1];
  }
}

static void mat_mul_int(const Mat& A, const Mat& B, Mat& R) {
  const int dim = A.n;
  for (int i = 0; i < dim; ++i) {
    for (int j = 0; j < dim; ++j) {
      long long sum = 0;
      for (int t = 0; t < dim; ++t)
        sum += (long long)A.a[i][t] * B.a[t][j];
      R.a[i][j] = (int)(sum % MOD);
    }
  }
}

static void mat_vec_int(const Mat& A, const vector<int>& in, vector<int>& out) {
  const int dim = A.n;
  for (int i = 0; i < dim; ++i) {
    long long sum = 0;
    for (int j = 0; j < dim; ++j) {
      if (A.a[i][j])
        sum += (long long)A.a[i][j] * in[j];
    }
    out[i] = (int)(sum % MOD);
  }
}

// Used in main: binary exponentiation on the automaton.
static int getnum_bm(const Mat& p, int st, int ed, long long K) {
  const int dim = p.n;
  vector<int> vec(dim), nxt(dim);
  vec[st] = 1;

  Mat powM = p, sq{};
  sq.n = dim;
  while (K > 0) {
    if (K & 1) {
      mat_vec_int(powM, vec, nxt);
      vec.swap(nxt);
    }
    mat_mul_int(powM, powM, sq);
    powM = sq;
    K >>= 1;
  }
  return vec[ed];
}

// Explicit BM pipeline (correct; slower than getnum_bm on large dim).
static int getnum_via_berlekamp_massey(const Mat& p, int st, int ed, long long K) {
  const int dim = p.n;
  vector<Mint> cur(dim), nxt(dim);
  cur[st] = Mint(1);

  const int samples = 2 * dim + 10;
  vector<Mint> seq_m;
  seq_m.reserve(samples);
  for (int k = 0; k < samples; ++k) {
    seq_m.push_back(cur[ed]);
    if (k + 1 < samples)
      mat_vec_auto(p, cur, nxt), cur.swap(nxt);
  }

  const vector<Mint> trans_m = berlekamp_massey(seq_m);
  vector<int> seq(samples), trans(trans_m.size());
  for (int i = 0; i < samples; ++i)
    seq[i] = seq_m[i].x;
  for (int i = 0; i < (int)trans.size(); ++i)
    trans[i] = trans_m[i].x;
  return kth_linear_recurrence_int(seq, trans, K);
}

static char s[MAXN];
static int dp[MAXN][MAXN][MAXN];
static int t[MAXN][MAXN];

static int solve(int* d, int n, long long K, int type) {
  memset(t, 0, sizeof(t));
  const int ha = (n + 1) / 2;
  for (int i = 0; i <= n; ++i)
    t[i][(n - i + 1) / 2] = d[i];
  for (int i = 0; i < n; ++i) {
    for (int j = 1; j <= ha; ++j) {
      t[i + 1][j] = (t[i + 1][j] + t[i][j]) % MOD;
      t[i + 1][j - 1] = (t[i + 1][j - 1] + t[i][j]) % MOD;
      t[i][j] = 0;
    }
  }

  static constexpr int GOAL = 0;
  Mat p{};
  p.n = n + ha + 1;
  for (int i = 2; i <= n + ha; ++i)
    p.a[i][i - 1] = 1;
  for (int i = 1; i <= n + ha; ++i)
    p.a[i][i] = (i <= n ? 24 : 25);
  for (int i = ha; i <= n + ha; ++i) {
    p.a[GOAL][i] = (i <= n ? t[i][0] : t[n][i - n]);
  }
  p.a[GOAL][GOAL] = (type == 1 ? 26 : 25);
  return getnum_bm(p, 1, GOAL, K);
}

static void palindrome_dp(int n, int type) {
  memset(dp, 0, sizeof(dp));
  for (int i = n; i >= 1; --i) {
    for (int j = i; j <= n; ++j) {
      if (type == 1) {
        if (i == j) {
          dp[i][j][0] = 1;
          continue;
        }
      } else {
        if (i == j)
          continue;
        if (j == i + 1) {
          dp[i][j][0] = (s[i] == s[j] ? 1 : 0);
          continue;
        }
      }
      if (s[i] == s[j]) {
        for (int k = 0; k <= n; ++k)
          dp[i][j][k] = dp[i + 1][j - 1][k];
        if (j == i + 1)
          dp[i][j][0] = 1;
      } else {
        dp[i][j][0] = 0;
        for (int k = 1; k <= n; ++k) {
          dp[i][j][k] = (dp[i + 1][j][k - 1] + dp[i][j - 1][k - 1]) % MOD;
        }
      }
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  string input;
  long long m_add;
  cin >> input >> m_add;
  const int n = (int)input.size();
  for (int i = 1; i <= n; ++i)
    s[i] = input[i - 1];

  palindrome_dp(n, 1);
  const long long K = (n + m_add + 1) / 2;
  int ans = solve(dp[1][n], n, K, 1);

  if (n == 1 || (n + m_add) % 2 == 0) {
    cout << ans << '\n';
    return 0;
  }
  if (n == 2 && s[1] == s[2]) {
    ans = (ans - mod_pow_int(25, (int)(K - 1)) + MOD) % MOD;
  } else if (n != 2) {
    palindrome_dp(n, 2);
    ans = (ans - solve(dp[1][n], n - 2, K - 1, 2) + MOD) % MOD;
  }
  cout << ans << '\n';
  return 0;
}
