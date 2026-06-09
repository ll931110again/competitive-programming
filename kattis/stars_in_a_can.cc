// Kattis starsinacan / BOJ 14507 — Stars in a Can
// https://open.kattis.com/problems/starsinacan
//
// For each convex-hull face (three stars on the base plane), compute the
// minimum enclosing circle of projected stars and the max height above/below
// the plane.  Volume = pi * r^2 * h.

#include <bits/stdc++.h>
using namespace std;

using ld = long double;

namespace {

struct P3 {
  ld x, y, z;
};

struct Face {
  int a, b, c;
};

ld dot(const P3& a, const P3& b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

P3 cross(const P3& a, const P3& b) {
  return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

ld norm(const P3& a) {
  return sqrtl(dot(a, a));
}

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
        if (dot(nrm, nrm) < eps) {
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

P3 proj_to_plane(const P3& p, const P3& a, const P3& n_unit) {
  ld t = dot({p.x - a.x, p.y - a.y, p.z - a.z}, n_unit);
  return {p.x - n_unit.x * t, p.y - n_unit.y * t, p.z - n_unit.z * t};
}

ld mec2d(vector<pair<ld, ld>> p) {
  mt19937 rng(42);
  shuffle(p.begin(), p.end(), rng);
  ld cx = p[0].first, cy = p[0].second, r = 0;
  const ld eps = 1 + 1e-12L;
  for (size_t i = 0; i < p.size(); ++i) {
    ld dx = cx - p[i].first, dy = cy - p[i].second;
    if (dx * dx + dy * dy <= r * r * eps) {
      continue;
    }
    cx = p[i].first;
    cy = p[i].second;
    r = 0;
    for (size_t j = 0; j < i; ++j) {
      dx = cx - p[j].first;
      dy = cy - p[j].second;
      if (dx * dx + dy * dy <= r * r * eps) {
        continue;
      }
      cx = (p[i].first + p[j].first) / 2;
      cy = (p[i].second + p[j].second) / 2;
      r = hypotl(p[i].first - cx, p[i].second - cy);
      for (size_t k = 0; k < j; ++k) {
        dx = cx - p[k].first;
        dy = cy - p[k].second;
        if (dx * dx + dy * dy <= r * r * eps) {
          continue;
        }
        ld x1 = p[i].first, y1 = p[i].second;
        ld x2 = p[j].first, y2 = p[j].second;
        ld x3 = p[k].first, y3 = p[k].second;
        ld d = 2 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
        cx = ((x1 * x1 + y1 * y1) * (y2 - y3) + (x2 * x2 + y2 * y2) * (y3 - y1) +
              (x3 * x3 + y3 * y3) * (y1 - y2)) /
             d;
        cy = ((x1 * x1 + y1 * y1) * (x3 - x2) + (x2 * x2 + y2 * y2) * (x1 - x3) +
              (x3 * x3 + y3 * y3) * (x2 - x1)) /
             d;
        r = hypotl(x1 - cx, y1 - cy);
      }
    }
  }
  return r;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<P3> pts(n);
  for (int i = 0; i < n; ++i) {
    cin >> pts[i].x >> pts[i].y >> pts[i].z;
  }

  const ld pi = acosl(-1.0L);
  ld ans = 1e300L;

  for (const Face& f : hull_faces(pts)) {
    P3 a = pts[f.a], b = pts[f.b], c = pts[f.c];
    P3 ab{b.x - a.x, b.y - a.y, b.z - a.z};
    P3 ac{c.x - a.x, c.y - a.y, c.z - a.z};
    P3 nrm = cross(ab, ac);
    ld nn = norm(nrm);
    nrm = {nrm.x / nn, nrm.y / nn, nrm.z / nn};

    P3 bx = ab;
    ld bl = norm(bx);
    bx = {bx.x / bl, bx.y / bl, bx.z / bl};
    P3 by = cross(nrm, bx);

    ld height = 0;
    vector<pair<ld, ld>> flat;
    flat.reserve(n);
    for (const P3& p : pts) {
      P3 q = proj_to_plane(p, a, nrm);
      height = max(height, norm({p.x - q.x, p.y - q.y, p.z - q.z}));
      flat.push_back(
          {dot({q.x - a.x, q.y - a.y, q.z - a.z}, bx), dot({q.x - a.x, q.y - a.y, q.z - a.z}, by)});
    }
    ld r = mec2d(flat);
    ans = min(ans, pi * r * r * height);
  }

  cout << setprecision(10) << ans << '\n';
  return 0;
}
