// Codeforces 1750 (CodeTON Round 3 (Div. 1 + Div. 2, Rated, Prizes!)) — E. Bracket Cost
// Submission: https://codeforces.com/contest/1750/submission/334622245

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 200005;
int T, n;
string s;
int delta[k_max_n];

struct Node {
  int cnt;
  i64 sum;
};

Node tx[8 * k_max_n];

void init(int i, int low, int high) {
  tx[i] = {.cnt = 0, .sum = 0};
  if (low >= high) {
    return;
  }

  int mid = (low + high) / 2;
  init(2 * i, low, mid);
  init(2 * i + 1, mid + 1, high);
}

void add(int i, int low, int high, int pos) {
  if (low == high) {
    tx[i].cnt++;
    tx[i].sum += (pos - n);
    return;
  }

  int mid = (low + high) / 2;
  if (pos <= mid) {
    add(2 * i, low, mid, pos);
  } else {
    add(2 * i + 1, mid + 1, high, pos);
  }

  tx[i] = {.cnt = tx[2 * i].cnt + tx[2 * i + 1].cnt, .sum = tx[2 * i].sum + tx[2 * i + 1].sum};
}

Node get(int i, int low, int high, int upper) {
  if (high < upper) {
    return {.cnt = 0, .sum = 0};
  }
  if (upper <= low) {
    return tx[i];
  }

  int mid = (low + high) / 2;
  Node t1 = get(2 * i, low, mid, upper);
  Node t2 = get(2 * i + 1, mid + 1, high, upper);
  return {.cnt = t1.cnt + t2.cnt, .sum = t1.sum + t2.sum};
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    cin >> s;
    delta[0] = 0;
    for (int i = 0; i < n; i++) {
      delta[i + 1] = delta[i];
      if (s[i] == '(') {
        delta[i + 1]++;
      } else {
        delta[i + 1]--;
      }
    }
    init(1, 0, 2 * n);

    i64 ans = 0;
    add(1, 0, 2 * n, n);

    for (int i = 1; i <= n; i++) {
      auto value = get(1, 0, 2 * n, delta[i] + n);
      ans += value.sum + 1LL * (i - value.cnt) * delta[i];
      add(1, 0, 2 * n, delta[i] + n);
    }

    stack<vector<i64>> ps;
    i64 ts = 0;
    ps.push({0, 1, 0});

    for (int i = 1; i <= n; i++) {
      vector<i64> accum = {delta[i], 0, 0};
      while (!ps.empty() && ps.top()[0] >= delta[i]) {
        accum[1] += ps.top()[1];
        accum[2] += 1LL * delta[i] * ps.top()[1];
        ts -= ps.top()[2];
        ps.pop();
      }

      ans -= ts;
      ans -= accum[2];
      accum[1]++;
      accum[2] += delta[i];
      ts += accum[2];
      ps.push(accum);
    }

    cout << ans << endl;
  }
  return 0;
}
