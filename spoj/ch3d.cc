// SPOJ CH3D — Convex Hull 3D
// https://www.spoj.com/problems/CH3D/
//
// Count triangular faces on the 3D convex hull (each face once, CCW outward).

#include <algorithm>
#include <cmath>
#include <iostream>
#include <vector>

using ld = long double;

struct P3 {
  ld x, y, z;
};

static ld dot(const P3& a, const P3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

static P3 cross(const P3& a, const P3& b) {
  return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

static ld vol(const P3& a, const P3& b, const P3& c, const P3& d) {
  P3 ab{b.x - a.x, b.y - a.y, b.z - a.z};
  P3 ac{c.x - a.x, c.y - a.y, c.z - a.z};
  P3 ad{d.x - a.x, d.y - a.y, d.z - a.z};
  return dot(cross(ab, ac), ad);
}

struct Face {
  int a, b, c;
};

static std::vector<Face> hullFaces(const std::vector<P3>& p) {
  int n = (int)p.size();
  std::vector<Face> faces;
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

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  int t;
  std::cin >> t;
  while (t--) {
    int n;
    std::cin >> n;
    std::vector<P3> pts(n);
    for (int i = 0; i < n; ++i) {
      std::cin >> pts[i].x >> pts[i].y >> pts[i].z;
    }
    std::cout << hullFaces(pts).size() << '\n';
  }
  return 0;
}
