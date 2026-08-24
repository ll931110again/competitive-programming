#ifdef ONLINE_JUDGE
  #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>
using namespace std;

namespace {
    string s;
} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  vector<char> target = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j'};
  cin >> s;
  int ways = 0;
  
  do {
    string tmp = s;
    for (int i = 0; i < tmp.size(); i++) {
        tmp[i] = target[tmp[i] - '0'];
    }
    int cursor = 'a';
    for (auto c : tmp) if (c == cursor) {
        cursor++;
    }
    if (cursor > 'j') {
        ways++;
    }
  } while (next_permutation(target.begin(), target.end()));

  cout << ways << endl;

  return 0;
}
