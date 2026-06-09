#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 1000005;
int n, m;
int a[k_max_n], b[k_max_n];
int pos_b[k_max_n];

bool good(int i, int j, int k) {
  if (k < 0) {
    return false;
  }
  if (i <= j) {
    return (k > j || k < i);
  }
  return (k > j && k < i);
}

int length(int i, int j) {
  if (i <= j) {
    return j - i + 1;
  }
  return (j - i + 1 + n);
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  memset(pos_b, -1, sizeof pos_b);

  cin >> n >> m;
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  for (int i = 0; i < m; i++) {
    cin >> b[i];
    pos_b[b[i]] = i;
  }

  int ans = 0;
  int R = -1;
  for (int L = 0; L < n; L++) {
    if (R < 0 && pos_b[a[L]] >= 0) {
      R = L;
    }
    if (R >= 0) {
      while ((R + 1) % n != L && good(pos_b[a[L]], pos_b[a[R]], pos_b[a[(R + 1) % n]])) {
        R = (R + 1) % n;
      }
      ans = max(ans, length(L, R));
      if (L == R) {
        R = -1;
      }
    }
  }

  cout << ans << endl;
  return 0;
}
