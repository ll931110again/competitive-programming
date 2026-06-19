#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int n, h, a[100010];
int module = 1000000000, phi = module / 10 * 4;
i64 ret = 1, p2 = 0, p5 = 0;

i64 power(int x, int p, int mod) {
  if (!p)
    return 1;
  i64 q = power(x, p / 2, mod);
  q = (q * q) % mod;
  if (p & 1)
    q = (q * x) % mod;
  return q;
}

void multiply(int x) {
  while (x % 2 == 0) {
    p2++;
    x /= 2;
  }
  while (x % 5 == 0) {
    p5++;
    x /= 5;
  }
  ret = (1LL * ret * x) % module;
}

void divide(int x) {
  while (x % 2 == 0) {
    p2--;
    x /= 2;
  }
  while (x % 5 == 0) {
    p5--;
    x /= 5;
  }
  ret = (1LL * ret * power(x, phi - 1, module)) % module;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //  freopen("TREELINE.INP","r",stdin);
  //  freopen("TREELINE.OUT","w",stdout);
  cin >> n >> h;
  for (int i = 0; i < n; i++)
    cin >> a[i];
  int way = n + 1, minh = a[n - 1];
  for (int i = n - 2; i >= 0; i--)
    if (a[i] > minh) {
      way = n - i;
      break;
    } else
      minh = a[i];
  cout << way << '\n';

  n++;
  for (int i = n + 2; i <= 2 * n; i++)
    multiply(i);
  for (int i = 1; i <= n; i++)
    divide(i);
  while (p2--)
    ret = (ret * 2LL) % module;
  while (p5--)
    ret = (ret * 5LL) % module;
  cout << ret << endl;
}
