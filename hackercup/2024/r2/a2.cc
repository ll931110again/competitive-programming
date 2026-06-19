#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 1000005;
int T, n;
vector<i64> nums;

i64 solve() {
  i64 A, B, M;
  cin >> A >> B >> M;

  i64 ret = 0;
  for (auto num : nums) {
    if (num >= A && num <= B && num % M == 0) {
      ret++;
    }
  }

  return ret;
}

void rec(int pos, int d, i64 val, int last_digit) {
  if (pos >= d) {
    nums.push_back(val);
    return;
  }

  for (int next_digit = 1; next_digit < 10; next_digit++) {
    if ((pos == d / 2 || pos == d / 2 + 1) && (next_digit == last_digit)) {
      continue;
    }

    if (pos <= d / 2 && next_digit < last_digit) {
      continue;
    }

    if (pos > d / 2 && next_digit > last_digit) {
      continue;
    }

    rec(pos + 1, d, val * 10 + next_digit, next_digit);
  }
}

void generate_candidates() {
  for (int d = 1; d < 19; d += 2) {
    rec(0, d, 0, -1);
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  generate_candidates();

  cin >> T;
  for (int it = 1; it <= T; it++) {
    cout << "Case #" << it << ": " << solve() << '\n';
  }
  return 0;
}
