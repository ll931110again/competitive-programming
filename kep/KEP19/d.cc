#include "bits/stdc++.h"
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int n;
pair<int, int> duration[k_max_n];
int events[k_max_n];
int interval[k_max_n];

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> duration[i].first >> duration[i].second;
    events[i] = i;
  }
  sort(events, events + n, [&](int u, int v) {
    if (duration[u].first != duration[v].first) {
      return duration[u].first < duration[v].first;
    }
    return u < v;
  });

  int endpoint = 0;
  queue<int> q;

  int i = 0;
  while (i < n) {
    if (q.empty()) {
      endpoint = max(endpoint, duration[events[i]].first);
    }

    while (i < n && duration[events[i]].first <= endpoint) {
      q.push(events[i]);
      i++;
    }

    while (!q.empty() && duration[q.front()].second < endpoint) {
      q.pop();
    }

    if (q.empty()) {
      continue;
    }

    int idx = q.front();
    if (duration[idx].first <= endpoint && endpoint <= duration[idx].second) {
      interval[idx] = duration[idx].second - endpoint + 1;
      endpoint = duration[idx].second + 1;
    }
  }

  int max_value = 0;
  for (int i = 0; i < n; i++) {
    max_value = max(max_value, interval[i]);
  }

  for (int i = 0; i < n; i++) {
    if (interval[i] == max_value) {
      cout << i + 1 << ' ';
    }
  }
  cout << endl;

  return 0;
}
