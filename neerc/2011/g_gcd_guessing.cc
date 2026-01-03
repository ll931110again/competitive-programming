/*
 * Solution for problem G: GCD Guessing.
 *
 */

#ifdef ONLINE_JUDGE
  #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <unordered_set>
#include <vector>
#define maxn 10005
using namespace std;

int n;
bool is_prime[maxn];
vector<int> primes;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("gcd.in", "r", stdin);
  freopen("gcd.out", "w", stdout);

  cin >> n;
  memset(is_prime, true, sizeof is_prime);
  for (int i = 2; i <= n; i++) if (is_prime[i]) {
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
