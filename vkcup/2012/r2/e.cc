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
#define maxn 1000005
using namespace std;
 
int depth[maxn],next[maxn][26],linka[maxn],pos[maxn];
vector<int> adj[maxn];
bool status[maxn];
int n,Q,total = 1;
int begin[maxn],end[maxn],eval[maxn],cnt = 0,fenwick[2 * maxn];
 
string pattern[maxn];
 
void DFS(int u) {
  begin[u] = cnt++;
  for (int i = 0; i < adj[u].size(); i++) DFS(adj[u][i]);
  end[u] = cnt++;
 
  if (!eval[u]) return;
  for (int i = end[u]; i > 0; i -= i & -i) fenwick[i]++;
  for (int i = begin[u] - 1; i > 0; i -= i & -i) fenwick[i]--;
}
 
void AhoCorasick() {
  memset(next,-1,sizeof(next));
  // trie implementation
  for (int i = 0; i < n; i++) {
    int start = 0;
    for (int j = 0; j < pattern[i].size(); j++) {
      int ford = next[start][pattern[i][j] - 'a'];
      if (ford < 0) {
        ford = next[start][pattern[i][j] - 'a'] = total++;
	depth[total - 1] = depth[start] + 1;
      }
      start = ford;
    }
    eval[start] = 1;  pos[i + 1] = start;
  }
  // linka: the longest proper suffix
  queue<int> q;
  q.push(0);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int i = 0; i < 26; i++) if (next[u][i] >= 0) {
      int v = next[u][i];
      int x = linka[u];
      while (x) {
        int y = next[x][i];
	if (y < 0) x = linka[x]; else break;
      }
      linka[v] = max(next[x][i],0);
      if (linka[v] == v) linka[v] = 0;
      q.push(v);
      adj[linka[v]].push_back(v);
    }
 }
 DFS(0);
}
 
void update(string s,int sign) {
  stringstream w; w << s; int x;  w >> x;
  if (status[x] == sign) return;
  status[x] = sign;
 
  x = pos[x];
  int delta = (sign) ? 1 : -1;
  for (int i = end[x]; i > 0; i -= i & -i) fenwick[i] += delta;
  for (int i = begin[x] - 1; i > 0; i -= i & -i) fenwick[i] -= delta;
}
 
long long computeNode(int u) {
  long long ret = 0;
  int idx = begin[u];
  if (idx) for (int i = idx; i < cnt; i += i & -i) ret += fenwick[i];
  return ret;
}
 
void query(string s) {
  long long ret = 0;
  int start = 0;
  for (int i = 0; i < s.size(); i++) {
    while (start) {
      int ford = next[start][s[i] - 'a'];
      if (ford < 0) start = linka[start]; else break;
    }
    start = max(next[start][s[i] - 'a'],0);
    ret += computeNode(start);
  }
  cout << ret << endl;
}
 
int main() {
  scanf("%d %d", &Q, &n);
  for (int i = 0; i < n; i++) cin >> pattern[i];
  AhoCorasick();
  memset(status,true,sizeof(status));
  while (Q--) {
    string s;
    cin >> s;
    string t = s.substr(1);
    if (s[0] == '+') update(t,1);
    else if (s[0] == '-') update(t,0);
    else query(t);
  }
}
