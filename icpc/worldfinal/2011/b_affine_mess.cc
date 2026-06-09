/*
 * ICPC World Finals 2011 — Problem B: Affine Mess
 *
 * Faithful port of ProbB.java (Louis Wasserman, lowasser/ICPC2011):
 * java.lang.Math.round snapping, same 80 rotations, same derive() logic.
 */

#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

namespace {

inline int64 java_round(double a) {
  return (int64)floor(a + 0.5);
}

struct Aff2 {
  double m00, m01, m02, m10, m11, m12;
  bool equals(const Aff2& o) const {
    const double eps = 1e-9;
    return fabs(m00 - o.m00) < eps && fabs(m01 - o.m01) < eps && fabs(m02 - o.m02) < eps &&
           fabs(m10 - o.m10) < eps && fabs(m11 - o.m11) < eps && fabs(m12 - o.m12) < eps;
  }
};

Aff2 multiply(const Aff2& A, const Aff2& B) {
  Aff2 R{};
  R.m00 = A.m00 * B.m00 + A.m01 * B.m10;
  R.m01 = A.m00 * B.m01 + A.m01 * B.m11;
  R.m02 = A.m00 * B.m02 + A.m01 * B.m12 + A.m02;
  R.m10 = A.m10 * B.m00 + A.m11 * B.m10;
  R.m11 = A.m10 * B.m01 + A.m11 * B.m11;
  R.m12 = A.m10 * B.m02 + A.m11 * B.m12 + A.m12;
  return R;
}

Aff2 rotate_instance(int64 vx, int64 vy) {
  double L = hypot((double)vx, (double)vy);
  double c = (double)vx / L, s = (double)vy / L;
  Aff2 R{};
  R.m00 = c;
  R.m01 = -s;
  R.m02 = 0;
  R.m10 = s;
  R.m11 = c;
  R.m12 = 0;
  return R;
}

Aff2 scale_then_translate(int sx, int sy, int tx, int ty) {
  Aff2 S{};
  S.m00 = sx;
  S.m01 = 0;
  S.m02 = 0;
  S.m10 = 0;
  S.m11 = sy;
  S.m12 = 0;
  Aff2 T{};
  T.m00 = 1;
  T.m01 = 0;
  T.m02 = tx;
  T.m10 = 0;
  T.m11 = 1;
  T.m12 = ty;
  return multiply(S, T);
}

struct Pt {
  int x, y;
};

struct ST1D {
  int scale;
  int translate;
  bool operator==(const ST1D& o) const {
    return scale == o.scale && translate == o.translate;
  }
  bool operator!=(const ST1D& o) const {
    return !(*this == o);
  }
};

/* Inner derive(x00,x01,x10,x11): throws if diff0==0 && diff1==0 */
optional<ST1D> derive_pair(int x00, int x01, int x10, int x11, bool& nu_exc) {
  nu_exc = false;
  int diff0 = x01 - x00;
  int diff1 = x11 - x10;
  if (diff0 == 0 && diff1 == 0) {
    nu_exc = true;
    return {};
  }
  if (diff0 == 0 || diff1 % diff0 != 0)
    return {};
  ST1D r;
  r.scale = diff1 / diff0;
  r.translate = x10 - r.scale * x00;
  return r;
}

/*
 * Matches Java derive(int[] x0, int[] x1): any tmp==null => return null;
 * mismatch => return null; else all pairs agree; if !unique throw NU at end.
 */
optional<ST1D> derive1d(const int x0[3], const int x1[3], bool& nu_end) {
  nu_end = false;
  bool unique = false;
  optional<ST1D> ans;

  for (int i = 0; i < 3; ++i) {
    for (int j = i + 1; j < 3; ++j) {
      bool exc = false;
      optional<ST1D> tmp = derive_pair(x0[i], x0[j], x1[i], x1[j], exc);
      if (exc)
        continue; // Java empty catch
      if (!tmp.has_value() || (ans.has_value() && *tmp != *ans)) {
        return {};
      }
      ans = *tmp;
      unique = true;
    }
  }

  if (unique && ans.has_value()) {
    for (int i = 0; i < 3; ++i)
      if (ans->scale * x0[i] + ans->translate != x1[i])
        return {};
    return ans;
  }
  nu_end = true;
  return {};
}

optional<Aff2> derive_st(const Pt p0[3], const Pt p1[3], bool& nu1d) {
  nu1d = false;
  int xs0[3] = {p0[0].x, p0[1].x, p0[2].x};
  int ys0[3] = {p0[0].y, p0[1].y, p0[2].y};
  int xs1[3] = {p1[0].x, p1[1].x, p1[2].x};
  int ys1[3] = {p1[0].y, p1[1].y, p1[2].y};

  bool nu_x = false, nu_y = false;
  optional<ST1D> xt = derive1d(xs0, xs1, nu_x);
  optional<ST1D> yt = derive1d(ys0, ys1, nu_y);
  if (nu_x || nu_y) {
    nu1d = true;
    return {};
  }
  if (xt.has_value() && yt.has_value()) {
    return scale_then_translate(xt->scale, yt->scale, xt->translate, yt->translate);
  }
  return {};
}

optional<Aff2> derive_perm(Pt p0r[3], const Pt p1[3], bool& nu_perm) {
  nu_perm = false;
  optional<Aff2> ans;

  for (int i = 0; i < 3; ++i) {
    for (int j = 0; j < 3; ++j) {
      if (i == j)
        continue;
      int k = 0;
      while (k == i || k == j)
        k++;

      Pt triple[3] = {p0r[i], p0r[j], p0r[k]};
      bool nu = false;
      optional<Aff2> tmp = derive_st(triple, p1, nu);
      if (nu) {
        nu_perm = true;
        return {};
      }
      if (!ans.has_value())
        ans = tmp;
      else if (tmp.has_value() && ans.has_value() && !ans->equals(*tmp)) {
        nu_perm = true;
        return {};
      }
    }
  }
  return ans;
}

vector<Aff2> build_rotations() {
  vector<Aff2> rot;
  for (int x = -10; x <= 10; x += 20)
    for (int y = -10; y <= 10; y++)
      rot.push_back(rotate_instance(x, y));
  for (int y = -10; y <= 10; y += 20)
    for (int x = -9; x <= 9; x++)
      rot.push_back(rotate_instance(x, y));
  return rot;
}

optional<Aff2> derive_full(const Pt p0[3], const Pt p1[3], bool& nu_full) {
  const vector<Aff2> ROT = build_rotations();
  nu_full = false;
  optional<Aff2> answer;

  for (const Aff2& R : ROT) {
    Pt p0p[3];
    for (int i = 0; i < 3; ++i) {
      double xr = R.m00 * p0[i].x + R.m01 * p0[i].y;
      double yr = R.m10 * p0[i].x + R.m11 * p0[i].y;
      p0p[i].x = (int)java_round(xr);
      p0p[i].y = (int)java_round(yr);
    }

    bool nu = false;
    optional<Aff2> st = derive_perm(p0p, p1, nu);
    if (nu) {
      nu_full = true;
      return {};
    }
    if (!st.has_value())
      continue;

    Aff2 full = multiply(R, *st);
    if (!answer.has_value())
      answer = full;
    else if (!answer->equals(full)) {
      nu_full = true;
      return {};
    }
  }
  return answer;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int z = 0;
  while (true) {
    Pt p0[3], p1[3];
    for (int i = 0; i < 3; ++i)
      cin >> p0[i].x >> p0[i].y;
    if (p0[0].x == 0 && p0[0].y == 0 && p0[1].x == 0 && p0[1].y == 0 && p0[2].x == 0 &&
        p0[2].y == 0)
      break;
    for (int i = 0; i < 3; ++i)
      cin >> p1[i].x >> p1[i].y;

    cout << "Case " << ++z << ": ";
    bool nu = false;
    optional<Aff2> ans = derive_full(p0, p1, nu);
    if (nu)
      cout << "inconsistent solutions\n";
    else if (!ans.has_value())
      cout << "no solution\n";
    else
      cout << "equivalent solutions\n";
  }
  return 0;
}
