// R – Team Up! (stub)
// https://open.kattis.com/problems/teamup2

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, Q;
  if (!(cin >> N >> Q))
    return 0;
  vector<int> a(N);
  for (int i = 0; i < N; ++i)
    cin >> a[i];
  while (Q--) {
    int ty;
    cin >> ty;
    if (ty == 1) {
      int t, i;
      cin >> t >> i;
    } else {
      int t;
      cin >> t;
      cout << -1 << '\n';
    }
  }
  return 0;
}
