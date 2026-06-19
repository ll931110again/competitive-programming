
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr unsigned MOD = 1'000'000'007;
#include "../../../lib/modint.h"
using Mint = ModInt<MOD>;

Mint binom(i64 x, i64 y) {
  if (x < y) {
    return 0;
  }
  Mint ans = 1;
  for (int i = 1; i <= y; i++) {
    ans *= (x - i + 1);
    ans /= i;
  }
  return ans;
}

int T;
i64 n, a, b;

vector<pair<i64, int>> mp;
vector<int> divisors;

Mint ans;

vector<int> complement_divisors(vector<int> x) {
  vector<int> y = divisors;
  for (int i = 0; i < divisors.size(); i++) {
    y[i] = mp[i].second - x[i];
  }
  return y;
}

Mint steps(i64 n, vector<int> divisors) {
  Mint value = 1;
  for (auto x : divisors) {
    value *= binom(n + x - 1, x);
  }
  return value;
}

void rec(int idx, i64 prod) {
  if (prod > a) {
    return;
  }

  if (idx >= divisors.size()) {
    ans += steps(n, divisors) * steps(n, complement_divisors(divisors));
    return;
  }

  for (int i = 0; i <= mp[idx].second; i++) {
    divisors[idx] = i;
    rec(idx + 1, prod);
    prod *= mp[idx].first;
  }
}

Mint solve(i64 n, i64 a, i64 b) {
  mp.clear();
  i64 tmp = b;

  for (i64 i = 2; i * i <= b; i++)
    if (tmp % i == 0) {
      int cnt = 0;
      while (tmp % i == 0) {
        cnt++;
        tmp /= i;
      }
      mp.push_back({i, cnt});
    }

  if (tmp > 1) {
    mp.push_back({tmp, 1});
  }

  ans = 0;
  divisors.resize(mp.size());
  rec(0, 1);
  return ans;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  for (int it = 1; it <= T; it++) {
    cerr << it << endl;
    cin >> n >> a >> b;
    cout << "Case #" << it << ": ";
    cout << solve(n, a, b) << endl;
  }

  return 0;
}
