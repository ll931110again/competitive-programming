#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <stack>
#include <queue>
#include <vector>
using namespace std;

vector<int> v[10001];
vector< pair<int,int> > e;
queue<int> q;
int way[10001],len[10001];
bool fr[10001];

int main()
{
    int n,m,x,y,s;
//    freopen("stable.in","r",stdin);
//    freopen("stable.out","w",stdout);

    scanf("%d%d%d", &n, &m, &s);
    for (int i = 0; i < m; i++)
    {
        scanf("%d%d", &x, &y);
        e.push_back(make_pair(x,y));
    };
    sort(e.begin(),e.end());
    v[e[0].first].push_back(e[0].second);
    for (int i = 1; i < m; i++) if (e[i] != e[i - 1])
      v[e[i].first].push_back(e[i].second);

    memset(len, 0, sizeof(len));
    memset(way, 0, sizeof(way));
    memset(fr, true, sizeof(fr)); 
    fr[s] = false; way[s] = 1;  q.push(s);
    while (!q.empty())
    {
        int a = q.front(); q.pop();
        for (int i = 0; i < v[a].size(); i++)
        {
            int b = v[a][i];
            if (fr[b])
            {
                fr[b] = false; len[b] = len[a] + 1; way[b] = way[a]; q.push(b);
            }
            else if (len[b] == len[a] + 1)
              way[b] = min(200,way[b] + way[a]);
        };
    };
//    for (int i = 1; i <= n; i++) cout << len[i] << ' ' << way[i] << endl;

    int ret = 0;
    for (int i = 1; i <= n; i++)
      if (way[i] >= 2) ret++;
    printf("%d\n", ret);
};
