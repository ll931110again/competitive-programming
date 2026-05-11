#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <unordered_set>
#include <vector>
#define maxn 100005
using namespace std;

int T;
int a, b, p;
string s;
long long costs[maxn];
int nxt_differ[maxn];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> T;
  while (T--) {
    cin >> a >> b >> p;
    cin >> s;

    int n = s.size();
    nxt_differ[n - 1] = n - 1;
    for (int i = n - 2; i >= 0; i--) {
      nxt_differ[i] = (s[i] == s[i + 1]) ? nxt_differ[i + 1] : (i + 1);
    }

    costs[n - 1] = 0;
    for (int i = n - 2; i >= 0; i--) {
      costs[i] = costs[nxt_differ[i]] + ((s[i] == 'A') ? a : b);
    }

    for (int i = 0; i < n; i++)
      if (costs[i] <= p) {
        cout << i + 1 << endl;
        break;
      }
  }
  return 0;
}