#include <iostream>
#include <utility>
using namespace std;

pair<long long, long long> solve(long long len, long long k) {
  if (len < k) {
    return {0, 0};
  }
  if (len % 2 == 0) {
    auto [A1, B1] = solve(len / 2, k);
    long long A = 2 * A1;
    long long B = A1 * (len / 2) + 2 * B1;
    return {A, B};
  } else {
    auto [A1, B1] = solve((len - 1) / 2, k);
    long long A = 1 + 2 * A1;
    long long B = (len - 1) / 2 + A1 * ((len + 1) / 2) + 2 * B1;
    return {A, B};
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    long long n, k;
    cin >> n >> k;
    auto [A, B] = solve(n, k);
    cout << A + B << '\n';
  }
  return 0;
}