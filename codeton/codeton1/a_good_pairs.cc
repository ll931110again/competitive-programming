// Codeforces 1656 (CodeTON Round 1 (Div. 1 + Div. 2, Rated, Prizes!)) — A. Good Pairs
// Submission: https://codeforces.com/contest/1656/submission/150726089

#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <cstdio>
#include <queue>
#include <stack>
#include <vector>
#define maxn 100005
using namespace std;

int T, n;
int a[maxn];

int main() {
  scanf("%d", &T);
  while (T--) {
    scanf("%d", &n);
    for (int i = 1; i <= n; i++) {
      scanf("%d", &a[i]);
    }

    int min_idx = 1, max_idx = 1;
    for (int i = 2; i <= n; i++) {
      if (a[min_idx] > a[i]) {
        min_idx = i;
      }
      if (a[max_idx] < a[i]) {
        max_idx = i;
      }
    }

    printf("%d %d\n", min_idx, max_idx);
  }
  return 0;
}
