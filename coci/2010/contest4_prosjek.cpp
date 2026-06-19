#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

i64 r[6];
string p;

i64 gcd(i64 x, i64 y) {
  if (x > y)
    swap(x, y);
  return (!x) ? y : gcd(y % x, x);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //  freopen("prosjek.in","r",stdin);
  //  freopen("prosjek.ou","w",stdout);
  cin >> p;
  i64 a = 1, b = 1;
  a = p[0] - '0';
  for (int i = 2; i < p.size(); i++)
    if ('0' <= p[i] && p[i] <= '9') {
      b *= 10LL;
      a = a * 10LL + p[i] - '0';
    }
  i64 s = gcd(a, b);
  a /= s;
  b /= s;
  i64 tmp = a - b;
  for (int i = 5; i > 1; i--) {
    r[i] = tmp / (i - 1);
    tmp %= (i - 1);
  }
  r[1] = b;
  for (int i = 2; i <= 5; i++)
    r[1] -= r[i];
  for (int i = 1; i <= 5; i++)
    cout << r[i] << ' ';
}
