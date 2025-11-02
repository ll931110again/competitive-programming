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

int n,h,a[100010];
int module = 1000000000,phi = module / 10 * 4;
long long ret = 1,p2 = 0,p5 = 0;

long long power(int x,int p,int mod)
{
    if (!p) return 1;
    long long q = power(x,p/2,mod);
    q = (q * q) % mod;
    if (p & 1) q = (q * x) % mod;
    return q;
}

void multiply(int x)
{
    while (x % 2 == 0)
    {
        p2++;  x /= 2;
    }
    while (x % 5 == 0)
    {
        p5++;  x /= 5;
    }
    ret = (1LL * ret * x) % module;
}

void divide(int x)
{
    while (x % 2 == 0)
    {
        p2--;  x /= 2;
    }
    while (x % 5 == 0)
    {
        p5--;  x /= 5;
    }
    ret = (1LL * ret * power(x,phi - 1,module)) % module;
}

int main()
{
//  freopen("TREELINE.INP","r",stdin);
//  freopen("TREELINE.OUT","w",stdout);
    scanf("%d %d", &n, &h);
    for (int i = 0; i < n; i++) scanf("%d", &a[i]);
    int way = n + 1,minh = a[n - 1];
    for (int i = n - 2; i >= 0; i--) if (a[i] > minh)
    {
        way = n - i;  break;
    }
    else minh = a[i];
    printf("%d\n", way);

    n++;
    for (int i = n + 2; i <= 2 * n; i++) multiply(i);
    for (int i = 1; i <= n; i++) divide(i);
    while (p2--) ret = (ret * 2LL) % module;
    while (p5--) ret = (ret * 5LL) % module;
    cout << ret << endl;
}
