#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class STable {
  string s, t;

  bool lessS(const string& a, const string& b) const {
    if (a == b)
      return false;
    if (a.size() < b.size() && a == b.substr(0, a.size()))
      return true;
    for (int i = 0; i < (int)min(a.size(), b.size()); i++) {
      if (a[i] != b[i])
        return a[i] < b[i];
    }
    return a.size() < b.size();
  }

  string combine(const string& a, const string& b) const {
    if (lessS(a, b))
      return a + b;
    if (lessS(b, a))
      return b + a;
    return a + b;
  }

  struct Mat {
    string a, b;
  };

  Mat mul(const Mat& x, const Mat& y) const {
    Mat r;
    r.a = combine(x.a, y.a);
    r.b = combine(x.b, y.b);
    return r;
  }

  Mat step(const Mat& cur, char ch) const {
    Mat r;
    string one(1, ch);
    r.a = combine(cur.a, one);
    r.b = combine(cur.b, one);
    return r;
  }

  Mat power(Mat base, i64 exp) const {
    Mat res;
    res.a = s;
    res.b = t;
    while (exp > 0) {
      if (exp & 1)
        res = mul(res, base);
      base = mul(base, base);
      exp >>= 1;
    }
    return res;
  }

  i64 len(i64 n, i64 m) const {
    if (n == 0)
      return m;
    if (m == 0)
      return n;
    return len(n - 1, m) + len(n, m - 1) + 1;
  }

  string build(i64 n, i64 m, i64 pos, i64 lim) const {
    if (n == 0) {
      string r;
      for (i64 i = 0; i < m && (int)r.size() < lim; i++)
        r.push_back(t[i]);
      return r.substr(pos, lim);
    }
    if (m == 0) {
      string r;
      for (i64 i = 0; i < n && (int)r.size() < lim; i++)
        r.push_back(s[i]);
      return r.substr(pos, lim);
    }
    i64 l1 = len(n - 1, m);
    i64 l2 = len(n, m - 1);
    i64 l3 = 1;
    i64 total = l1 + l2 + l3;
    if (pos < l1)
      return build(n - 1, m, pos, lim);
    pos -= l1;
    if (pos < l2)
      return build(n, m - 1, pos, lim);
    pos -= l2;
    string a = build(n - 1, m, len(n - 1, m) - 1, 1);
    string b = build(n, m - 1, len(n, m - 1) - 1, 1);
    string c = combine(a, b);
    if (pos == 0)
      return c.substr(0, lim);
    return "";
  }

public:
  string getString(string s_, string t_, i64 pos) {
    s = s_;
    t = t_;
    i64 n = s.size(), m = t.size();
    i64 L = len(n, m);
    i64 take = min(50LL, L - pos);
    return build(n, m, pos, take);
  }
};
