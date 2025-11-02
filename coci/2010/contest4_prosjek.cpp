#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

long long r[6];
string p;

long long gcd(long long x,long long y)
{
    if (x > y) swap(x,y);
    return (!x) ? y : gcd(y % x,x);
}

int main()
{
//  freopen("prosjek.in","r",stdin);
//  freopen("prosjek.ou","w",stdout);
    cin >> p;
    long long a = 1,b = 1;  a = p[0] - '0';
    for (int i = 2; i < p.size(); i++) if ('0' <= p[i] && p[i] <= '9')
    {
        b *= 10LL;  a = a * 10LL + p[i] - '0';
    }
    long long s = gcd(a,b);   a /= s;  b /= s;
    long long tmp = a - b;
    for (int i = 5; i > 1; i--)
    {
        r[i] = tmp / (i - 1);  tmp %= (i - 1);
    }
    r[1] = b;
    for (int i = 2; i <= 5; i++) r[1] -= r[i];
    for (int i = 1; i <= 5; i++) cout << r[i] << ' ';
}
