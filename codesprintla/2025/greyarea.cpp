// H – Grey Area
// https://open.kattis.com/problems/greyarea

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  cout << fixed << setprecision(15);
  double K, S;
  cin >> K >> S;
  int Q;
  cin >> Q;
  double t = K / S;
  while (Q--) {
    double d, vx, vy, vz;
    cin >> d >> vx >> vy >> vz;
    double rx = vx - S, ry = vy, rz = vz;
    double sum = 0;
    // Face +x (normal +x): influx when relative x velocity is toward -x
    sum += d * t * max(0.0, -rx);
    // Face -x
    sum += d * t * max(0.0, rx);
    // Face +y (up): no flux from below ground rule uses vy — particles toward +y hit bottom?
    // Problem: negative y face is ground (no exposure). Positive y face (up): wind from above.
    sum += d * t * max(0.0, -ry);
    // Face -y (ground): blocked
    // sum += 0;
    // +z / -z
    sum += d * t * max(0.0, -rz);
    sum += d * t * max(0.0, rz);
    cout << sum << '\n';
  }
  return 0;
}
