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
#define maxn 100005
using namespace std;
 
int init[maxn];
bool touch[maxn],ford[maxn],back[maxn];
vector<int> adj[maxn],rev[maxn];
int n,m;
 
void BFS() {
  queue<int> q;
  for (int i = 1; i <= n; i++) if (init[i] == 1) {
    ford[i] = true;
    q.push(i);
    touch[i] = true;
  }
  while (!q.empty()) {
    int u = q.front();  q.pop();
    for (int i = 0; i < adj[u].size(); i++) {
      int v = adj[u][i];
      if (init[v] == 1 || touch[v]) continue;
      ford[v] = true;
      touch[v] = true;
      q.push(v);
    }
  }
 
  memset(touch,false,sizeof(touch));
  for (int i = 1; i <= n; i++) if (init[i] == 2) {
    touch[i] = back[i] = true;
    q.push(i);
  }
  while (!q.empty()) {
    int u = q.front();  q.pop();
    if (init[u] == 1) continue;
    for (int i = 0; i < rev[u].size(); i++) {
      int v = rev[u][i];
      if (touch[v]) continue;
      back[v] = touch[v] = true;
      q.push(v);
    }
  }
 
  for (int i = 1; i <= n; i++) if (back[i] && ford[i]) printf("1\n"); else printf("0\n");
}
 
int main() {
  scanf("%d %d", &n, &m);
  for (int i = 1; i <= n; i++) scanf("%d", &init[i]);
  for (int i = 0; i < m; i++) {
    int u,v;
    scanf("%d %d", &u, &v);
    adj[u].push_back(v);
    rev[v].push_back(u);
  }
  BFS();
}
