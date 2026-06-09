#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 100005;
int n, k, h;
int w[k_max_n], a[k_max_n], pos[k_max_n], ans[k_max_n], store[k_max_n];

bool cmp(int x, int y) {
  if (w[x] != w[y])
    return w[x] < w[y];
  return a[x] < a[y];
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> k >> h;
  for (int i = 0; i < n; i++)
    cin >> w[i];
  for (int i = 0; i < n; i++)
    cin >> a[i];

  for (int i = 0; i < n; i++)
    pos[i] = i;
  sort(pos, pos + n, cmp);

  double low = 0, high = 1e9;
  for (int iter = 0; iter < 200; iter++) {
    double mid = (low + high) / 2;
    int x = k;
    for (int i = n - 1; i >= 0; i--)
      if (x > 0) {
        int u = pos[i];
        if (a[u] * mid >= x) {
          store[x] = u;
          x--;
        }
      }
    if (x)
      low = mid;
    else {
      for (int i = 1; i <= k; i++)
        ans[i] = store[i];
      high = mid;
    }
  }
  for (int i = 1; i <= k; i++)
    cout << ans[i] + 1 << ' ';
}
