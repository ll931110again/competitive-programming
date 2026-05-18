#include <bits/stdc++.h>
using namespace std;

using namespace std;
static bool isPrefix(int idx, const string& str, const string& S) {
  if (idx + (int)str.size() > (int)S.size())
    return false;
  for (int i = 0; i < (int)str.size(); i++)
    if (S[idx + i] != str[i])
      return false;
  return true;
}

class HandlesSpelling {
  vector<string> badges;
  string S;
  bool prefix[1001][51];
  int startLen[1001];
  int memoL[1001];
  int memoR[1001];
  int limit;

  int maxCover(int idx, int* memo) {
    if (idx >= limit)
      return 0;
    if (memo[idx] != -1)
      return memo[idx];
    int res = maxCover(idx + 1, memo);
    for (int j = 0; j < (int)badges.size(); j++) {
      if (!prefix[idx][j])
        continue;
      int len = (int)badges[j].size();
      if (idx + len > limit)
        continue;
      res = max(res, len + maxCover(idx + len, memo));
    }
    return memo[idx] = res;
  }

public:
  int spellIt(vector<string> parts, vector<string> badges) {
    this->badges = badges;
    S.clear();
    for (int i = 0; i < (int)parts.size(); i++)
      S += parts[i];
    int n = (int)S.size();
    memset(prefix, 0, sizeof(prefix));
    for (int i = 0; i < n; i++)
      for (int j = 0; j < (int)badges.size(); j++)
        if (isPrefix(i, badges[j], S))
          prefix[i][j] = true;

    memset(startLen, 0, sizeof(startLen));
    for (int i = n - 1; i >= 0; --i) {
      for (int j = 0; j < (int)badges.size(); j++) {
        if (!prefix[i][j])
          continue;
        int len = (int)badges[j].size();
        startLen[i] = max(startLen[i], startLen[i + len] + len);
      }
    }

    int longest = 0;
    for (int i = 0; i < n; i++)
      longest = max(longest, startLen[i]);
    if (longest == 0)
      return -n;

    int best = -n;
    for (int i = 0; i < n; i++) {
      if (startLen[i] != longest)
        continue;
      int leftUnc = 0, rightUnc = 0;
      if (i > 0) {
        memset(memoL, -1, sizeof(memoL));
        limit = i;
        leftUnc = i - maxCover(0, memoL);
      }
      if (i + longest < n) {
        memset(memoR, -1, sizeof(memoR));
        limit = n;
        rightUnc = (n - i - longest) - maxCover(i + longest, memoR);
      }
      best = max(best, longest * longest - leftUnc - rightUnc);
    }
    return best;
  }
};
