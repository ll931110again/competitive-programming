/*
 * Solution for problem D: Dome of Circus.
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
#define maxn 10005
using namespace std;

struct Point {
  double x, y, z;
};

int n;
Point a[maxn];

double scaled_volume(double h, double r) {
  // the true volume of a cone is 1/3 pi h r^2, but we can simply scale out the constant factors
  return h * r * r;
}

double eval(double h) {
  double r = 0;
  for (int i = 0; i < n; i++) {
    if (a[i].z >= h) {
      return -1;
    }

    // connecting from (0, 0, h) to (x, y, z)
    double dh = h - a[i].z;
    double dx = sqrt(a[i].x * a[i].x + a[i].y * a[i].y);
    r = max(r, dx / dh * h);
  }

  return r;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("dome.in", "r", stdin);
  freopen("dome.out", "w", stdout);

  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> a[i].x >> a[i].y >> a[i].z;
  }

  double low_h = 0, high_h = 1e5;
  for (int iter = 0; iter < 200; iter++) {
    double m1 = (low_h + low_h + high_h) / 3, m2 = (low_h + high_h + high_h) / 3;
    double r1 = eval(m1), r2 = eval(m2);

    if (r1 < 0 || scaled_volume(m1, r1) > scaled_volume(m2, r2)) {
      low_h = m1;
    } else {
      high_h = m2;
    }
  }

  double r = eval(low_h);
  cout << std::fixed;
  cout << setprecision(12);
  cout << low_h << ' ' << r << endl;

  return 0;
}
