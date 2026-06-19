// #pragma comment(linker, "/STACK:16777216")

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int m, n;
int a[100010];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //    freopen("lj.in","r",stdin);
  //    freopen("lj.ou","w",stdout);
  cin >> m >> n;
  for (int i = 1; i <= n; i++)
    cin >> a[i];
  sort(a + 1, a + n + 1);
  for (int i = n; i > 0; i--) {
    int delta = a[i] - a[i - 1], cnt = n - i + 1;
    if (m / cnt >= delta)
      m -= cnt * delta;
    else {
      delta = m / cnt;
      a[i] -= delta;
      m %= cnt;
      i64 ret = 0;
      for (int j = 1; j < i; j++)
        ret += 1LL * a[j] * a[j];
      ret += 1LL * m * (a[i] - 1) * (a[i] - 1) + 1LL * (cnt - m) * a[i] * a[i];
      cout << ret << endl;
      return 0;
    }
  }
}
