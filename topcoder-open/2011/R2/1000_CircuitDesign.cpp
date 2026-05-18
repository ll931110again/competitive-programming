#include <bits/stdc++.h>
using namespace std;

class CircuitDesign {
  static const int MOD = 1000000007;

  int n, m;
  vector<int> top, bottom;
  vector<int> topCells;
  vector<int> botCells;
  int topId[51];
  int kTop, kBot;
  long long fact[51];

  long long modPow(long long a, long long e) {
    long long r = 1;
    while (e) {
      if (e & 1)
        r = r * a % MOD;
      a = a * a % MOD;
      e >>= 1;
    }
    return r;
  }

  long long linext(const vector<vector<int>>& must, int k) {
    for (int i = 0; i < k; i++)
      if (must[i][i])
        return 0;
    vector<long long> dp(1 << k);
    dp[0] = 1;
    for (int mask = 0; mask < (1 << k); mask++) {
      if (!dp[mask])
        continue;
      for (int v = 0; v < k; v++) {
        if (mask & (1 << v))
          continue;
        bool ok = true;
        for (int u = 0; u < k; u++) {
          if ((mask & (1 << u)) && must[u][v]) {
            ok = false;
            break;
          }
        }
        if (ok)
          dp[mask | (1 << v)] = (dp[mask | (1 << v)] + dp[mask]) % MOD;
      }
    }
    return dp[(1 << k) - 1];
  }

  long long countBottom(const vector<vector<int>>& mustFull) {
    vector<vector<int>> must(kBot, vector<int>(kBot));
    for (int i = 0; i < kBot; i++) {
      for (int j = 0; j < kBot; j++) {
        if (mustFull[botCells[i]][botCells[j]])
          must[i][j] = 1;
      }
    }
    long long ways = linext(must, kBot);
    if (kBot < n) {
      long long denom = fact[n - kBot];
      ways = ways * fact[n] % MOD * modPow(denom, MOD - 2) % MOD;
    }
    return ways;
  }

  map<pair<int, vector<vector<int>>>, long long> memo;

  long long dfs(int maskT, vector<vector<int>>& mustFull) {
    if (maskT == (1 << kTop) - 1)
      return countBottom(mustFull);
    auto key = make_pair(maskT, mustFull);
    auto it = memo.find(key);
    if (it != memo.end())
      return it->second;

    long long res = 0;
    for (int ti = 0; ti < kTop; ti++) {
      if (maskT & (1 << ti))
        continue;
      int t = topCells[ti];
      vector<vector<int>> nxt = mustFull;
      for (int i = 0; i < m; i++) {
        if (top[i] != t)
          continue;
        int bi = bottom[i];
        for (int j = 0; j < m; j++) {
          if (top[j] == t)
            continue;
          int tj = top[j];
          int tjIdx = topId[tj];
          if (!(maskT & (1 << tjIdx)))
            continue;
          int bj = bottom[j];
          nxt[bj][bi] = 1;
        }
      }
      res = (res + dfs(maskT | (1 << ti), nxt)) % MOD;
    }
    memo[key] = res;
    return res;
  }

public:
  int countPerms(int n_, vector<int> top_, vector<int> bottom_) {
    n = n_;
    top = top_;
    bottom = bottom_;
    m = top.size();

    fact[0] = 1;
    for (int i = 1; i <= n; i++)
      fact[i] = fact[i - 1] * i % MOD;

    memset(topId, -1, sizeof(topId));
    topCells.clear();
    botCells.clear();
    for (int i = 0; i < m; i++) {
      if (topId[top[i]] < 0) {
        topId[top[i]] = (int)topCells.size();
        topCells.push_back(top[i]);
      }
      if (find(botCells.begin(), botCells.end(), bottom[i]) == botCells.end())
        botCells.push_back(bottom[i]);
    }
    kTop = (int)topCells.size();
    kBot = (int)botCells.size();

    vector<vector<int>> mustFull(n + 1, vector<int>(n + 1));
    memo.clear();
    long long inner = dfs(0, mustFull);

    long long embed = fact[n];
    if (kTop < n)
      embed = embed * modPow(fact[n - kTop], MOD - 2) % MOD;

    return (int)(inner * embed % MOD);
  }
};
