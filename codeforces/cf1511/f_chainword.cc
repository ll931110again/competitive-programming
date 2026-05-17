// CF 1511F - Chainword
// https://codeforces.com/contest/1511/problem/F
//
// Trie automaton for (top-hint state, bottom-hint state); transition matrix M has
// size <= 161. Answer = (M^m)[0][0]. Cayley–Hamilton: that scalar sequence satisfies
// a linear recurrence — generate prefix via mat–vec, Berlekamp–Massey, kth term.

#include <bits/stdc++.h>
using namespace std;

static constexpr int MOD = 998244353;
static constexpr int K = 161;
static constexpr int AL = 26;

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

struct TrieNode {
  int nxt[AL];
  bool term;
  TrieNode() : term(false) {
    memset(nxt, -1, sizeof(nxt));
  }
};

static int get_id(map<pair<int, int>, int>& num, vector<pair<int, int>>& order, int v, int u) {
  if (v > u)
    swap(v, u);
  const pair<int, int> key{v, u};
  auto it = num.find(key);
  if (it != num.end())
    return it->second;
  const int id = (int)order.size();
  num[key] = id;
  order.push_back(key);
  return id;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  long long m;
  cin >> n >> m;

  vector<TrieNode> trie(1);
  for (int i = 0; i < n; ++i) {
    string s;
    cin >> s;
    int cur = 0;
    for (char ch : s) {
      const int c = ch - 'a';
      if (trie[cur].nxt[c] == -1) {
        trie[cur].nxt[c] = (int)trie.size();
        trie.emplace_back();
      }
      cur = trie[cur].nxt[c];
    }
    trie[cur].term = true;
  }

  map<pair<int, int>, int> num;
  vector<pair<int, int>> order;
  num[{0, 0}] = 0;
  order.push_back({0, 0});

  vector<vector<int>> trans(K, vector<int>(K));
  for (int qi = 0; qi < (int)order.size(); ++qi) {
    const auto [v, u] = order[qi];
    const int x = num.at({v, u});
    for (int c = 0; c < AL; ++c) {
      const int tov = trie[v].nxt[c];
      const int tou = trie[u].nxt[c];
      if (tov == -1 || tou == -1)
        continue;
      const int id = get_id(num, order, tov, tou);
      trans[x][id]++;
      if (trie[tov].term)
        trans[x][get_id(num, order, 0, tou)]++;
      if (trie[tou].term)
        trans[x][get_id(num, order, tov, 0)]++;
      if (trie[tov].term && trie[tou].term)
        trans[x][0]++;
    }
  }

  const int dim = (int)order.size();
  const int need = min(2 * dim + 5, K);

  vector<Mint> seq;
  seq.reserve(need);
  vector<Mint> vec(dim), nxt(dim);
  vec[0] = Mint(1);
  for (int t = 0; t < need; ++t) {
    seq.push_back(vec[0]);
    fill(nxt.begin(), nxt.end(), Mint(0));
    for (int i = 0; i < dim; ++i) {
      if (vec[i] == Mint(0))
        continue;
      for (int j = 0; j < dim; ++j) {
        if (trans[i][j])
          nxt[j] += vec[i] * Mint(trans[i][j]);
      }
    }
    vec.swap(nxt);
  }

  const vector<Mint> rec = berlekamp_massey<MOD>(seq);
  cout << kth_linear_recurrence<MOD>(seq, rec, m).x << '\n';
  return 0;
}
