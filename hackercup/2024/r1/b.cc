#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 10000005;
int T, n;
int dp[k_max_n];
bool primes[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  memset(primes, true, sizeof primes);
  primes[0] = primes[1] = false;
  for (int i = 2; i < k_max_n; i++)
    if (primes[i]) {
      for (int j = i + i; j < k_max_n; j += i) {
        primes[j] = false;
      }
    }

  for (int i = 2; i < k_max_n; i++) {
    dp[i] = dp[i - 1];
    if (primes[i] && primes[i - 2]) {
      dp[i]++;
    }
    // special case for p = 2, i = 5
    if (i == 5) {
      dp[i]++;
    }
  }

  cin >> T;
  for (int it = 1; it <= T; it++) {
    cin >> n;
    cout << "Case #" << it << ": " << dp[n] << '\n';
  }
  return 0;
}
