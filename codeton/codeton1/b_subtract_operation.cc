// Codeforces 1656 (CodeTON Round 1 (Div. 1 + Div. 2, Rated, Prizes!)) — B. Subtract Operation
// Submission: https://codeforces.com/contest/1656/submission/150734646

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int T, n;
long long K;
unordered_set<long long> elems;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    bool ok = false;
    elems.clear();
    scanf("%d %lld", &n, &K);
    for (int i = 0; i < n; i++) {
      long long x;
      scanf("%lld", &x);
      if (elems.count(x + K) || elems.count(x - K)) {
        ok = true;
      }
      elems.insert(x);
    }
    if (ok) {
      printf("YES\n");
    } else {
      printf("NO\n");
    }
  }
  return 0;
}
