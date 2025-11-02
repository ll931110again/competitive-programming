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

long long n;
long long fibo[90];
int cnt;

int main()
{
//    freopen("hrpa.in","r",stdin);
//    freopen("hrpa.ou","w",stdout);
    cin >> n;
    fibo[0] = fibo[1] = 1;
    for (int i = 2; ; i++)
    {
        fibo[i] = fibo[i - 1] + fibo[i - 2];
        if (fibo[i] >= n) 
        {
          cnt = i;  break;
        }
    }   
    long long ret = n;
    for (int i = cnt; i >= 0; i--) if (n >= fibo[i])
    {
        n -= fibo[i];  ret = fibo[i];
    } 
    cout << ret << endl;
}
