// Google Code Jam 2011 — Round 2 — A. Airport Walkways

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int X, S, R, t, N;
    cin >> X >> S >> R >> t >> N;

    vector<double> seg(101, 0.0);
    int pos = 0;
    vector<tuple<int, int, int>> w(N);
    for (int i = 0; i < N; i++) {
      int B, E, wi;
      cin >> B >> E >> wi;
      w[i] = {B, E, wi};
    }
    sort(w.begin(), w.end());

    for (auto [B, E, wi] : w) {
      if (B > pos)
        seg[0] += B - pos;
      seg[wi] += E - B;
      pos = E;
    }
    if (pos < X)
      seg[0] += X - pos;

    double res = 0.0;
    double run_left = t;
    for (int i = 0; i <= 100; i++) {
      double run_time = min(run_left, seg[i] / (i + R));
      double walk_dist = seg[i] - run_time * (i + R);
      double walk_time = walk_dist / (i + S);
      res += run_time + walk_time;
      run_left -= run_time;
    }

    cout << "Case #" << tc << ": " << fixed << setprecision(6) << res << '\n';
  }
  return 0;
}
