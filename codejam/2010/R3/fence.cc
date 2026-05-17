#include <algorithm>
#include <deque>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

using int64 = long long;

static const int64 kInf = (int64)4e18;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int64 L;
    int N;
    cin >> L >> N;
    vector<int64> B(N);
    for (int i = 0; i < N; i++)
      cin >> B[i];

    int64 g = 0;
    for (int64 x : B)
      g = std::gcd(g, x);
    if (L % g != 0) {
      cout << "Case #" << tc << ": IMPOSSIBLE\n";
      continue;
    }

    int64 A = *max_element(B.begin(), B.end());
    vector<int> shorter;
    for (int64 x : B)
      if (x < A)
        shorter.push_back((int)x);

    int64 p = L / A;
    int64 q = L % A;

    if (shorter.empty()) {
      if (L % A == 0)
        cout << "Case #" << tc << ": " << (L / A) << "\n";
      else
        cout << "Case #" << tc << ": IMPOSSIBLE\n";
      continue;
    }

    vector<int64> dist((size_t)A, kInf);
    dist[0] = 0;
    deque<int> dq;
    dq.push_front(0);

    while (!dq.empty()) {
      int r = dq.front();
      dq.pop_front();
      int64 d = dist[(size_t)r];
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

    if (dist[(size_t)q] >= kInf) {
      cout << "Case #" << tc << ": IMPOSSIBLE\n";
      continue;
    }

    int64 ans = p + dist[(size_t)q];
    cout << "Case #" << tc << ": " << ans << "\n";
  }
  return 0;
}
