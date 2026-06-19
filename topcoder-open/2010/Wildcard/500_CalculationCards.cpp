#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class CalculationCards {
  map<tuple<vector<int>, i64, i64>, double> memo;
  vector<char> ops;
  vector<int> digs;
  int tTypes;

  double solve(vector<int> cnt, i64 sf, i64 term) {
    int rem = 0;
    for (int x : cnt)
      rem += x;
    if (rem == 0)
      return (double)(sf + term);
    auto key = make_tuple(cnt, sf, term);
    if (memo.count(key))
      return memo[key];

    double res = 0;
    for (int i = 0; i < tTypes; i++) {
      if (cnt[i] == 0)
        continue;
      cnt[i]--;
      i64 nsf = sf;
      i64 nterm = term;
      char op = ops[i];
      int d = digs[i];
      if (op == '+') {
        nsf += term;
        nterm = d;
      } else if (op == '-') {
        nsf += term;
        nterm = -d;
      } else {
        nterm *= d;
      }
      res += (cnt[i] + 1.0) / rem * solve(cnt, nsf, nterm);
      cnt[i]++;
    }
    return memo[key] = res;
  }

public:
  double getExpected(vector<string> cards) {
    map<string, int> id;
    vector<int> cnt;
    for (const string& c : cards) {
      if (!id.count(c)) {
        id[c] = (int)ops.size();
        ops.push_back(c[0]);
        digs.push_back(c[1] - '0');
        cnt.push_back(0);
      }
      cnt[id[c]]++;
    }
    tTypes = (int)ops.size();
    memo.clear();
    return solve(cnt, 0, 0);
  }
};
