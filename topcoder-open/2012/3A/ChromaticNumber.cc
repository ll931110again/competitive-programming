#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>
#include <cstring>
#define maxn 55
using namespace std;
 
bool ok[55][55];
bool vis[55];
int n;
vector<int> store;
 
struct ChromaticNumber {
  int minColors(vector <string> graph) {
    n = graph.size();
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++) ok[i][j] = (graph[i][j] == 'Y');
 
    int ret = 0;
    memset(vis,true,sizeof(vis));
    for (int i = 0; i < n; i++) if (vis[i]) {
      store.clear();
      DFS(i);    
      if (check3(store)) ret++; else ret += (1 + store.size())/2;
    }
    return ret;
  }
 
  bool check3(vector<int> store) {
    if (store.size() != 3) return false;
    for (int i = 0; i < 3; i++) {
      int u = store[i],v = store[(i + 1) % 3];
      if (ok[u][v]) return false;
    }
    return true;
  }
 
  void DFS(int u) {
    if (!vis[u]) return;
    vis[u] = false;  store.push_back(u);
    for (int v = 0; v < n; v++) if (!ok[u][v]) DFS(v);
  }
};
 
 
// Edited by VimCoder 0.3.2
// http://www.dogcows.com/vimcoder
