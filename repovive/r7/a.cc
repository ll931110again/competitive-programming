#ifdef ONLINE_JUDGE
  #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
#define maxn 200005
using namespace std;

namespace {
  int T, n;
  string s;
  bool marked[maxn];
} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    cin >> s;

    n *= 2;
    int ans = 0;
    
    for (int i = 0; i < n; i++) {
      marked[i] = false;
    }

    int i = 0, j = n - 1;
    while (i < j) {
      while (i < n) {
        if (!marked[i] && s[i] == 'R') {
          break;
        }
        i++;
      }
      while (j >= 0) {
        if (!marked[j] && s[j] == 'B') {
          break;
        }
        j--;
      }
      if (i < j) {
        ans++;
        marked[i] = marked[j] = true;
      }
    }

    cout << ans << endl;
  }

  return 0;
}
