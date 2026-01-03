#include <stack>
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>
#define mod 10007
using namespace std;

struct CowsMooing {
    int power(int x,int p) {
      int ret = 1,y = x;
      while (p > 0) {
        if (p & 1) ret = (ret * y) % mod;
        y = (y * y) % mod;
        p >>= 1;
      }
      return ret;
    }

    int inverse(int x) {
      return power(x,mod - 2);
    }

    int gcd(int x,int y) {
      if (!x) return y;
      if (x > y) {
        swap(x, y);
      }
      return gcd(y % x,x);
    }

    vector<int> getDistribution(vector<string> patterns) {
      int n = patterns.size();
      vector<int> results(n + 1,0);

      int T = 3235;  // 50! modulo 10007
      int R = 32 * 27 * 25;
      int TR = (T * inverse(R)) % mod;
      for (int c = 0; c < R; c++) {
        // solve for each c
        int t[n + 1];
        int counter[n + 1][51];
        memset(counter,0,sizeof(counter));
        // counter[i][j]: with index i, how many cows are mooing in the j-th moment
        for (int i = 0; i < n; i++) {
          int s = patterns[i].size();         
          t[i] = s/gcd(R,s);
          if (t[i] == 7) t[i] = 49;
          for (int j = 0; j < t[i]; j++)
            counter[i][j] = (patterns[i][(c + R * j) % s] == 'M');
        }

        bool exist[n + 1];
        memset(exist,true,sizeof(exist));
        // exist[i] = true means that only cow i exists
        for (int i = 0; i < n; i++) if (exist[i])
          for (int j = i + 1; j < n; j++) if (exist[j] && t[i] == t[j]) {
            exist[j] = false;
            for (int k = 0; k < t[i]; k++) counter[i][k] += counter[j][k];
          }

        int product = 1;
        int prop[n + 1];  // how many seconds that i cows are mooing at the same time
        memset(prop,0,sizeof(prop));
        prop[0] = 1;
        for (int i = 0; i < n; i++) if (exist[i]) {
          int prop2[n + 1],prop3[n + 1];
          memset(prop2,0,sizeof(prop2));
          memset(prop3,0,sizeof(prop3));
          for (int j = 0; j < t[i]; j++) prop2[counter[i][j]]++;
          for (int a = 0; a <= n; a++)
            for (int b = 0; a + b <= n; b++)
          prop3[a + b] = (prop3[a + b] + prop[a] * prop2[b]) % mod;
          product = (product * t[i]) % mod;
          for (int j = 0; j <= n; j++) prop[j] = prop3[j];
        }
        int ratio = (TR * inverse(product)) % mod;
        for (int i = 0; i <= n; i++)
          results[i] = (results[i] + ratio * prop[i]) % mod;
      }
      return results;
    }
};


<%:testing-code%>// Edited by VimCoder 0.3.1
// http://www.dogcows.com/vimcoder