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
#include <cstring>
#define maxn 15
#define maxd 105
using namespace std;
 
long long ways[maxn][maxd];
 
class SingleOrDouble {
public:
  double probAlice(int N, int D, int A, int B) {
    memset(ways, 0, sizeof ways);    
    ways[0][0] = 1;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j <= N * D; j++) if (ways[i][j]) {
        for (int k = 1; k <= D; k++) {
          ways[i + 1][j + k] += ways[i][j];
        }
      }
    }
    
    long long total_ways = 0;
    for (int i = 0; i <= N * D; i++) {
      total_ways += ways[N][i];
    }
    double pa = 1.0 * ways[N][A] / total_ways, pb = 1.0 * ways[N][B] / total_ways;
    
    double pbad = pb * pb / (pa + pb * (pa + pb));
    return 1. - pbad;
  }
};
 
 
 
//Powered by KawigiEdit 2.1.4 (beta) modified by pivanof!