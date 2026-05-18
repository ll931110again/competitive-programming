#include <bits/stdc++.h>
using namespace std;

class GuessTheNumberGame {
  static const int MOD = 1000000007;

public:
  int possibleClues(int n) {
    vector<bool> isPrime(n + 1, true);
    isPrime[0] = isPrime[1] = false;
    vector<int> primes;
    for (int i = 2; i <= n; i++) {
      if (!isPrime[i])
        continue;
      primes.push_back(i);
      for (long long j = 1LL * i * i; j <= n; j += i)
        isPrime[(int)j] = false;
    }

    long long ans = 1;
    for (int p : primes) {
      int cnt = 0;
      for (long long x = 1; x <= n; x *= p)
        cnt++;
      ans = ans * cnt % MOD;
    }
    return (int)ans;
  }
};
