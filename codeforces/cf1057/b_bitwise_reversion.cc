#include <iostream>
using namespace std;

int main() {
  int t;
  cin >> t;
  while (t--) {
    long long x, y, z;
    cin >> x >> y >> z;
    if ((x & y) == (y & z) && (y & z) == (x & z)) {
      cout << "YES\n";
    } else {
      cout << "NO\n";
    }
  }
  return 0;
}