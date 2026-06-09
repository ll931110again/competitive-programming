#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 100005;
long long ss[k_max_n];

int besthub(int R, int L, int X[], long long B) {
  ss[0] = X[0];
  for (int i = 1; i < R; i++) {
    ss[i] = ss[i - 1] + X[i];
  }

  auto ss_sum = [&](int i, int j) { return ss[j] - (i > 0 ? ss[i - 1] : 0); };

  auto compute_cost = [&](int i, int j) {
    int k = (i + j) / 2;
    long long dist_left = 1LL * (k - i + 1) * X[k] - ss_sum(i, k);
    long long dist_right = ss_sum(k, j) - 1LL * (j - k + 1) * X[k];
    return dist_left + dist_right;
  };

  int i = 0, j = 0, max_count = 1;
  while (i < R) {
    if (j < i) {
      j = i;
    }
    while (j < R && compute_cost(i, j) <= B) {
      j++;
    }
    max_count = max(max_count, j - i);
    i++;
  }

  return max_count;
}

} // namespace
