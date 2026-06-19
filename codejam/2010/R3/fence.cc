#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
namespace {

const i64 k_inf = (i64)4e18;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    i64 L;
    int N;
    cin >> L >> N;
    vector<i64> B(N);
    for (int i = 0; i < N; i++)
      cin >> B[i];

    i64 g = 0;
    for (i64 x : B)
      g = gcd(g, x);
    if (L % g != 0) {
      cout << "Case #" << tc << ": IMPOSSIBLE\n";
      continue;
    }

    i64 A = *max_element(B.begin(), B.end());
    vector<int> shorter;
    for (i64 x : B)
      if (x < A)
        shorter.push_back((int)x);

    i64 p = L / A;
    i64 q = L % A;

    if (shorter.empty()) {
      if (L % A == 0)
        cout << "Case #" << tc << ": " << (L / A) << "\n";
      else
        cout << "Case #" << tc << ": IMPOSSIBLE\n";
      continue;
    }

    vector<i64> dist((size_t)A, k_inf);
    dist[0] = 0;
    deque<int> dq;
    dq.push_front(0);

    while (!dq.empty()) {
      int r = dq.front();
      dq.pop_front();
      i64 d = dist[(size_t)r];
      for (int b : shorter) {
        int sum = r + b;
        int nr = sum % A;
        int w = (sum < (int)A) ? 1 : 0;
        if (d + w < dist[(size_t)nr]) {
          dist[(size_t)nr] = d + w;
          if (w == 0)
            dq.push_front(nr);
          else
            dq.push_back(nr);
        }
      }
    }

    if (dist[(size_t)q] >= k_inf) {
      cout << "Case #" << tc << ": IMPOSSIBLE\n";
      continue;
    }

    i64 ans = p + dist[(size_t)q];
    cout << "Case #" << tc << ": " << ans << "\n";
  }
  return 0;
}
