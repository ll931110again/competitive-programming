#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 105;
long long f[k_max_n], g[k_max_n];
int n;
string s;

void populate() {
  for (int i = 0; i + 1 < n; i++) {
    if (s[i] == 'A' && s[i + 1] == 'A') {
      f[i + 1] += f[i];
      f[i + 1] += g[i];
    } else if (s[i] == 'A' && s[i + 1] == 'B') {
      f[i + 1] += f[i];
      f[i + 1] += g[i];
      g[i + 1] += g[i];
    } else if (s[i] == 'B' && s[i + 1] == 'A') {
      g[i + 1] += f[i];
      f[i + 1] += g[i];
    } else if (s[i] == 'B' && s[i + 1] == 'B') {
      f[i + 1] += g[i];
      g[i + 1] += g[i];
    }
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> s;
  n = s.size();

  long long ans = 0;
  f[0] = 1;
  populate();

  if (s[n - 1] == 'B') {
    ans += g[n - 1];
  } else {
    ans += f[n - 1] + g[n - 1];
  }

  memset(f, 0, sizeof f);
  memset(g, 0, sizeof g);
  g[0] = 1;
  populate();

  if (s[0] == 'A' && s[n - 1] == 'B') {
    ans += f[n - 1];
  } else if (s[0] == 'B') {
    ans += g[n - 1];
  }

  cout << ans << endl;
  return 0;
}
