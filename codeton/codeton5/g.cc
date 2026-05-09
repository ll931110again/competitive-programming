#include <bits/stdc++.h>
using namespace std;

static constexpr int maxn = 5005;
static constexpr unsigned MOD = 1'000'000'007;
#include "../../lib/modint.h"
using Mint = ModInt<MOD>;

int n, m, v;
int a[maxn];

Mint cur[maxn], nxt[maxn];

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m >> v;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }

  cur[0] = 1;
  for (int i = 1; i <= n; i++) {
    Mint coef = Mint(i) / n * v;
    for (int j = 0; j <= n; j++) {
      nxt[j] = 0;
    }

    for (int j = 0; j < i; j++) {
      nxt[j + 1] += cur[j] * coef * (m - j);
      nxt[j] += cur[j] * (Mint(a[i]) + Mint(j) * v);
    }

    for (int j = 0; j <= n; j++) {
      cur[j] = nxt[j];
    }
  }

  Mint ans = 0;
  for (int i = 0; i <= n; i++) {
    ans += cur[i];
  }

  cout << ans << endl;
  return 0;
}