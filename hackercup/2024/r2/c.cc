#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 805;
int T;
int m, n;
i64 K;

int a[k_max_n][k_max_n];
unordered_map<int, vector<pair<int, int>>> buckets;

int tx[k_max_n][k_max_n];

void update(int x, int y, int delta) {
  for (int dx = x; dx <= m; dx += (dx & -dx)) {
    for (int dy = y; dy <= n; dy += (dy & -dy)) {
      tx[dx][dy] += delta;
    }
  }
}

i64 get_single(int x, int y) {
  i64 ans = 0;
  for (int dx = x; dx > 0; dx -= (dx & -dx)) {
    for (int dy = y; dy > 0; dy -= (dy & -dy)) {
      ans += tx[dx][dy];
    }
  }
  return ans;
}

i64 get(int x, int y, int mid) {
  int lx = max(1, x - mid);
  int ly = max(1, y - mid);
  int rx = min(m, x + mid);
  int ry = min(n, y + mid);

  i64 ans = get_single(rx, ry) - get_single(lx - 1, ry) - get_single(rx, ly - 1) +
            get_single(lx - 1, ly - 1);
  return (rx - lx + 1) * (ry - ly + 1) - ans;
}

i64 count_valid_configuration(int mid) {
  i64 ret = 0;
  for (auto bucket : buckets) {
    for (auto item : bucket.second) {
      update(item.first, item.second, 1);
    }

    for (auto item : bucket.second) {
      auto value = get(item.first, item.second, mid);
      ret += value;
    }

    for (auto item : bucket.second) {
      update(item.first, item.second, -1);
    }
  }

  return ret;
}

int solve() {
  buckets.clear();

  cin >> m >> n >> K;
  for (int i = 1; i <= m; i++) {
    for (int j = 1; j <= n; j++) {
      cin >> a[i][j];
      buckets[a[i][j]].push_back({i, j});
    }
  }

  int low = 1, high = max(m, n), ans = high;
  while (low <= high) {
    int mid = (low + high) / 2;
    if (count_valid_configuration(mid) >= K) {
      ans = mid;
      high = mid - 1;
    } else {
      low = mid + 1;
    }
  }

  return ans;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  for (int it = 1; it <= T; it++) {
    cerr << it << endl;
    cout << "Case #" << it << ": " << solve() << endl;
  }
  return 0;
}
