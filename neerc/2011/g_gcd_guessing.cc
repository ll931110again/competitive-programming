/*
 * Solution for problem G: GCD Guessing.
 *
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 10005;
int n;
bool is_prime[k_max_n];
vector<int> primes;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("gcd.in", "r", stdin);
  freopen("gcd.out", "w", stdout);

  cin >> n;
  memset(is_prime, true, sizeof is_prime);
  for (int i = 2; i <= n; i++)
    if (is_prime[i]) {
      primes.push_back(i);
      for (int j = i + i; j <= n; j += i) {
        is_prime[j] = false;
      }
    }

  int low = 0, high = primes.size() - 1;
  int ans = 0;
  while (low <= high) {
    if (primes[low] * primes[high] <= n) {
      low++;
    }
    ans++;
    high--;
  }

  cout << ans << endl;
  return 0;
}
