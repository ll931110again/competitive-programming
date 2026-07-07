// AHC048 A - Mixing on the Palette: https://atcoder.jp/contests/ahc048/tasks/ahc048_a
//
// Single merged well; per target mix nearest tubes then deliver 1g.

#include <bits/stdc++.h>
#include <cmath>
#include <iomanip>
using namespace std;

using i64 = long long;

struct RGB {
  double c, m, y;
};

int main() {
  ios::sync_with_stdio(false);
  cout.setf(ios::fixed);
  cout << setprecision(5);

  int N, K, H, T, D;
  cin >> N >> K >> H >> T >> D;
  vector<RGB> own(K);
  for (int i = 0; i < K; i++)
    cin >> own[i].c >> own[i].m >> own[i].y;
  vector<RGB> target(H);
  for (int i = 0; i < H; i++)
    cin >> target[i].c >> target[i].m >> target[i].y;

  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N - 1; j++)
      cout << 0 << (j + 1 == N - 1 ? '\n' : ' ');
  }
  for (int i = 0; i < N - 1; i++) {
    for (int j = 0; j < N; j++)
      cout << 0 << (j + 1 == N ? '\n' : ' ');
  }

  auto best_tube = [&](const RGB& t) {
    int id = 0;
    double best = 1e100;
    for (int k = 0; k < K; k++) {
      const double e = hypot(t.c - own[k].c, t.m - own[k].m) + fabs(t.y - own[k].y);
      if (e < best) {
        best = e;
        id = k;
      }
    }
    return id;
  };

  for (int h = 0; h < H; h++) {
    const int k = best_tube(target[h]);
    const int r = h % N, c = (h / N) % N;
    cout << "1 " << r << ' ' << c << ' ' << k << '\n';
    cout << "2 " << r << ' ' << c << '\n';
  }
  return 0;
}
