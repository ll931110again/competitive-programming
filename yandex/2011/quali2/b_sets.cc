// Codeforces 82 (Yandex.Algorithm 2011: Qualification 2) — B. Sets
// Submission: https://codeforces.com/contest/82/submission/439391

#include <bits/stdc++.h>
using namespace std;

vector<int> occur[205];
bool c[205][205];
int n;
int idx = 0;
int exist[205];
vector<int> store[205];

bool cmp(int px,int py)
{
    return occur[px] < occur[py];
}

void DFS(int u)
{
  store[idx].push_back(u);
  for (int i = 1; i <= 200; i++) if (exist[i] && c[u][i])
  {
      exist[i] = false;  DFS(i);
  }
}

int main()
{
    //freopen("b.in","r",stdin);
    //freopen("b.ou","w",stdout);

    scanf("%d", &n);
    int k = n * (n - 1)/2;
    for (int i = 0; i < k; i++)
    {
        int sz;  scanf("%d", &sz);
        for (int j = 0; j < sz; j++)
        {
            int u;  scanf("%d", &u);  occur[u].push_back(i);
        }

        if (n == 2)
        {
            printf("1 ");
            int flag = 0;
            for (int j = 1; j <= 200; j++) if (!occur[j].empty())
            {
                printf("%d\n", j);  flag = 1;  break;
            }
            printf("%d ", sz - 1);
            for (int j = 1; j <= 200; j++) if (!occur[j].empty())
            {
                flag++;  if (flag > 2) printf("%d ", j);
            }
            printf("\n");
            return 0;
        }
    }

    for (int i = 1; i <= 200; i++) sort(occur[i].begin(),occur[i].end());

    for (int i = 1; i <= 200; i++) if (!occur[i].empty())
      for (int j = i + 1; j <= 200; j++) if (i > j) c[i][j] = c[j][i]; else c[i][j] = (occur[i] == occur[j]);

    memset(exist,true,sizeof(exist));
    for (int i = 1; i <= 200; i++) if (!occur[i].empty() && exist[i])
    {
        DFS(i);  idx++;
    }

    for (int i = 0; i < n; i++)
    {
        printf("%d ", store[i].size());
        for (int j = 0; j < store[i].size(); j++) printf("%d ", store[i][j]);
        printf("\n");
    }
}
