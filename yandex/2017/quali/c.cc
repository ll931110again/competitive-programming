#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#define maxn 55
using namespace std;

int n;
int a[maxn];
vector<int> dependencies[maxn];
long long cnt[maxn];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> n;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  for (int i = 1; i <= n; i++) {
    int k;
    cin >> k;
    dependencies[i].resize(k);
    for (int j = 0; j < k; j++) {
      cin >> dependencies[i][j];
    }
  }

  long long run_1 = 0;
  for (int i = 1; i <= n; i++) {
    run_1 += a[i];
  }

  queue<int> q;
  q.push(1);
  cnt[1] = 1;
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (auto v : dependencies[u]) {
      q.push(v);
      cnt[v] += 2 * cnt[u];
    }
  }

  long long run_2 = 0;
  for (int i = 1; i <= n; i++) {
    run_2 += cnt[i] * a[i];
  }

  cout << run_1 << ' ' << run_2 << endl;
  return 0;
}