// Codeforces 1704 (CodeTON Round 2 (Div. 1 + Div. 2, Rated, Prizes!)) — A. Two 0-1 Sequences
// Submission: https://codeforces.com/contest/1704/submission/318382308

#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>
#include <queue>
#include <vector>
using namespace std;

int T, m, n;
string a, b;

bool solve() {
  bool ok = false;
  for (int i = 0; i <= n - m; i++) {
    if (a[i] == b[0]) {
      ok = true;
      break;
    }
  }

  if (!ok) {
    return false;
  }

  for (int i = n - m + 1, j = 1; i < n && j < m; i++, j++) {
    if (a[i] != b[j]) {
      return false;
    }
  }

  return true;
}

int main() {
  cin >> T;
  while (T--) {
    cin >> n >> m;
    cin >> a >> b;
    cout << (solve() ? "YES" : "NO") << endl;
  }
  return 0;
}
