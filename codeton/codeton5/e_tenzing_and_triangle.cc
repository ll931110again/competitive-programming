// Codeforces 1842 (CodeTON Round 5 (Div. 1 + Div. 2, Rated, Prizes!)) — E. Tenzing and Triangle
// Submission: https://codeforces.com/contest/1842/submission/334343383

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 200005;
int n, k, A;
i64 inf = 1e18;

struct Point {
  int x, y, c;
};

Point pts[k_max_n];
vector<int> buckets[k_max_n];
i64 dp[k_max_n];

struct Node {
  i64 value;
  i64 carry;
};

Node tx[5 * k_max_n];

void flag(int i) {
  tx[2 * i].value += tx[i].carry;
  tx[2 * i + 1].value += tx[i].carry;
  tx[2 * i].carry += tx[i].carry;
  tx[2 * i + 1].carry += tx[i].carry;
  tx[i].carry = 0;
}

void update(int i, int low, int high, int u, int v, int delta) {
  if (v < low || high < u) {
    return;
  }
  if (u <= low && high <= v) {
    tx[i].value += delta;
    tx[i].carry += delta;
    return;
  }
  flag(i);
  int mid = (low + high) / 2;
  update(2 * i, low, mid, u, v, delta);
  update(2 * i + 1, mid + 1, high, u, v, delta);
  tx[i].value = max(tx[2 * i].value, tx[2 * i + 1].value);
}

i64 get(int i, int low, int high, int u, int v) {
  if (v < low || high < u) {
    return -inf;
  }
  if (u <= low && high <= v) {
    return tx[i].value;
  }
  flag(i);
  int mid = (low + high) / 2;
  return max(get(2 * i, low, mid, u, v), get(2 * i + 1, mid + 1, high, u, v));
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 total_sum = 0;

  cin >> n >> k >> A;
  for (int i = 0; i < n; i++) {
    cin >> pts[i].x >> pts[i].y >> pts[i].c;
    buckets[k - pts[i].y].push_back(i);
    total_sum += pts[i].c;
  }

  for (int i = 1; i <= k; i++) {
    update(1, 0, k, 0, i - 1, -A);
    for (auto idx : buckets[i]) {
      update(1, 0, k, 0, pts[idx].x, pts[idx].c);
    }
    i64 v = get(1, 0, k, 0, k);
    dp[i] = max(dp[i - 1], v);

    // set the value of i to v
    i64 tmp = get(1, 0, k, i, i);
    update(1, 0, k, i, i, dp[i] - tmp);
  }

  cout << total_sum - dp[k] << endl;

  return 0;
}
