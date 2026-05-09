// C – Balancing the Council
// https://codesprintla25.kattis.com/problems/balancingthecouncil
//
// Seats are vertices of a regular N-gon centered at O. Pick M vertices;
// balanced iff their centroid is O, i.e. sum of position vectors = 0.
// For N even: impossible iff M == 1 (no singleton at center) or M == N-1
// (sum of all N vertices is 0, so any N-1 sum is −v ≠ 0).

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  long long N, M;
  if (!(cin >> N >> M))
    return 0;
  if (M == 1 || M == N - 1)
    cout << "No\n";
  else
    cout << "Yes\n";
  return 0;
}
