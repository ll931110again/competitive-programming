/*
 * Solution for problem B: Binary Encoding.
 *
 * To construct such a code, we can insert all the codes in a prefix tries. Every step,
 * we take the shortest code word and expand it by appending 0 and 1 in the end.
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
#define maxn 10005
using namespace std;

int n;
string s[maxn];

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("binary.in", "r", stdin);
  freopen("binary.out", "w", stdout);

  cin >> n;
  s[0] = "0";
  s[1] = "1";

  int cursor = 1;
  for (int i = 2; i < n; i++) {
    for (int j = i; j > cursor; j--) {
      s[j] = s[j - 1];
    }
    s[cursor + 1] = s[cursor];
    s[cursor] += '0';
    s[cursor + 1] += '1';

    cursor--;
    if (cursor < 0) {
      cursor = i;
    }
  }

  for (int i = 0; i < n; i++) {
    cout << s[i] << endl;
  }

  return 0;
}
