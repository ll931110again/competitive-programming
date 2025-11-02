#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>
using namespace std;

int a[1010][1010];
int n,k;

int main()
{
//  freopen("BONUS.INP","r",stdin);
//  freopen("BONUS.OUT","w",stdout);
    scanf("%d %d", &n, &k);
    memset(a,0,sizeof(a));
    for (int i = 1; i <= n; i++)
      for (int j = 1; j <= n; j++) scanf("%d", &a[i][j]);
    for (int i = 2; i <= n; i++)
      for (int j = 1; j <= n; j++) a[i][j] += a[i - 1][j];
    for (int j = 2; j <= n; j++)
      for (int i = 1; i <= n; i++) a[i][j] += a[i][j - 1];

    int ret = 0;
    for (int i = k; i <= n; i++)
      for (int j = k; j <= n; j++) ret = max(ret,a[i][j] - a[i - k][j] - a[i][j - k] + a[i - k][j - k]);
    printf("%d\n", ret);
}
