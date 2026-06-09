#include <bits/stdc++.h>
using namespace std;

namespace {

string suffix = "hogwarts.com";
string inp;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> inp;
  int idx = -1;
  for (int i = 0; i < inp.size(); i++)
    if (inp[i] == '@') {
      idx = i;
      break;
    }

  if (idx != 5) {
    cout << "No\n";
    return 0;
  }

  if (inp.substr(6) != suffix) {
    cout << "No\n";
    return 0;
  }
  for (int i = 0; i < 5; i++) {
    if (inp[i] < 'a' || 'z' < inp[i]) {
      cout << "No\n";
      return 0;
    }
  }
  cout << "Yes\n";
  return 0;
}
