// Codeforces 2039 (CodeTON Round 9 (Div. 1 + Div. 2, Rated, Prizes!)) — D. Shohag Loves GCD
// Submission: https://codeforces.com/contest/2039/submission/295467002

#include <algorithm>
#include <iostream>
#define maxn 100005
using namespace std;

int T;
int n, m;
int a[maxn];
int value[maxn];

void solve() {
  for (int i = 1; i <= n; i++) {
    value[i] = m - 1;
  }

  for (int i = 1; i <= n; i++) {
    for (int j = i + i; j <= n; j += i) {
      value[j] = min(value[j], value[i] - 1);
    }
  }

  for (int i = 1; i <= n; i++) {
    if (value[i] < 0) {
      cout << -1 << endl;
      return;
    }
  }

  for (int i = 1; i <= n; i++) {
    cout << a[value[i]] << ' ';
  }
  cout << endl;
}

int main() {
  cin >> T;
  while (T--) {
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
      cin >> a[i];
    }

    solve();
  }
  return 0;
}
