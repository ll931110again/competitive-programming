
#include <bits/stdc++.h>
#define maxn 200005
using namespace std;

int T, n, k;

int bits_count(int x) {
  if (x == 0) {
    return 1;
  }

  int ans = 0;
  while (x > 0) {
    ans++;
    x /= 2;
  }

  return ans;
}

void solve() {
  if (n == 1) {
    if (k == 1) {
      cout << "YES" << endl;
      cout << 0 << endl;
    } else {
      cout << "NO" << endl;
    }
    return;
  }

  k ^= n;
  if (bits_count(k) > bits_count(n - 1)) {
    cout << "NO" << endl;
    return;
  }

  vector<int> included;
  if (0 < k && k <= n - 1) {
    included = {0, k};
  } else if (k > 0) {
    included = {0, k ^ (n - 1), n - 1};
  } else {
    included = {0};
  }

  vector<int> ans;
  for (int i = 0; i < n; i++) {
    if (find(included.begin(), included.end(), i) == included.end()) {
      ans.push_back(i);
    }
  }
  for (auto x : included) {
    ans.push_back(x);
  }

  cout << "YES" << endl;
  for (auto x : ans) {
    cout << x << ' ';
  }
  cout << endl;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n >> k;
    solve();
  }

  return 0;
}
