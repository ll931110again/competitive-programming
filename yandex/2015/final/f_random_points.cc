#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 200005;
int n;
i64 a[k_max_n];
int delta = 200;

const i64 GENERATOR_A = 134775813;
const i64 GENERATOR_C = 1;

const i64 X = 100000000;
const i64 DENOMINATOR = 1LL << 32;

bool can_generate(i64 candidate) {
  i64 state = candidate;
  if (state * X / DENOMINATOR != a[0]) {
    return false;
  }
  for (int i = 1; i < n; i++) {
    state = (state * GENERATOR_A + GENERATOR_C) % DENOMINATOR;
    i64 ax = (state * X / DENOMINATOR);
    if (ax != a[i]) {
      return false;
    }
  }
  return true;
}

bool solve() {
  i64 s = a[0] * DENOMINATOR / X;
  for (i64 candidate = s - delta; candidate <= s + delta; candidate++)
    if (candidate >= 0 && candidate < DENOMINATOR) {
      if (can_generate(candidate)) {
        return true;
      }
    }
  return false;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> n;
  n *= 2;
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }

  cout << (solve() ? "RAW" : "SHUFFLED") << endl;
  return 0;
}
