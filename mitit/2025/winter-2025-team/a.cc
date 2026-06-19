
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 105;
int T, n;
i64 a[k_max_n];

i64 query(int i, int j) {
  cout << "? " << i << ' ' << j << endl;
  cout << flush;
  i64 x;
  cin >> x;
  return x;
}

void output() {
  cout << "! ";
  for (int i = 1; i <= n; i++) {
    cout << a[i] << ' ';
  }
  cout << endl;
  cout << flush;
}

i64 gcd(i64 x, i64 y) {
  if (x > y) {
    swap(x, y);
  }
  if (x == 0) {
    return y;
  }
  return gcd(y % x, x);
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    int i = 1;
    while (i <= n) {
      if (i + 2 <= n) {
        i64 x = query(i, i + 1);
        i64 y = query(i, i + 2);
        a[i] = gcd(x, y);
        a[i + 1] = x / a[i];
        a[i + 2] = y / a[i];
        i += 3;
      } else {
        i64 x = query(1, i);
        a[i] = x / a[1];
        i++;
      }
    }
    output();
  }
  return 0;
}
