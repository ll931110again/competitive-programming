
#include <bits/stdc++.h>
using namespace std;

namespace {

int T, n;
string s;

string solve(string s) {
  string corpus = "rgby";

  string ans = "";
  for (int i = 0; i < n - 2; i++) {
    set<char> available;
    for (int j = i; j < i + 3; j++) {
      available.insert(s[j]);
    }

    for (auto c : corpus) {
      if (available.find(c) == available.end()) {
        ans += c;
        break;
      }
    }
  }

  ans += "gg";
  return ans;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    cin >> s;
    cout << solve(s) << endl;
  }
}
