// P – Popcorn Button (stub — full solution needs BFS on microwave states)
// https://open.kattis.com/problems/popcornbutton

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, q;
  if (!(cin >> n >> q))
    return 0;
  vector<string> broken(n);
  for (int i = 0; i < n; ++i)
    cin >> broken[i];
  vector<int> xs(q);
  for (int i = 0; i < q; ++i)
    cin >> xs[i];
  for (int x : xs) {
    (void)x;
    cout << -1 << '\n';
  }
  return 0;
}
