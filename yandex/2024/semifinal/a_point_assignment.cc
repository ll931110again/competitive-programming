#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 100005;
int T, n;
int x[k_max_n], y[k_max_n];

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> x[i] >> y[i];
  }
  sort(x, x + n);
  sort(y, y + n);

  long long ret = 0;
  for (int i = 0; i < n; i++) {
    ret += abs(x[i] - (i + 1)) + abs(y[i] - (i + 1));
  }
  cout << ret << endl;
  return 0;
}
