// Codeforces 1896 (CodeTON Round 7 (Div. 1 + Div. 2, Rated, Prizes!)) — A. Jagged Swaps
// Submission: https://codeforces.com/contest/1896/submission/336566355

#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 20
using namespace std;

int T, n;
int a[maxn];

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 1; i <= n; i++) {
      cin >> a[i];
    }

    if (a[1] == 1) {
      cout << "Yes" << endl;
    } else {
      cout << "No" << endl;
    }
  }

  return 0;
}
