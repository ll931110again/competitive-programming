#include <bits/stdc++.h>
using namespace std;

class SieveOfEratosthenes {
  static const int kSieveLimit = 50000;
  vector<int> primes;

  void buildPrimes() {
    if (!primes.empty())
      return;
    vector<bool> comp(kSieveLimit + 1, false);
    for (int i = 2; i * i <= kSieveLimit; ++i) {
      if (!comp[i])
        for (int j = i * i; j <= kSieveLimit; j += i)
          comp[j] = true;
    }
    for (int i = 2; i <= kSieveLimit; ++i)
      if (!comp[i])
        primes.push_back(i);
  }

  int smallestPrimeFactor(int n) const {
    if (n < 2)
      return n;
    for (int p : primes) {
      if (p * p > n)
        break;
      if (n % p == 0)
        return p;
    }
    return n;
  }

  int largestPrimeAtMost(int n) const {
    if (n < 2)
      return 2;
    if (n <= kSieveLimit) {
      auto it = upper_bound(primes.begin(), primes.end(), n);
      return it == primes.begin() ? 2 : *--it;
    }
    if ((n & 1) == 0)
      --n;
    for (; n >= 2; n -= 2) {
      if (isPrime(n))
        return n;
    }
    return 2;
  }

  static bool isPrime(int n) {
    if (n < 2)
      return false;
    if (n % 2 == 0)
      return n == 2;
    for (int d = 3; 1LL * d * d <= n; d += 2)
      if (n % d == 0)
        return false;
    return true;
  }

public:
  SieveOfEratosthenes() {
    buildPrimes();
  }

  int lastScratch(int maxNum) {
    int root = (int)sqrt((double)maxNum);
    while (1LL * root * root > maxNum)
      --root;
    int p = largestPrimeAtMost(root);
    int limit = maxNum / p;
    for (int y = limit; y >= p; --y) {
      if (smallestPrimeFactor(y) >= p)
        return p * y;
    }
    return p * 2;
  }
};

#ifdef LOCAL_TEST
#include <iostream>
using namespace std;

int main() {
  auto test = [](int maxNum, int expected) {
    int got = SieveOfEratosthenes().lastScratch(maxNum);
    if (got != expected) {
      cerr << "FAIL maxNum=" << maxNum << ": got " << got << " expected " << expected << endl;
      exit(1);
    }
  };
  test(18, 15);
  test(5, 4);
  test(100, 91);
  test(400, 361);
  test(2000000000, 1999878319);
  test(8, 8);
  test(1999878319, 1999878319);
  test(1999878318, 1999073521);
  test(1111111111, 1110955561);
  test(841, 841);
  test(9, 9);
  test(7, 6);
  cerr << "all tests passed" << endl;
}
#endif
