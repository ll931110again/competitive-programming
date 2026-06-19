#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

struct SegTreeCoverLen {
  int nseg = 0;            // number of elementary segments (m-1)
  vector<int> cover_count; // lazy cover count
  vector<i64> cover_len;   // covered length in this node
  vector<i64> ys;          // coordinate breaks, size m

  SegTreeCoverLen() = default;

  explicit SegTreeCoverLen(vector<i64> ys_) {
    init(move(ys_));
  }

  void init(vector<i64> ys_) {
    ys = move(ys_);
    nseg = max(0, (int)ys.size() - 1);
    cover_count.assign(4 * max(1, nseg), 0);
    cover_len.assign(4 * max(1, nseg), 0);
  }

  inline void pull(int v, int l, int r) {
    if (cover_count[v] > 0) {
      cover_len[v] = ys[r + 1] - ys[l];
    } else if (l == r) {
      cover_len[v] = 0;
    } else {
      cover_len[v] = cover_len[v << 1] + cover_len[v << 1 | 1];
    }
  }

  void add_range(int ql, int qr, int delta) { // over segment indices, inclusive
    if (nseg == 0 || ql > qr)
      return;
    add_range_rec(1, 0, nseg - 1, ql, qr, delta);
  }

  void add_range_rec(int v, int l, int r, int ql, int qr, int delta) {
    if (qr < l || r < ql)
      return;
    if (ql <= l && r <= qr) {
      cover_count[v] += delta;
      pull(v, l, r);
      return;
    }
    int m = (l + r) >> 1;
    add_range_rec(v << 1, l, m, ql, qr, delta);
    add_range_rec(v << 1 | 1, m + 1, r, ql, qr, delta);
    pull(v, l, r);
  }

  inline i64 total_covered_len() const {
    return nseg == 0 ? 0 : cover_len[1];
  }
};

struct Event {
  i64 x;
  i64 y1;
  i64 y2; // half-open [y1, y2)
  int delta;
};

inline vector<i64> sorted_unique(vector<i64> v) {
  sort(v.begin(), v.end());
  v.erase(unique(v.begin(), v.end()), v.end());
  return v;
}

bool covered_at_x(const vector<pair<i64, i64>>& mummies, i64 X) {
  const i64 L = -X;
  const i64 R = X;
  const i64 target_y_len = (R + 1) - L; // = 2X+1, in half-open representation

  vector<Event> events;
  events.reserve(mummies.size() * 2);
  vector<i64> xs;
  vector<i64> ys;
  xs.reserve(mummies.size() * 2 + 2);
  ys.reserve(mummies.size() * 2 + 2);

  xs.push_back(L);
  xs.push_back(R + 1);
  ys.push_back(L);
  ys.push_back(R + 1);

  for (auto [mx, my] : mummies) {
    i64 x1 = max(L, mx - X);
    i64 x2 = min(R, mx + X);
    if (x1 > x2)
      continue;
    i64 y1 = max(L, my - X);
    i64 y2 = min(R, my + X);
    if (y1 > y2)
      continue;

    i64 xs0 = x1;
    i64 xe0 = x2 + 1; // half-open
    i64 ys0 = y1;
    i64 ye0 = y2 + 1; // half-open

    events.push_back(Event{xs0, ys0, ye0, +1});
    events.push_back(Event{xe0, ys0, ye0, -1});
    xs.push_back(xs0);
    xs.push_back(xe0);
    ys.push_back(ys0);
    ys.push_back(ye0);
  }

  xs = sorted_unique(move(xs));
  ys = sorted_unique(move(ys));
  if (xs.size() < 2 || ys.size() < 2)
    return false;

  sort(events.begin(), events.end(), [](const Event& a, const Event& b) {
    if (a.x != b.x)
      return a.x < b.x;
    return a.delta > b.delta;
  });

  SegTreeCoverLen st(ys);

  size_t ei = 0;
  for (size_t xi = 0; xi + 1 < xs.size(); xi++) {
    const i64 x = xs[xi];
    const i64 nx = xs[xi + 1];

    while (ei < events.size() && events[ei].x == x) {
      int l = (int)(lower_bound(ys.begin(), ys.end(), events[ei].y1) - ys.begin());
      int r = (int)(lower_bound(ys.begin(), ys.end(), events[ei].y2) - ys.begin());
      // segments are [idx, idx+1), so apply on [l, r-1]
      st.add_range(l, r - 1, events[ei].delta);
      ++ei;
    }

    if (nx - x >= 1) {
      if (st.total_covered_len() < target_y_len)
        return false;
    }
  }

  return true;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  int tc = 1;
  while (cin >> n) {
    if (n == -1)
      break;

    vector<pair<i64, i64>> mummies;
    mummies.reserve(n);
    for (int i = 0; i < n; i++) {
      i64 x, y;
      cin >> x >> y;
      mummies.push_back({x, y});
    }

    cout << "Case " << tc++ << ": ";

    if (n == 0) {
      cout << "never\n";
      continue;
    }

    // Necessary condition for eventual capture: each closed quadrant must
    // contain a mummy. Otherwise, some corner of [-X,X]^2 is never coverable
    // for any X.
    bool qpp = false, qpn = false, qnp = false, qnn = false;
    for (auto [x, y] : mummies) {
      if (x >= 0 && y >= 0)
        qpp = true;
      if (x >= 0 && y <= 0)
        qpn = true;
      if (x <= 0 && y >= 0)
        qnp = true;
      if (x <= 0 && y <= 0)
        qnn = true;
    }
    if (!(qpp && qpn && qnp && qnn)) {
      cout << "never\n";
      continue;
    }

    i64 lo = 0, hi = 1;
    while (!covered_at_x(mummies, hi)) {
      hi <<= 1;
      // With the quadrant condition, hi will eventually work. Guard anyway.
      if (hi > (1LL << 32))
        break;
    }

    while (lo + 1 < hi) {
      i64 mid = lo + ((hi - lo) >> 1);
      if (covered_at_x(mummies, mid))
        hi = mid;
      else
        lo = mid;
    }

    cout << hi << '\n';
  }

  return 0;
}
