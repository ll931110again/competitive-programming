// Codeforces 756 (8VC Venture Cup 2017 - Final Round) — A. Pavel and barbecue
// Submission: https://codeforces.com/contest/756/submission/335410373

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int n;
int a[k_max_n];
bool marked[k_max_n];
int flips;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }

  int cyc = 0;
  for (int i = 1; i <= n; i++)
    if (!marked[i]) {
      cyc++;
      int x = i;
      while (!marked[x]) {
        marked[x] = true;
        x = a[x];
      }
    }

  for (int i = 0; i < n; i++) {
    int x;
    cin >> x;
    flips ^= x;
  }

  int ans = ((cyc > 1) ? cyc : 0) + (flips ^ 1);
  cout << ans << endl;

  return 0;
}
