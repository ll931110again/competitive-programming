// ABC428 G - Necklace: https://atcoder.jp/contests/abc428/tasks/abc428_g
//
// O(U log U) via Dirichlet generating functions (editorial solution 2).
// C(s) = sum_d phi(d)/d * log(1/(1-A(ds)))  where A(s) = sum freq[b]/b^s.
// Use Omega(n) in place of log n for the formal derivative on DGFs mod 998244353.

#include <bits/stdc++.h>
using namespace std;

static constexpr unsigned MOD = 998244353;

template <unsigned M> struct ModInt {
  unsigned x;
  ModInt() : x(0) {}
  ModInt(unsigned long long v) : x(unsigned(v % M)) {}
  ModInt(int v) : x(((v %= int(M)) < 0) ? (v + int(M)) : v) {}

  ModInt& operator+=(ModInt a) {
    x = ((x += a.x) >= M) ? (x - M) : x;
    return *this;
  }
  ModInt& operator-=(ModInt a) {
    x = ((x -= a.x) >= M) ? (x + M) : x;
    return *this;
  }
  ModInt& operator*=(ModInt a) {
    x = unsigned((unsigned long long)x * a.x % M);
    return *this;
  }
  ModInt& operator/=(ModInt a) {
    return *this *= a.inv();
  }

  ModInt inv() const {
    unsigned a = M, b = x;
    int y = 0, z = 1;
    for (; b;) {
      unsigned q = a / b;
      unsigned c = a - q * b;
      a = b;
      b = c;
      int w = y - int(q) * z;
      y = z;
      z = w;
    }
    return ModInt(y);
  }

  ModInt operator+(ModInt a) const {
    return ModInt(*this) += a;
  }
  ModInt operator-(ModInt a) const {
    return ModInt(*this) -= a;
  }
  ModInt operator*(ModInt a) const {
    return ModInt(*this) *= a;
  }
  ModInt operator/(ModInt a) const {
    return ModInt(*this) /= a;
  }
};

using Mint = ModInt<MOD>;

static void dirichlet_conv(const vector<Mint>& a, const vector<Mint>& b, vector<Mint>& c, int U) {
  fill(c.begin(), c.end(), Mint(0));
  for (int i = 1; i <= U; i++) {
    if (!a[i].x)
      continue;
    for (int j = 1; i * j <= U; j++)
      c[i * j] += a[i] * b[j];
  }
}

// Inverse of a (Dirichlet); requires a[1] == 1.
static void dirichlet_inv(const vector<Mint>& a, vector<Mint>& b, int U) {
  fill(b.begin(), b.end(), Mint(0));
  b[1] = 1;
  for (int i = 1; i <= U; i++) {
    if (!b[i].x)
      continue;
    for (int j = 2; i * j <= U; j++)
      b[i * j] -= b[i] * a[j];
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, U;
  cin >> N >> U;

  vector<Mint> A(U + 1);
  for (int i = 0; i < N; i++) {
    int b;
    cin >> b;
    A[b] += 1;
  }

  int V = 0;
  while ((1LL << (V + 1)) <= U)
    ++V;

  vector<int> lpf(U + 1);
  iota(lpf.begin(), lpf.end(), 0);
  for (int p = 2; p <= U; p++) {
    if (lpf[p] != p)
      continue;
    for (int j = p; j <= U; j += p) {
      if (lpf[j] == j)
        lpf[j] = p;
    }
  }
  vector<int> Omega(U + 1);
  for (int i = 2; i <= U; i++)
    Omega[i] = Omega[i / lpf[i]] + 1;

  auto euler_phi = [](int x) {
    int r = x;
    for (int p = 2; p * p <= x; p++) {
      if (x % p)
        continue;
      while (x % p == 0)
        x /= p;
      r -= r / p;
    }
    if (x > 1)
      r -= r / x;
    return r;
  };

  int max_omega = 0;
  for (int i = 2; i <= U; i++)
    max_omega = max(max_omega, Omega[i]);

  vector<Mint> totient(V + 1), inv_d(V + 1), inv_omega(max_omega + 1);
  for (int d = 1; d <= V; d++) {
    totient[d] = euler_phi(d);
    inv_d[d] = Mint(d).inv();
  }
  for (int k = 1; k <= max_omega; k++)
    inv_omega[k] = Mint(k).inv();

  // B = 1 - A(s): B[1]=1, B[n]=-A[n].
  vector<Mint> B(U + 1);
  B[1] = 1;
  for (int i = 2; i <= U; i++)
    B[i] = Mint(0) - A[i];

  // G = 1 / (1 - A(s)).
  vector<Mint> G(U + 1);
  dirichlet_inv(B, G, U);

  // D_Omega(G)[n] = Omega(n) * G[n];  F = D_Omega(G) * B;  F[n] /= Omega(n).
  for (int i = 1; i <= U; i++)
    G[i] *= Mint(Omega[i]);
  vector<Mint> F(U + 1);
  dirichlet_conv(G, B, F, U);
  for (int i = 2; i <= U; i++)
    F[i] *= inv_omega[Omega[i]];

  vector<Mint> ans(U + 1);
  for (int i = 2; i <= U; i++) {
    if (!F[i].x)
      continue;
    for (long long j = i, d = 1; j <= U; j *= i, ++d) {
      ans[j] += F[i] * totient[d] * inv_d[d];
    }
  }

  for (int x = 2; x <= U; x++) {
    if (x > 2)
      cout << ' ';
    cout << ans[x].x;
  }
  cout << '\n';
  return 0;
}
