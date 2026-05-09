// H – Mapping the Undercity (stub)
// https://codesprintla25.kattis.com/problems/mappingtheundercity

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  if (!(cin >> N))
    return 0;
  vector<int> a(N), b(N);
  for (int i = 0; i < N; ++i)
    cin >> a[i];
  for (int i = 0; i < N; ++i)
    cin >> b[i];
  (void)a;
  (void)b;
  cout << 0 << '\n';
  return 0;
}
