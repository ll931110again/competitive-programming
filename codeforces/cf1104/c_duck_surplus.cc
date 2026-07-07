
#include <bits/stdc++.h>
#define maxn 200005
using namespace std;

int T, n;
int a[maxn];

long long solve() {
    long long ans = 0;
    for (int i = 0; i < n; i++) {
        if (ans <= a[i]) {
            ans = a[i];
        } else {
            ans += a[i];
        }
    }
    return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }
    cout << solve() << endl;
  }
  return 0;
}
