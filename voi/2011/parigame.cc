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

int a[502][502];
bool ret[502][502];
int T,n;

bool even(int x1,int y1,int x2,int y2)
{
    int sum = a[x2][y2] - a[x1 - 1][y2] - a[x2][y1 - 1] + a[x1 - 1][y1 - 1];
    sum %= 2;  sum = (sum + 2) % 2;
    return (sum == 0);
}

int main()
{
//  freopen("PARIGAME.INP","r",stdin);
//  freopen("PARIGAME.OUT","w",stdout);
    scanf("%d", &T);
    while (T--)
    {
        scanf("%d", &n);
        memset(a,0,sizeof(a));
        for (int i = 1; i <= n; i++)
          for (int j = 1; j <= n; j++)
          {
                int x;  scanf("%d", &x);  a[i][j] = (x % 2);
            }
        for (int i = 2; i <= n; i++)
          for (int j = 1; j <= n; j++) a[i][j] += a[i - 1][j];
        for (int j = 2; j <= n; j++)
          for (int i = 1; i <= n; i++) a[i][j] += a[i][j - 1];

        memset(ret,false,sizeof(ret));
        for (int i = 1; i <= n; i++)
          for (int j = 1; j <= n; j++)
          {
                if (even(i,1,i,j) && !ret[i - 1][j]) ret[i][j] = true;
                if (even(1,j,i,j) && !ret[i][j - 1]) ret[i][j] = true;
            }
        if (ret[n][n]) printf("YES\n"); else printf("NO\n");
    }
}
