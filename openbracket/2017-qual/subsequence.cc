#include <bits/stdc++.h>
using namespace std;

namespace {

string inp;
int K, pos = 0;
string ret;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> inp >> K;
  for (int ch = 'z'; ch >= 'a'; ch--) {
    int ncount = 0, last_pos = -1;
    for (int i = pos; i < inp.size(); i++)
      if (inp[i] == ch) {
        ncount++;
        last_pos = i;
      }
    if (ncount >= K) {
      for (int i = 0; i < ncount; i++) {
        ret += ch;
      }
      pos = last_pos + 1;
    }
  }
  cout << ret << endl;
  return 0;
}
