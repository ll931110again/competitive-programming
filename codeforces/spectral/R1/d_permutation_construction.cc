
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 200005;
int T, n;
int a[k_max_n];
i64 prefix[k_max_n];

int id[k_max_n], ans[k_max_n];

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }

    prefix[0] = 0;
    for (int i = 0; i < n; i++) {
      prefix[i + 1] = prefix[i] + a[i];
    }

    for (int i = 0; i < n; i++) {
      id[i] = i;
    }

    sort(id, id + n, [&](int u, int v) { return prefix[u] > prefix[v]; });

    for (int i = 0; i < n; i++) {
      ans[id[i]] = i;
    }

    for (int i = 0; i < n; i++) {
      cout << ans[i] + 1 << ' ';
    }

    cout << endl;
  }

  return 0;
}
