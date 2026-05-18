#include <bits/stdc++.h>
using namespace std;

class CalculationCards {
  map<tuple<vector<int>, long long, long long>, double> memo;
  vector<char> ops;
  vector<int> digs;
  int tTypes;

  double solve(vector<int> cnt, long long sf, long long term) {
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
      long long nsf = sf;
      long long nterm = term;
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
