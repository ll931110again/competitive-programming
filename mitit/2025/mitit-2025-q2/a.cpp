
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 500005;
int T, n;
int hold[k_max_n], cap[k_max_n];

bool solve() {
  queue<int> fulls;
  priority_queue<pair<int, int>> partials;

  for (int i = 0; i < n; i++) {
    if (hold[i] == cap[i]) {
      fulls.push(i);
    } else {
      partials.push({hold[i] - cap[i], i});
    }
  }

  while (!fulls.empty()) {
    int u = fulls.front();
    fulls.pop();

    int rem = cap[u];
    while (rem > 0 && !partials.empty()) {
      int v = partials.top().second;
      partials.pop();

      if (hold[v] != cap[v]) {
        int added = min(rem, cap[v] - hold[v]);
        rem -= added;
        hold[v] += added;

        if (hold[v] < cap[v]) {
          partials.push({hold[v] - cap[v], v});
        } else {
          fulls.push(v);
        }
      }
    }
  }

  for (int i = 0; i < n; i++)
    if (hold[i] < cap[i]) {
      return false;
    }

  return true;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> hold[i];
    }
    for (int i = 0; i < n; i++) {
      cin >> cap[i];
    }
    cout << (solve() ? "YES" : "NO") << endl;
  }

  return 0;
}
