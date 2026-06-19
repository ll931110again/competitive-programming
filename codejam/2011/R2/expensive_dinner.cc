// Google Code Jam 2011 — Round 2 — C. Expensive Dinner

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

vector<int> primes;

void sieve(int limit) {
  vector<bool> is_prime(limit + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (int i = 2; i * i <= limit; i++) {
    if (!is_prime[i])
      continue;
    for (int j = i * i; j <= limit; j += i)
      is_prime[j] = false;
  }
  for (int i = 2; i <= limit; i++) {
    if (is_prime[i])
      primes.push_back(i);
  }
}

i64 spread(i64 N) {
  if (N <= 1)
    return 0;
  i64 ans = 1;
  for (int p : primes) {
    i64 cur = 1LL * p * p;
    while (cur <= N) {
      ans++;
      if (cur > N / p)
        break;
      cur *= p;
    }
  }
  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  sieve(1000000);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    i64 N;
    cin >> N;
    cout << "Case #" << tc << ": " << spread(N) << '\n';
  }
  return 0;
}
