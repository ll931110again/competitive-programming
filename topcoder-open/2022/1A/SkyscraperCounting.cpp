#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#define mod 1000000007
#define maxn 105
using namespace std;
 
long long binom[maxn][maxn];
long long fact[maxn];
 
class SkyscraperCounting {
public:
  int count(string visibility) {
    if (visibility[0] == 'X') {
      return 0;
    }
    
    int n = visibility.size();
    fact[0] = 1;
    for (int i = 1; i <= n; i++) {
      fact[i] = fact[i - 1] * i % mod;
    }    
    binom[0][0] = 1;
    for (int i = 1; i < maxn; i++) {
      for (int j = 0; j <= i; j++) {
        binom[i][j] = binom[i - 1][j];
        if (j) {
          binom[i][j] += binom[i - 1][j - 1];
          binom[i][j] %= mod;
        }
      }
    }
    
    long long ret = 1;
    int last = n;
    for (int i = n - 1; i >= 0; i--) if (visibility[i] == 'O') {
      int x = i, y = last - i - 1;
      ret = (ret * binom[x + y][y]) % mod;
      ret = (ret * fact[y]) % mod;
      last = i;
    }
    return ret;
  }
};
 
 
 
//Powered by KawigiEdit 2.1.4 (beta) modified by pivanof!