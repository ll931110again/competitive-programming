// Codeforces 1656 (CodeTON Round 1 (Div. 1 + Div. 2, Rated, Prizes!)) — C. Make Equal With Mod
// Submission: https://codeforces.com/contest/1656/submission/150742937

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int T, n, a[k_max_n];

bool solve() {
  sort(a, a + n);
  bool found1 = false;
  for (int i = 0; i < n; i++)
    if (a[i] == 1) {
      found1 = true;
      break;
    }

  if (!found1) {
    return true;
  }

  // Bad if found any consecutive pair
  for (int i = 0; i + 1 < n; i++)
    if (a[i + 1] - a[i] == 1) {
      return false;
    }

  return true;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    if (solve()) {
      printf("YES\n");
    } else {
      printf("NO\n");
    }
  }
  return 0;
}
