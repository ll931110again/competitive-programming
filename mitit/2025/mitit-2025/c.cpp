
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int T, n;
string s;
bool vis[k_max_n];

void solve() {
  int even_count = 0;
  for (auto c : s) {
    if (c == 'E') {
      even_count++;
    }
  }

  if (even_count % 2) {
    cout << "NO" << endl;
    return;
  }

  for (int i = 1; i <= 2 * n; i++) {
    vis[i] = false;
  }

  cout << "YES" << endl;

  int even_value = 2, odd_value = 1;
  even_count = 0;
  for (auto c : s) {
    if (c == 'O') {
      cout << even_value << ' ' << odd_value << endl;
      vis[even_value] = vis[odd_value] = true;
    } else {
      even_count++;
      if (even_count % 2) {
        cout << odd_value << ' ' << odd_value + 2 << endl;
        vis[odd_value] = vis[odd_value + 2] = true;
      } else {
        cout << even_value << ' ' << even_value + 2 << endl;
        vis[even_value] = vis[even_value + 2] = true;
      }
    }

    while (vis[even_value]) {
      even_value += 2;
    }
    while (vis[odd_value]) {
      odd_value += 2;
    }
  }
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  int T;
  cin >> T;
  while (T--) {
    cin >> n;
    cin >> s;
    solve();
  }
  return 0;
}
