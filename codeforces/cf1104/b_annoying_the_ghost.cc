
#include <bits/stdc++.h>
#define maxn 2005
using namespace std;

int T, n;
int a[maxn], b[maxn], used[maxn];

int solve() {
  for (int i = 0; i < n; i++) {
    bool found = false;
    for (int j = 0; j < n; j++)
      if (a[i] <= b[j] && !used[j]) {
        a[i] = b[j];
        used[j] = true;
        found = true;
        break;
      }
    if (!found) {
      return -1;
    }
  }

  int ans = 0;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++)
      if (a[i] > a[j]) {
        ans++;
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

    for (int i = 0; i < n; i++) {
      cin >> b[i];
    }

    memset(used, false, sizeof used);

    cout << solve() << endl;
  }
  return 0;
}
