#include <bits/stdc++.h>
using namespace std;

class TripleStrings {
public:
  int getMinimumOperations(string init, string goal) {
    int n = init.size();
    int maxMatch = 0;
    for (int len = 1; len <= n; len++) {
      bool ok = true;
      for (int i = 0; i < len; i++)
        if (init[n - len + i] != goal[i])
          ok = false;
      if (ok)
        maxMatch = len;
    }
    return (n - maxMatch) * 2;
  }
};
