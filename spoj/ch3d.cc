// SPOJ CH3D — Convex Hull 3D
// https://www.spoj.com/problems/CH3D/
//
// Count triangular faces on the 3D convex hull (each face once, CCW outward).

#include <bits/stdc++.h>
using namespace std;

using ld = long double;

namespace {

struct P3 {
  ld x, y, z;
};

ld dot(const P3& a, const P3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

P3 cross(const P3& a, const P3& b) {
  return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

ld vol(const P3& a, const P3& b, const P3& c, const P3& d) {
  P3 ab{b.x - a.x, b.y - a.y, b.z - a.z};
  P3 ac{c.x - a.x, c.y - a.y, c.z - a.z};
  P3 ad{d.x - a.x, d.y - a.y, d.z - a.z};
  return dot(cross(ab, ac), ad);
}

struct Face {
  int a, b, c;
};

vector<Face> hull_faces(const vector<P3>& p) {
  int n = (int)p.size();
  vector<Face> faces;
  const ld eps = 1e-9L;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      for (int k = j + 1; k < n; ++k) {
        P3 a = p[i], b = p[j], c = p[k];
        P3 ab{b.x - a.x, b.y - a.y, b.z - a.z};
        P3 ac{c.x - a.x, c.y - a.y, c.z - a.z};
        P3 nrm = cross(ab, ac);
        ld nn = dot(nrm, nrm);
        if (nn < eps) {
          continue;
        }
        int pos = 0, neg = 0;
        for (int t = 0; t < n; ++t) {
          if (t == i || t == j || t == k) {
            continue;
          }
          ld s = dot(nrm, {p[t].x - a.x, p[t].y - a.y, p[t].z - a.z});
          if (s > eps) {
            ++pos;
          } else if (s < -eps) {
            ++neg;
          }
        }
        if (pos && neg) {
          continue;
        }
        if (neg) {
          faces.push_back({i, k, j});
        } else {
          faces.push_back({i, j, k});
        }
      }
    }
  }
  return faces;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<P3> pts(n);
    for (int i = 0; i < n; ++i) {
      cin >> pts[i].x >> pts[i].y >> pts[i].z;
    }
    cout << hull_faces(pts).size() << '\n';
  }
  return 0;
}
