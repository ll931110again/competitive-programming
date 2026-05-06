#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#define maxn 100005
using namespace std;

int T, n, m;
vector<int> odds, evens;

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n >> m;
    odds.clear();
    evens.clear();

    int x;
    for (int i = 0; i < n; i++) {
      cin >> x;
      if (i % 2 == 0) {
        evens.push_back(x);
      } else {
        odds.push_back(x);
      }
    }

    bool marked_odds = false, marked_evens = false;
    sort(odds.begin(), odds.end());
    sort(evens.begin(), evens.end());

    for (int i = 0; i < m; i++) {
      cin >> x;
      x--;
      if (x % 2 == 0 && !evens.empty() && (!marked_evens || evens.back() > 0)) {
        marked_evens = true;
        evens.pop_back();
      } else if (x % 2 && !odds.empty() && (!marked_odds || odds.back() > 0)) {
        marked_odds = true;
        odds.pop_back();
      }
    }

    long long ans = 0;
    for (auto x : odds) {
      ans += x;
    }
    for (auto x : evens) {
      ans += x;
    }

    cout << ans << endl;
  }

  return 0;
}
