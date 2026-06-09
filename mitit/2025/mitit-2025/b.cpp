
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int T, n;
long long a[k_max_n];

long long solve() {
  long long ans = 1e18;
  long long min_delta = a[1] - a[0];
  for (int i = 3; i < n; i++) {
    ans = min(ans, a[i] - a[i - 1] + min_delta);
    min_delta = min(min_delta, a[i - 1] - a[i - 2]);
  }

  return ans;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  int T;
  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    sort(a, a + n);

    cout << solve() << endl;
  }
  return 0;
}
