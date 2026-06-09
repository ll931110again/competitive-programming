#include <bits/stdc++.h>
using namespace std;

namespace {

string s;

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> s;
  cout << ((s == "00:00") ? "okay" : "poor") << endl;
  return 0;
}
