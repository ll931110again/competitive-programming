#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 200005
#define maxk 20
using namespace std;

int T, n, k;
int a[maxn], good[maxn], last_good[maxn];
int gcds[maxn][maxk];

int gcd(int x, int y) {
  if (x > y) {
    swap(x, y);
  }

  if (x == 0) {
    return y;
  }

  return gcd(y % x, x);
}

int gcd_range(int i, int j) {
  int ans = a[i];
  for (int k = maxk - 1; k >= 0; k--) {
    if ((i + (1 << k) - 1) <= j) {
      ans = gcd(ans, gcds[i][k]);
      i += (1 << k);
    }
  }
  return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
      cin >> n;
      for (int i = 0; i < n; i++) {
        cin >> a[i];
      }

      for (int i = 0; i < n; i++) {
        good[i] = true;
        int x = a[i];
        for (int p = 2; p * p <= x && x > 1; p++) if (x % p == 0) {
          int cnt = 0;
          while (x % p == 0) {
            x /= p;
            cnt++;
          }
          if (cnt == 1) {
            good[i] = false;
          }
        }
        if (x > 1) {
          good[i] = false;
        }
      }

      for (int i = n - 1; i >= 0; i--) {
        last_good[i] = i;
        if (i + 1 < n && good[i + 1]) {
          last_good[i] = last_good[i + 1];
        }
      }

      for (int i = 0; i < n; i++) {
        gcds[i][0] = a[i];
      }

      for (int j = 1; j < maxk; j++) {
        for (int i = 0; i < n; i++) {
          gcds[i][j] = gcds[i][j - 1];
          int _i = i + (1 << (j - 1));
          if (_i < n) {
            gcds[i][j] = gcd(gcds[i][j - 1], gcds[_i][j - 1]);
          }
        }
      }

      long long ans = 0;
      for (int i = 0; i < n; i++) if (good[i]) {
        int j = i, value = a[i];
        while (j <= last_good[i]) {
          int low = j, high = last_good[i], pos = low;
          while (low <= high) {
            int mid = (low + high) / 2;
            if (gcd_range(i, mid) == value) {
              pos = mid;
              low = mid + 1;
            } else {
              high = mid - 1;
            }
          }
          ans += 1LL * value * (pos - j + 1);
          value = gcd_range(i, pos + 1);
          j = pos + 1;
        }
      }

      cout << ans << endl;
    }
    
    return 0;
}