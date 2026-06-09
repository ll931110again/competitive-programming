#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 55;
int n, m;
int scores[k_max_n];
int test_cnt[k_max_n];
bool ok[k_max_n];

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> scores[i];
  }

  for (int i = 0; i < n; i++) {
    cin >> test_cnt[i];
  }

  cin >> m;
  int total_score = 0;

  for (int attempt = 0; attempt < m; attempt++) {
    for (int i = 0; i < n; i++) {
      bool ans = true;
      for (int j = 0; j < test_cnt[i]; j++) {
        int x;
        cin >> x;
        ans &= x;
      }
      if (ans && !ok[i]) {
        ok[i] = true;
        total_score += scores[i];
      }
    }
  }

  cout << total_score << endl;
  return 0;
}
