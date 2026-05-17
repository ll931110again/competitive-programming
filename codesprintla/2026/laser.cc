/*
    B — Guardian Laser (user AC)

    By the standard mirroring trick, we can see that the laser hits point A
    if it hits a point (2m - 1, 2n) for some integers m and n.

    This evaluates to a(2m - 1) - b(2n) = 0, or 2ma - 2nb = a.
    This will happen if and only if a is divisible by 2 * gcd(a, b).

    Similarly, the laser hits point B if b is divisible by 2 * gcd(a, b).

    So we can simply check if a or b is not divisible by 2 * gcd(a, b).
    Note that both of them cannot happen, since otherwise
    2 * gcd(a, b) > gcd(a, b) is divisible by both a and b, which is impossible.
 */

#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif
#include <iostream>
using namespace std;

int t, a, b;

int gcd(int x, int y) {
  if (x == 0) {
    return y;
  }
  if (x > y) {
    swap(x, y);
  }
  return gcd(y % x, x);
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> t;
  while (t--) {
    cin >> a >> b;
    if (a % (2 * gcd(a, b))) {
      cout << "A" << endl;
    } else {
      cout << "B" << endl;
    }
  }

  return 0;
}