#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<pair<int, int>> a(n);
    int maxV = 2 * n;
    vector<int> freq(maxV + 2, 0);
    for (int i = 0; i < n; ++i) {
      int l, r;
      cin >> l >> r;
      a[i] = {l, r};
      if (l == r) {
        freq[l]++;
      }
    }
    vector<int> zeroPref(maxV + 2, 0);
    for (int v = 1; v <= maxV; ++v) {
      zeroPref[v] = zeroPref[v - 1] + (freq[v] == 0 ? 1 : 0);
    }
    string ans;
    for (int i = 0; i < n; ++i) {
      int l = a[i].first;
      int r = a[i].second;
      if (l < r) {
        int zeros = zeroPref[r] - zeroPref[l - 1];
        ans += (zeros > 0 ? '1' : '0');
      } else {
        int v = l;
        ans += (freq[v] == 1 ? '1' : '0');
      }
    }
    cout << ans << '\n';
  }
  return 0;
}