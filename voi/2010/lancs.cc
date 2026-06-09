#include <bits/stdc++.h>
using namespace std;

namespace {

int a[1010], b[1010], f[1010][1010];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  for (int i = 1; i <= n; i++)
    cin >> a[i];
  for (int i = 1; i <= m; i++)
    cin >> b[i];
  memset(f, 0, sizeof(f));
  for (int i = 1; i <= n; i++)
    f[i][1] = (a[i] == b[1]) ? 1 : f[i - 1][1];
  for (int i = 1; i <= m; i++)
    f[1][i] = (a[1] == b[i]) ? 1 : f[1][i - 1];
  for (int i = 2; i <= n; i++)
    for (int j = 2; j <= m; j++) {
      f[i][j] = max(f[i - 1][j], f[i][j - 1]);
      if (a[i] == b[j])
        f[i][j] = max(f[i][j], f[i - 2][j - 2] + 1);
    };
  cout << f[n][m] << '\n';
};
