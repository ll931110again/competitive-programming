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
#include <vector>
#define maxn 1000005
using namespace std;

int n, m;
int a[maxn], b[maxn];
int posB[maxn];

bool good(int i, int j, int k) {
  if (k < 0) {
    return false;
  }
  if (i <= j) {
    return (k > j || k < i);
  }
  return (k > j && k < i);
}

int length(int i, int j) {
  if (i <= j) {
    return j - i + 1;
  }
  return (j - i + 1 + n);
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  memset(posB, -1, sizeof posB);
  
  cin >> n >> m;
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  for (int i = 0; i < m; i++) {
    cin >> b[i];
    posB[b[i]] = i;
  }

  int ans = 0;
  int R = -1;
  for (int L = 0; L < n; L++) {
    if (R < 0 && posB[a[L]] >= 0) {
      R = L;
    }
    if (R >= 0) {
      while ((R + 1) % n != L && good(posB[a[L]], posB[a[R]], posB[a[(R + 1) % n]])) {
        R = (R + 1) % n;
      }
      ans = max(ans, length(L, R));
      if (L == R) {
        R = -1;
      }
    }
  }

  cout << ans << endl;
  return 0;
}
