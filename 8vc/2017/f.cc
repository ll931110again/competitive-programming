#include <bits/stdc++.h>
using namespace std;

static constexpr unsigned MOD = 1'000'000'007;
#include "../../lib/modint.h"
using Mint = ModInt<MOD>;

static constexpr int maxn = 5005;

int n;
string s;

Mint fact[maxn], inv[maxn];
Mint dp[maxn][26], sum_dp[maxn];

static Mint binom(int x, int y) { return fact[x] * inv[y] * inv[x - y]; }

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  cin >> s;

  string compressed = "";
  for (int i = 0; i < n; i++)
    if (i == 0 || s[i] != s[i - 1]) {
      compressed += s[i];
    }

  for (auto c : compressed) {
    int ds = c - 'a';

    if (dp[1][ds] == 0) {
      sum_dp[1] += 1;
    }
    dp[1][ds] = 1;

    for (int i = 2; i <= n; i++) {
      sum_dp[i] -= dp[i][ds];
      dp[i][ds] = sum_dp[i - 1] - dp[i - 1][ds];
      sum_dp[i] += dp[i][ds];
    }
  }

  fact[0] = 1;
  for (int i = 1; i < maxn; i++) {
    fact[i] = fact[i - 1] * i;
  }
  inv[maxn - 1] = fact[maxn - 1].inv();
  for (int i = maxn - 2; i >= 0; i--) {
    inv[i] = inv[i + 1] * (i + 1);
  }

  Mint ans = 0;
  for (int i = 1; i <= n; i++) {
    ans += sum_dp[i] * binom(n - 1, i - 1);
  }
  cout << ans << endl;

  return 0;
}