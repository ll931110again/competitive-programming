#include <algorithm>
#include <iostream>
#include <vector>
 
using namespace std;
 
struct Machine {
  long long d, p, r, g;
};
 
struct Line {
  long long m = 0; // slope
  long long b = -(1LL << 60);
  Line() = default;
  Line(long long m_, long long b_) : m(m_), b(b_) {}
  inline long long value(long long x) const {
    __int128 v = (__int128)m * (__int128)x + (__int128)b;
    if (v < (__int128)LLONG_MIN) return LLONG_MIN;
    if (v > (__int128)LLONG_MAX) return LLONG_MAX;
    return (long long)v;
  }
};
 
struct LiChaoMax {
  struct Node {
    Line ln;
    Node *l = nullptr;
    Node *r = nullptr;
    explicit Node(Line ln_) : ln(ln_) {}
  };
 
  long long xl, xr;
  Node *root = nullptr;
 
  LiChaoMax(long long xl_, long long xr_) : xl(xl_), xr(xr_) {}
 
  void addLine(Line nw) { addLineRec(root, xl, xr, nw); }
 
  void addLineRec(Node *&nd, long long l, long long r, Line nw) {
    if (!nd) {
      nd = new Node(nw);
      return;
    }
 
    long long mid = l + ((r - l) >> 1);
    bool left = nw.value(l) > nd->ln.value(l);
    bool m = nw.value(mid) > nd->ln.value(mid);
    if (m) swap(nw, nd->ln);
 
    if (l == r) return;
    if (left != m)
      addLineRec(nd->l, l, mid, nw);
    else
      addLineRec(nd->r, mid + 1, r, nw);
  }
 
  long long query(long long x) const { return queryRec(root, xl, xr, x); }
 
  long long queryRec(Node *nd, long long l, long long r, long long x) const {
    if (!nd) return LLONG_MIN;
    long long res = nd->ln.value(x);
    if (l == r) return res;
    long long mid = l + ((r - l) >> 1);
    if (x <= mid) return max(res, queryRec(nd->l, l, mid, x));
    return max(res, queryRec(nd->r, mid + 1, r, x));
  }
};
 
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
 
  int tc = 1;
 
  int N;
  long long C, D;
  while (cin >> N >> C >> D) {
    if (N == 0 && C == 0 && D == 0) break;
 
    vector<Machine> ms;
    ms.reserve(N);
    for (int i = 0; i < N; i++) {
      Machine t;
      cin >> t.d >> t.p >> t.r >> t.g;
      ms.push_back(t);
    }
    sort(ms.begin(), ms.end(), [](const Machine &a, const Machine &b) {
      if (a.d != b.d) return a.d < b.d;
      return a.p < b.p;
    });
 
    LiChaoMax hull(1, D + 1);
    hull.addLine(Line(0, C)); // keep cash, no machine
 
    for (int i = 0; i < N;) {
      long long day = ms[i].d;
      long long money = hull.query(day);
 
      vector<Line> toAdd;
      for (; i < N && ms[i].d == day; i++) {
        const auto &mch = ms[i];
        if (money < mch.p) continue;
        // If we buy this machine on 'day' using 'money' cash, then selling on day x yields:
        // money - p + r + g*(x - day - 1) = g*x + (money - p + r - g*(day+1))
        long long b = money - mch.p + mch.r - mch.g * (day + 1);
        toAdd.push_back(Line(mch.g, b));
      }
 
      for (const auto &ln : toAdd) hull.addLine(ln);
    }
 
    long long ans = hull.query(D + 1);
    cout << "Case " << tc++ << ": " << ans << "\n";
  }
 
  return 0;
}