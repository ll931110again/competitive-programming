
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 105;
int T, n;
long long a[k_max_n];

long long query(int i, int j) {
  cout << "? " << i << ' ' << j << endl;
  cout << flush;
  long long x;
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

long long gcd(long long x, long long y) {
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
        long long x = query(i, i + 1);
        long long y = query(i, i + 2);
        a[i] = gcd(x, y);
        a[i + 1] = x / a[i];
        a[i + 2] = y / a[i];
        i += 3;
      } else {
        long long x = query(1, i);
        a[i] = x / a[1];
        i++;
      }
    }
    output();
  }
  return 0;
}
