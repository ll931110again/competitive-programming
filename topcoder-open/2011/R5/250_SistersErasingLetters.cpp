#include <bits/stdc++.h>
using namespace std;

class SistersErasingLetters {
  string s;
  map<string, int> memo;

  // 0 = Camomile wins, 1 = Romashka wins
  int solve(const string& t, int turn, int minPos) {
    if ((int)t.size() == 1) {
      if (t > s)
        return 0;
      return 1;
    }
    string key;
    key.reserve(t.size() + 4);
    key.push_back(char('0' + turn));
    key.push_back(char('0' + minPos));
    key += t;
    if (memo.count(key))
      return memo[key];

    if (turn == 0) {
      for (int i = minPos; i < (int)t.size(); i++) {
        string nxt = t;
        nxt.erase(nxt.begin() + i);
        if (solve(nxt, 1, i) == 0)
          return memo[key] = 0;
      }
      return memo[key] = 1;
    }
    for (int i = minPos; i < (int)t.size(); i++) {
      string nxt = t;
      nxt.erase(nxt.begin() + i);
      if (solve(nxt, 0, i) == 1)
        return memo[key] = 1;
    }
    return memo[key] = 0;
  }

public:
  string whoWins(string word) {
    s = word;
    memo.clear();
    return solve(word, 0, 0) == 0 ? "Camomile" : "Romashka";
  }
};
