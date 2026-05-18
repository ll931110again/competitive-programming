#include <bits/stdc++.h>
using namespace std;

class DecodeDigits {
  bool validVal(int x) {
    return x >= 1 && x <= 26;
  }

  bool anyDecode(const string& D) {
    int n = (int)D.size();
    vector<char> ok(n + 1, 0);
    ok[0] = 1;
    for (int i = 0; i < n; i++) {
      if (!ok[i])
        continue;
      if (D[i] == '0')
        return false;
      int one = D[i] - '0';
      if (validVal(one))
        ok[i + 1] = 1;
      if (i + 1 < n) {
        int two = one * 10 + (D[i + 1] - '0');
        if (validVal(two))
          ok[i + 2] = 1;
      }
    }
    return ok[n];
  }

  // min/max matched prefix length after processing all of D
  pair<int, int> matchRange(const string& D, const string& pref) {
    int n = (int)D.size();
    int m = (int)pref.size();
    const int INF = 1e9;
    vector<int> lo(n + 1, INF), hi(n + 1, -INF);
    lo[0] = hi[0] = 0;
    for (int i = 0; i < n; i++) {
      if (lo[i] == INF)
        continue;
      if (D[i] == '0')
        continue;
      int one = D[i] - '0';
      if (validVal(one)) {
        int nlo = lo[i];
        int nhi = hi[i];
        if (nlo < m && pref[nlo] == char('a' + one - 1))
          nlo++;
        if (nhi < m && pref[nhi] == char('a' + one - 1))
          nhi++;
        lo[i + 1] = min(lo[i + 1], nlo);
        hi[i + 1] = max(hi[i + 1], nhi);
      }
      if (i + 1 < n) {
        int two = one * 10 + (D[i + 1] - '0');
        if (validVal(two)) {
          int nlo = lo[i];
          int nhi = hi[i];
          if (nlo < m && pref[nlo] == char('a' + two - 1))
            nlo++;
          if (nhi < m && pref[nhi] == char('a' + two - 1))
            nhi++;
          lo[i + 2] = min(lo[i + 2], nlo);
          hi[i + 2] = max(hi[i + 2], nhi);
        }
      }
    }
    return {lo[n], hi[n]};
  }

  bool allContain(const string& D, const string& pref) {
    auto pr = matchRange(D, pref);
    return pr.first >= (int)pref.size();
  }

  bool someContain(const string& D, const string& pref) {
    auto pr = matchRange(D, pref);
    return pr.second >= (int)pref.size();
  }

public:
  string solve(string D) {
    if (!anyDecode(D))
      return "NONE";

    string ans;
    while (true) {
      bool ok = false;
      for (char c = 'a'; c <= 'z'; c++) {
        string nxt = ans;
        nxt.push_back(c);
        if (allContain(D, nxt)) {
          ans = nxt;
          ok = true;
          break;
        }
      }
      if (!ok)
        break;
    }
    return ans;
  }
};
