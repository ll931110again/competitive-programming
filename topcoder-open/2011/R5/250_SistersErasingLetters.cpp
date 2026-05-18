#include <bits/stdc++.h>
using namespace std;

class SistersErasingLetters {
  string w;
  map<pair<unsigned long long, int>, int> memo;

  int solve(unsigned long long mask, int minPos, bool camiTurn) {
    if (__builtin_popcountll(mask) == 1) {
      int pos = __builtin_ctzll(mask);
      string fin(1, w[pos]);
      return fin > w ? 1 : 0;
    }
    auto key = make_pair(mask, minPos * 2 + camiTurn);
    if (memo.count(key))
      return memo[key];

    int best = camiTurn ? 0 : 1;
    for (int i = 0; i < (int)w.size(); i++) {
      if (!(mask & (1ULL << i)) || i < minPos)
        continue;
      unsigned long long nmask = mask ^ (1ULL << i);
      int res = solve(nmask, 0, !camiTurn);
      if (camiTurn)
        best = max(best, res);
      else
        best = min(best, res);
    }
    return memo[key] = best;
  }

public:
  string whoWins(string word) {
    w = word;
    memo.clear();
    unsigned long long mask = (word.size() == 64 ? ~0ULL : ((1ULL << word.size()) - 1));
    return solve(mask, 0, true) ? "Camomile" : "Romashka";
  }
};
