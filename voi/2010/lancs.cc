#include <algorithm>
#include <bitset>
#include <cmath>
#include <ctime>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <queue>
#include <vector>
#include <utility>
using namespace std;

int a[1010],b[1010],f[1010][1010];

int main()
{
    int n,m;
    scanf("%d %d", &n, &m);
    for (int i = 1; i <= n; i++) scanf("%d", &a[i]);
    for (int i = 1; i <= m; i++) scanf("%d", &b[i]);
    memset(f,0,sizeof(f));
    for (int i = 1; i <= n; i++) f[i][1] = (a[i] == b[1]) ? 1 : f[i - 1][1];
    for (int i = 1; i <= m; i++) f[1][i] = (a[1] == b[i]) ? 1 : f[1][i - 1];
    for (int i = 2; i <= n; i++)
      for (int j = 2; j <= m; j++)
      {
            f[i][j] = max(f[i - 1][j],f[i][j - 1]);
            if (a[i] == b[j]) f[i][j] = max(f[i][j],f[i - 2][j - 2] + 1);
      };
    printf("%d\n", f[n][m]);
};
