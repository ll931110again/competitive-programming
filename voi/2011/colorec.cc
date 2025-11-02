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

int a[402][402];
int f[402][402][16];
int n;

int main()
{
//  freopen("COLOREC.INP","r",stdin);
//  freopen("COLOREC.OUT","w",stdout);
    scanf("%d", &n);
    memset(a,0,sizeof(a));
    for (int i = 0; i < n; i++)
    {
        int x,y,c;  scanf("%d %d %d", &x, &y, &c);
        x += 200;  y += 200;  a[x][y] = c;
    }
    memset(f,0,sizeof(f));
    for (int i = 0; i <= 400; i++)
      for (int j = i + 1; j <= 400; j++)
        for (int k = 0; k <= 400; k++)
          if (a[i][k] && a[j][k] && a[i][k] != a[j][k])
          {
                int mask = (1 << (a[i][k] - 1)) + (1 << (a[j][k] - 1));
                f[i][j][mask]++;
            }

    long long ret = 0;
    for (int i = 0; i <= 400; i++)
      for (int j = i + 1; j <= 400; j++)
        for (int t = 0; t < 16; t++) ret += 1LL * f[i][j][t] * f[i][j][15 - t];
    ret /= 2;
    cout << ret << endl;
}
