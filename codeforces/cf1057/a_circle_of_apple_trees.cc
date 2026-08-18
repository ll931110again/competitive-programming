#include <iostream>
#include <set>
using namespace std;

int main() {
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    set<int> beauties;
    for (int i = 0; i < n; i++) {
      int x;
      cin >> x;
      beauties.insert(x);
    }
    cout << beauties.size() << endl;
  }
  return 0;
}