// Codeforces 1896 (CodeTON Round 7 (Div. 1 + Div. 2, Rated, Prizes!)) — B. AB Flipping
// Submission: https://codeforces.com/contest/1896/submission/336566608

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int T, n;
string s;
bool enabled[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    cin >> s;
    for (int i = 0; i < n; i++) {
      enabled[i] = false;
    }

    queue<int> q;
    for (int i = 0; i + 1 < n; i++) {
      if (s[i] == 'A' && s[i + 1] == 'B') {
        q.push(i);
      }
    }

    int ans = 0;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      ans++;

      swap(s[u], s[u + 1]);
      enabled[u] = true;
      if (u > 0 && s[u - 1] == 'A' && s[u] == 'B' && !enabled[u - 1]) {
        q.push(u - 1);
      }
      if (u + 2 < n && s[u + 1] == 'A' && s[u + 2] == 'B' && !enabled[u + 1]) {
        q.push(u + 1);
      }
    }

    cout << ans << endl;
  }

  return 0;
}
