#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

static const int MOD = 1000003;  // prime

static int mod_add(int a, int b) {
  a += b;
  if (a >= MOD) a -= MOD;
  return a;
}
static int mod_sub(int a, int b) {
  a -= b;
  if (a < 0) a += MOD;
  return a;
}
static long long mod_mul(long long a, long long b) { return (a * b) % MOD; }

static int mod_pow(int a, long long e) {
  long long r = 1, x = a;
  while (e > 0) {
    if (e & 1) r = (r * x) % MOD;
    x = (x * x) % MOD;
    e >>= 1;
  }
  return (int)r;
}

struct Comb {
  vector<int> fact, ifact;
  explicit Comb(int nmax) {
    fact.resize(nmax + 1);
    ifact.resize(nmax + 1);
    fact[0] = 1;
    for (int i = 1; i <= nmax; i++) fact[i] = (int)mod_mul(fact[i - 1], i);
    ifact[nmax] = mod_pow(fact[nmax], MOD - 2);
    for (int i = nmax; i >= 1; i--) ifact[i - 1] = (int)mod_mul(ifact[i], i);
  }
  int C(int n, int k) const {
    if (k < 0 || k > n) return 0;
    return (int)mod_mul(fact[n], mod_mul(ifact[k], ifact[n - k]));
  }
};

static int count_runs(const string& s) {
  int r = 1;
  for (int i = 1; i < (int)s.size(); i++) r += (s[i] != s[i - 1]);
  return r;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;

  // Per limits.
  Comb comb(450000);

  for (int tc = 1; tc <= T; tc++) {
    string S;
    cin >> S;
    int R = count_runs(S);

    array<int, 26> cnt{};
    for (char c : S) cnt[c - 'a']++;

    vector<int> letters;
    letters.reserve(26);
    for (int i = 0; i < 26; i++) {
      if (cnt[i] > 0) letters.push_back(i);
    }

    // dp[r][k] where r = total blocks used so far, k = total merged-adjacencies (inclusion-exclusion).
    vector<vector<int>> dp(R + 1, vector<int>(R + 1, 0));
    dp[0][0] = 1;

    for (int idx : letters) {
      int c = cnt[idx];
      vector<vector<int>> ndp(R + 1, vector<int>(R + 1, 0));
      for (int r = 0; r <= R; r++) {
        for (int k = 0; k <= R; k++) {
          int cur = dp[r][k];
          if (!cur) continue;

          // Choose number of blocks b for this letter (>=1), and number of merges m (0..b-1).
          for (int b = 1; b <= c && r + b <= R; b++) {
            int ways_split = comb.C(c - 1, b - 1);  // compositions of c into b positive parts
            if (!ways_split) continue;
            for (int m = 0; m <= b - 1 && k + m <= R; m++) {
              // inclusion-exclusion term for forbidding identical-adjacent blocks:
              // choose which adjacencies to merge among the b-1 potential ones.
              int ways_merge = comb.C(b - 1, m);
              int groups = b - m;
              int w = (int)mod_mul(mod_mul(ways_split, ways_merge), comb.ifact[groups]);
              ndp[r + b][k + m] = mod_add(ndp[r + b][k + m], (int)mod_mul(cur, w));
            }
          }
        }
      }
      dp.swap(ndp);
    }

    // Assemble answer:
    // sum_k (-1)^k * fact[R-k] * dp[R][k]
    int ans = 0;
    for (int k = 0; k <= R; k++) {
      int ways = dp[R][k];
      if (!ways) continue;
      int term = (int)mod_mul(comb.fact[R - k], ways);
      if (k & 1) ans = mod_sub(ans, term);
      else ans = mod_add(ans, term);
    }

    cout << "Case #" << tc << ": " << ans << "\n";
  }
  return 0;
}

