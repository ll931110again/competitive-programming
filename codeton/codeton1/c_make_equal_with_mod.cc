// Codeforces 1656 (CodeTON Round 1 (Div. 1 + Div. 2, Rated, Prizes!)) — C. Make Equal With Mod
// Submission: https://codeforces.com/contest/1656/submission/150742937

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <cstdio>
#include <queue>
#include <stack>
#include <vector>
#define maxn 200005
using namespace std;

int T, n, a[maxn];

bool solve() {
  sort(a, a + n);
  bool found1 = false;
  for (int i = 0; i < n; i++)
    if (a[i] == 1) {
      found1 = true;
      break;
    }

  if (!found1) {
    return true;
  }

  // Bad if found any consecutive pair
  for (int i = 0; i + 1 < n; i++)
    if (a[i + 1] - a[i] == 1) {
      return false;
    }

  return true;
}

int main() {
  scanf("%d", &T);
  while (T--) {
    scanf("%d", &n);
    for (int i = 0; i < n; i++) {
      scanf("%d", &a[i]);
    }
    if (solve()) {
      printf("YES\n");
    } else {
      printf("NO\n");
    }
  }
  return 0;
}
