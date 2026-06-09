#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 1000005;
int n, a[k_max_n], freq[k_max_n];
int dp[k_max_n];

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> a[i];
    freq[a[i]]++;
  }

  int ans = 0;
  for (int i = 1; i < k_max_n; i++)
    if (freq[i]) {
      dp[i] = max(dp[i], freq[i]);
      ans = max(ans, dp[i]);

      for (int j = i + i; j < k_max_n; j += i)
        if (freq[j]) {
          dp[j] = max(dp[j], dp[i] + freq[j]);
        }
    }

  cout << ans << endl;
  return 0;
}
