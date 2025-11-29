#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
#define maxn 50005
#define maxk 505
using namespace std;
 
int n,k,f[maxn][maxk];
long long ret = 0;
vector<int> adj[maxn];
 
void DFS(int u,int pre) {
  for (int i = 0; i < adj[u].size(); i++) {
    int v = adj[u][i];
    if (v == pre) continue;
    DFS(v,u);
  }
  f[u][0] = 1;
  for (int i = 0; i < adj[u].size(); i++) {
    int v = adj[u][i];
    if (v == pre) continue;
    for (int j = 0; j < k; j++) ret += 1LL * f[u][k - j - 1] * f[v][j];
    for (int j = 0; j < k; j++) f[u][j + 1] += f[v][j];
  }
}
 
int main() {
  scanf("%d %d", &n, &k);
  for (int i = 1; i < n; i++) {
    int u,v;
    scanf("%d %d", &u, &v);
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  DFS(1,-1);
  cout << ret << endl;
}
