// Codeforces 1896 (CodeTON Round 7 (Div. 1 + Div. 2, Rated, Prizes!)) — C. Matching Arrays
// Submission: https://codeforces.com/contest/1896/submission/336567371

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int T, n, k;
int a[k_max_n], b[k_max_n];

struct Element {
  int value;
  int pos;
};

Element sorted_a[k_max_n], sorted_b[k_max_n], arranged_b[k_max_n];
int ans[k_max_n];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n >> k;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
      sorted_a[i] = {.value = a[i], .pos = i};
    }
    sort(sorted_a, sorted_a + n, [&](auto x, auto y) { return x.value > y.value; });

    for (int i = 0; i < n; i++) {
      cin >> b[i];
      sorted_b[i] = {.value = b[i], .pos = i};
    }
    sort(sorted_b, sorted_b + n, [&](auto x, auto y) { return x.value > y.value; });

    for (int i = 0; i < n; i++) {
      if (i < k) {
        arranged_b[i] = sorted_b[i + n - k];
      } else {
        arranged_b[i] = sorted_b[i - k];
      }
    }

    int score = 0;
    for (int i = 0; i < n; i++)
      if (sorted_a[i].value > arranged_b[i].value) {
        score++;
      }

    if (score != k) {
      cout << "No" << endl;
      continue;
    }

    cout << "Yes" << endl;
    for (int i = 0; i < n; i++) {
      ans[sorted_a[i].pos] = arranged_b[i].pos;
    }

    for (int i = 0; i < n; i++) {
      cout << b[ans[i]] << ' ';
    }
    cout << endl;
  }

  return 0;
}
