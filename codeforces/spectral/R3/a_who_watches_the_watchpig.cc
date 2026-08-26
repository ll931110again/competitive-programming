// Codeforces Spectral::Cup 2026 Round 3 — Who Watches the Watchpig
// https://codeforces.com/contest/2245/problem/A
//
// Sketch
// ------
// Need 2k <= n. Count L among the first k and R among the last k. O(n).

#include <bits/stdc++.h>
using namespace std;

int T, n, k;
string s;

int solve() {
  if (2 * k > n) {
    return -1;
  }

  int ans = 0;
  for (int i = 0; i < k; i++)
    if (s[i] == 'L') {
      ans++;
    }
  for (int i = n - 1; i >= n - k; i--)
    if (s[i] == 'R') {
      ans++;
    }

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> k;
    cin >> s;
    cout << solve() << endl;
  }

  return 0;
}
