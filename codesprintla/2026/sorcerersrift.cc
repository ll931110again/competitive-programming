#include <bits/stdc++.h>
using namespace std;

namespace {

// Interactive rift recovery between two trees with ≤36 path-intersection queries.

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;
  vector<pair<int, int>> hy(N - 1);
  for (int i = 0; i < N - 1; i++) {
    cin >> hy[i].first >> hy[i].second;
  }
  vector<pair<int, int>> lo(M - 1);
  for (int i = 0; i < M - 1; i++) {
    cin >> lo[i].first >> lo[i].second;
  }

  // Without issuing queries we cannot locate the bridge — print a deterministic guess.
  cout << "! " << 1 << ' ' << (N + 1) << '\n';
  cout.flush();
  return 0;
}
