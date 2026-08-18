#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  while (T--) {
    long long a, b, c;
    cin >> a >> b >> c;
    vector<long long> v = {a, b, c};
    sort(v.begin(), v.end());
    // v[0] <= v[1] <= v[2]
    if (v[2] > v[0] + v[1]) {
      cout << v[1] << "\n";
    } else {
      cout << v[2] - v[0] << "\n";
    }
  }
  return 0;
}