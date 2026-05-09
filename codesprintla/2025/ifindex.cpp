// J – If Index
// https://open.kattis.com/problems/ifindex

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  string S;
  if (!(cin >> S))
    return 0;
  vector<int> depths;
  int bal = 0;
  for (size_t i = 0; i < S.size();) {
    if (i + 1 < S.size() && S[i] == 'i' && S[i + 1] == 'f') {
      depths.push_back(bal);
      i += 2;
    } else if (S[i] == '{') {
      ++bal;
      ++i;
    } else if (S[i] == '}') {
      --bal;
      ++i;
    } else {
      ++i;
    }
  }
  if (depths.empty()) {
    cout << -1 << '\n';
    return 0;
  }
  sort(depths.begin(), depths.end());
  int idx = (int)(depths.size() + 1) / 2;
  cout << depths[idx - 1] << '\n';
  return 0;
}
