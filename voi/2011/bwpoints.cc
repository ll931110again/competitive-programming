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

int n;
vector< pair<int,int> > v;

int main()
{
//  freopen("BWPOINTS.INP","r",stdin);
//  freopen("BWPOINTS.OUT","w",stdout);
    scanf("%d", &n);
    for (int i = 0; i < n; i++)
    {
        int x;  scanf("%d", &x);  v.push_back(make_pair(x,0));
    }
    for (int i = 0; i < n; i++)
    {
        int x;  scanf("%d", &x);  v.push_back(make_pair(x,1));
    }
    sort(v.begin(),v.end());
    int col = -1,ret = 0;
    for (int i = 0; i < v.size(); i++)
      if (col < 0 || col == v[i].second) col = v[i].second; else
      {
            ret++;  col = -1;
        }
    printf("%d\n", ret);
}
