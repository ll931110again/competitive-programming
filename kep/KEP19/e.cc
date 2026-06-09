#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
int n, m;
int color[k_max_n], layer[k_max_n];

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> n >> m;
  for (int i = 0; i < n; i++) {
    cin >> color[i];
  }
  for (int i = 0; i < n; i++) {
    cin >> layer[i];
  }

  int total_cnt = 0, max_decrement = 0;
  int i = 0;
  while (i < n) {
    int j = i + 1;
    while (j < n && color[j] == color[i]) {
      j++;
    }

    for (int k = i; k + 1 < j; k++) {
      if (layer[k] <= layer[k + 1]) {
        total_cnt++;
      }
    }

    int decrement = 0;
    for (int k = i; k + 1 < j; k++) {
      if (m > 1 && layer[k] <= layer[k + 1]) {
        if (layer[k] != 1 || layer[k + 1] != m) {
          decrement = 1;
        }
      }
    }

    max_decrement = max(max_decrement, decrement);
    i = j;
  }

  cout << total_cnt - max_decrement << endl;

  return 0;
}
