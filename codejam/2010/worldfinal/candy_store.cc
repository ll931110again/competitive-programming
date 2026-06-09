#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    long long k, C;
    cin >> k >> C;

    __int128 target = (__int128)k * (__int128)C;
    __int128 sum = 0;
    long long boxes = 0;

    while (sum < target) {
      __int128 next = sum / k + 1; // floor(sum/k) + 1
      sum += next;
      boxes++;
    }

    cout << "Case #" << tc << ": " << boxes << "\n";
  }
  return 0;
}
