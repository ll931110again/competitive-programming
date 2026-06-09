#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 55;
int n;
int a[k_max_n];
vector<int> dependencies[k_max_n];
long long cnt[k_max_n];

} // namespace

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
