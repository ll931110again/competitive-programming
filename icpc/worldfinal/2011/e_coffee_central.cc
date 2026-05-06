#include <bits/stdc++.h>

using namespace std;

static inline int idx2(int i, int j, int stride) { return i * stride + j; }

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int tc = 1;

  int dx, dy;
  int n, q;
  while (cin >> dx >> dy >> n >> q) {
    if (dx == 0 && dy == 0 && n == 0 && q == 0) break;

    const int Umin = 2;
    const int Umax = dx + dy;
    const int Usize = Umax - Umin + 1;

    const int Vmin = 1 - dy;
    const int Vmax = dx - 1;
    const int Vsize = Vmax - Vmin + 1;

    vector<int> cnt((Usize + 1) * (Vsize + 1), 0); // 1-based for prefix

    for (int i = 0; i < n; i++) {
      int x, y;
      cin >> x >> y;
      int u = x + y;
      int v = x - y;
      int ui = (u - Umin) + 1;
      int vi = (v - Vmin) + 1;
      cnt[idx2(ui, vi, Vsize + 1)]++;
    }

    // 2D prefix sum in-place in cnt
    for (int i = 1; i <= Usize; i++) {
      int rowBase = i * (Vsize + 1);
      int prevRowBase = (i - 1) * (Vsize + 1);
      int run = 0;
      for (int j = 1; j <= Vsize; j++) {
        run += cnt[rowBase + j];
        cnt[rowBase + j] = run + cnt[prevRowBase + j];
      }
    }

    auto rectSum = [&](int uL, int uR, int vL, int vR) -> int {
      if (uL > uR || vL > vR) return 0;
      uL = max(uL, Umin);
      uR = min(uR, Umax);
      vL = max(vL, Vmin);
      vR = min(vR, Vmax);
      if (uL > uR || vL > vR) return 0;
      int a = (uL - Umin) + 1;
      int b = (uR - Umin) + 1;
      int c = (vL - Vmin) + 1;
      int d = (vR - Vmin) + 1;
      int stride = Vsize + 1;
      auto at = [&](int ui, int vi) -> int { return cnt[idx2(ui, vi, stride)]; };
      return at(b, d) - at(a - 1, d) - at(b, c - 1) + at(a - 1, c - 1);
    };

    cout << "Case " << tc++ << ":\n";

    for (int qi = 0; qi < q; qi++) {
      int m;
      cin >> m;

      int best = -1;
      int bestX = 1, bestY = 1;

      for (int y = 1; y <= dy; y++) {
        for (int x = 1; x <= dx; x++) {
          int u = x + y;
          int v = x - y;
          int val = rectSum(u - m, u + m, v - m, v + m);
          if (val > best) {
            best = val;
            bestX = x;
            bestY = y;
          }
        }
      }

      cout << best << " (" << bestX << "," << bestY << ")\n";
    }
  }

  return 0;
}
