#ifdef ONLINE_JUDGE
  #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#include <unordered_set>
#include <set>
#define maxn 200005
using namespace std;

int n;
int cnt[maxn], req_time[maxn];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> cnt[i];
  }
  for (int i = 0; i < n; i++) {
    cin >> req_time[i];
  }
  
  map<int, vector<int> > mp;
  for (int i = 0; i < n; i++) {
    mp[cnt[i]].push_back(req_time[i]);
  }

  int tick = 0;
  long long ans = 0;

  while (true) {
    auto it = mp.upper_bound(tick);
    if (it == mp.end()) {
      break;
    }

    tick = it->first;

    long long entry_sum = 0;
    priority_queue<int> pq;
    for (auto entry : mp[tick]) {
      pq.push(entry);
      entry_sum += entry;
    }

    while (pq.size() > 1) {
      auto entry = pq.top();
      pq.pop();

      entry_sum -= entry;
      ans += entry_sum;

      tick++;
      for (auto entry : mp[tick]) {
        pq.push(entry);
        entry_sum += entry;
      }
    }
  }

  cout << ans << endl;

  return 0;
}