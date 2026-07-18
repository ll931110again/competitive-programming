
#include <bits/stdc++.h>
#define maxn 200005
using namespace std;

int T, n, c;
int a[maxn];

long long solve() {
  for (int i = 0; i < n; i++) {
    a[i] -= c;
  }
  sort(a, a + n);

  int m = (n + 1) / 2;
  long long ans = 0;
  for (int i = n - 1; (i >= n - m) || (a[i] > 0); i--) {
    ans += a[i];
  }

  return ans;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n >> c;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }

    cout << solve() << endl;
  }

  return 0;
}
