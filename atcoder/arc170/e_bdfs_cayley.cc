// ARC170 E - BDFS (Cayley–Hamilton / Berlekamp–Massey route)
// https://atcoder.jp/contests/arc170/tasks/arc170_e
//
// Expected sum a_N is a linear combination of entries of A^N (pair-DP matrix), so {a_N}
// satisfies a constant-coefficient recurrence (Cayley–Hamilton). Prefix terms are filled
// from the pair decomposition below; Berlekamp–Massey recovers coeffs, kth_linear_recurrence
// evaluates a_N in O(d^3 log N). Define BDFS_BRUTE_PREFIX to seed from exact small-N DP.

#include <bits/stdc++.h>
#include "../../common/berlekamp_massey.hpp"

using namespace std;

static constexpr int MOD = 998244353;
using Mint = ModInt<MOD>;

static Mint mod_inv(Mint a) { return mod_pow(a, MOD - 2); }

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
      for (int v : D) sum += Mint(v);
      return {sum, Mint(1)};
    }
    int v = Q.front().first, d = Q.front().second;
    Q.pop_front();
    if (D[v] != -1) return dfs(std::move(D), std::move(Q));
    D[v] = d;
    vector<int> nbrs;
    for (int dx : {-1, 1}) {
      int x = v + dx;
      if (x < 0) x += N;
      if (x >= N) x -= N;
      if (D[x] == -1) nbrs.push_back(x);
    }
    sort(nbrs.begin(), nbrs.end());
    Mint sum_exp(0);
    function<void(int, deque<pair<int, int>>, Mint)> gen =
        [&](int i, deque<pair<int, int>> curQ, Mint prob) {
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
  auto &entry = cache[P];
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
