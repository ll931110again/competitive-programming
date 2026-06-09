#include <bits/stdc++.h>
using namespace std;

namespace {

// Tree + one extra edge → unicyclic “Q graph” extraction (robbiesresearchquest).
// Placeholder — needs casework on cycle/tail attachment per query.

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, Q;
  cin >> N >> Q;
  vector<pair<int, int>> edges(N - 1);
  for (int i = 0; i < N - 1; i++) {
    cin >> edges[i].first >> edges[i].second;
  }
  for (int i = 0; i < Q; i++) {
    int a, b;
    cin >> a >> b;
    cout << -1 << '\n';
  }
  return 0;
}
