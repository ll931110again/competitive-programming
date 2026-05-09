// J – Popcorn Button (stub)
// https://codesprintla25.kattis.com/problems/popcornbutton

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int n, q;
  if (!(cin >> n >> q))
    return 0;
  cin.ignore();
  string line;
  getline(cin, line);
  getline(cin, line);
  while (q--) {
    int x;
    cin >> x;
    cout << -1 << '\n';
  }
  return 0;
}
