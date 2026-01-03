/*
 * Solution for problem A: ASCII Art.
 *
 */

#ifdef ONLINE_JUDGE
  #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <unordered_set>
#include <vector>
#define maxn 105
using namespace std;

int m, n;
string s[maxn];

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("ascii.in", "r", stdin);
  freopen("ascii.out", "w", stdout);

  cin >> m >> n;
  for (int i = 0; i < m; i++) {
    cin >> s[i];
  }

  int slashes = 0, dots = 0, parity = 0;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (s[i][j] == '/' || s[i][j] == '\\') {
        slashes++;
        parity ^= 1; 
      } else if (parity) {
        dots++;
      }
    }
  }

  cout << slashes / 2 + dots << endl;
  return 0;
}
