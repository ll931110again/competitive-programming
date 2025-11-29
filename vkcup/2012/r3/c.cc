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
#define maxm 20002
using namespace std;
 
int cap[maxm],flow[maxm],cost[maxm],a[maxm],b[maxm],c[maxm];
int taskIdx[maxm];
 
int n,k,m,end;
vector< pair<int,int> > adj[maxm];
int INF = (1e9) + 4;
 
void add(int u,int v,int f,int c) {
  adj[u].push_back(make_pair(v,m));
  adj[v].push_back(make_pair(u,m + 1));
  cap[m] = f;  cost[m] = c;
  m += 2;
}
 
void maxFlow() {
  int trace[2005],traceEdge[2005],d[2005];
  bool inqueue[2005];
 
  while (1) {
    memset(trace,-1,sizeof(trace));
    trace[0] = -2;
    for (int i = 0; i <= end; i++) d[i] = (i == 0) ? 0 : INF;
    memset(inqueue,false,sizeof(inqueue));
 
    queue<int> q;
    q.push(0);
    inqueue[0] = true;
    while (!q.empty()) {
      int u = q.front();  q.pop();
      inqueue[u] = false;
      for (int i = 0; i < adj[u].size(); i++) {
        int v = adj[u][i].first,road = adj[u][i].second;
	if (cap[road] <= flow[road]) continue;
	int fcost = (flow[road] >= 0) ? cost[road] : - cost[road ^ 1];
	if (d[v] > d[u] + fcost) {
	  d[v] = d[u] + fcost;
	  trace[v] = u;
	  traceEdge[v] = road;
 
	  if (!inqueue[v]) q.push(v);
	  inqueue[v] = true;
	}
      }
    }
 
    if (d[end] >= INF) return;
    for (int v = end,u = trace[v]; v != 0; v = u,u = trace[v]) {
      int x = traceEdge[v];
      flow[x]++;
      flow[x ^ 1]--;
    }
  }
}
 
int main() {
  scanf("%d %d", &n, &k);
  for (int i = 0; i < n; i++) {
    scanf("%d %d %d", &a[i], &b[i], &c[i]);
    b[i] += a[i];
  }
  vector< pair<int,int> > v;
  for (int i = 0; i < n; i++) {
    v.push_back(make_pair(a[i],i));
    v.push_back(make_pair(b[i],i + n));
  }
  sort(v.begin(),v.end());
  end = -1;
  for (int i = 0; i < v.size(); i++) {
    if (!i || v[i].first > v[i - 1].first) end++;
    if (v[i].second < n) a[v[i].second] = end; else b[v[i].second - n] = end;
  }
  end++;
  for (int i = 0; i < end; i++) add(i,i + 1,k,0);
  for (int i = 0; i < n; i++) {
    taskIdx[i] = m;
    add(a[i],b[i],1,-c[i]);
  }
  maxFlow();
  for (int i = 0; i < n; i++) if (flow[taskIdx[i]] > 0) printf("1 "); else printf("0 ");
  printf("\n");
}
