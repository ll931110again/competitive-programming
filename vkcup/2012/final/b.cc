#include <bits/stdc++.h>
#define maxn 1000005
#define maxk 26
#define maxQ 10005
using namespace std;
 
int nxt[maxn][maxk];
int maskValue[maxQ];
unordered_set<int> inMask;
unordered_map<int,int> counter;
string s;
int n,Q;
 
pair<int,int> order[26];
int cnt = 0;
 
int main() {
  cin >> s;
  n = s.size();
  memset(nxt,-1,sizeof(nxt));
  for (int i = n - 1; i >= 0; i--)
    for (int j = 0; j < 26; j++)
      nxt[i][j] = (s[i] == j + 'a') ? i : nxt[i + 1][j];
  scanf("%d", &Q);
  for (int it = 0; it < Q; it++) {
    string query;
    cin >> query;
    int mask = 0;
    for (int i = 0; i < query.size(); i++)
      mask |= 1 << (query[i] - 'a');
    inMask.insert(mask);    
    maskValue[it] = mask;
  }
  for (int i = 0; i < n; i++) {
    if (i && s[i] == s[i - 1]) continue;   
    cnt = 0;
    for (int j = 0; j < 26; j++) if (nxt[i][j] >= 0) order[cnt++] = make_pair(nxt[i][j],j);
    sort(order,order + cnt);
    int mask = 0;
    for (int j = 0; j < cnt; j++) {
      int pos = order[j].second;
      if (i && pos + 'a' == s[i - 1]) break;
      mask |= 1 << pos;
      if (inMask.find(mask) != inMask.end()) counter[mask]++;
    }    
  }
  for (int i = 0; i < Q; i++) printf("%d\n", counter[maskValue[i]]);
}
