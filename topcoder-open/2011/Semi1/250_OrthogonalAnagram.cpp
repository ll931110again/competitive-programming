#include <bits/stdc++.h>
using namespace std;

class OrthogonalAnagram {
public:
  string solve(string S) {
    int n = S.size();
    string T = S;
    sort(T.begin(), T.end());
    string ans(n, ' ');
    vector<int> cnt(26);
    for (char c : T)
      cnt[c - 'a']++;

    auto canFinish = [&](int pos, vector<int> rem) {
      for (int i = pos; i < n; i++) {
        bool ok = false;
        for (int c = 0; c < 26; c++) {
          if (rem[c] > 0 && ('a' + c) != S[i]) {
            ok = true;
            break;
          }
        }
        if (!ok)
          return false;
      }
      return true;
    };

    function<bool(int, vector<int>)> dfs = [&](int pos, vector<int> rem) -> bool {
      if (pos == n)
        return true;
      for (int c = 0; c < 26; c++) {
        if (rem[c] == 0)
          continue;
        char ch = 'a' + c;
        if (ch == S[pos])
          continue;
        rem[c]--;
        ans[pos] = ch;
        if (canFinish(pos + 1, rem) && dfs(pos + 1, rem))
          return true;
        rem[c]++;
      }
      return false;
    };

    if (!dfs(0, cnt))
      return "";
    return ans;
  }
};
