// Codeforces 1870 (CodeTON Round 6 (Div. 1 + Div. 2, Rated, Prizes!)) — B. Friendly Arrays
// Submission: https://codeforces.com/contest/1870/submission/307569758

#include <algorithm>
#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <unordered_set>
#define maxn 200005
using namespace std;

int T, n, m;
int a[maxn], b[maxn];

void solve() {
  int or_value = 0;
  for (int i = 0; i < m; i++) {
    or_value |= b[i];
  }

  int xor_value = 0;
  for (int i = 0; i < n; i++) {
    xor_value ^= a[i];
  }

  int mini = xor_value, maxi = xor_value;

  xor_value = 0;
  for (int i = 0; i < n; i++) {
    xor_value ^= (a[i] | or_value);
  }
  mini = min(mini, xor_value);
  maxi = max(maxi, xor_value);

  cout << mini << ' ' << maxi << endl;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    for (int i = 0; i < m; i++) {
      cin >> b[i];
    }
    solve();
  }
  return 0;
}
