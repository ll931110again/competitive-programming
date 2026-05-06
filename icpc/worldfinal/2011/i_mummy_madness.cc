#include <bits/stdc++.h>

using namespace std;

struct SegTreeCoverLen {
  int nseg = 0;                // number of elementary segments (m-1)
  vector<int> coverCount;      // lazy cover count
  vector<long long> coverLen;  // covered length in this node
  vector<long long> ys;        // coordinate breaks, size m

  SegTreeCoverLen() = default;

  explicit SegTreeCoverLen(vector<long long> ys_) { init(std::move(ys_)); }

  void init(vector<long long> ys_) {
    ys = std::move(ys_);
    nseg = max(0, (int)ys.size() - 1);
    coverCount.assign(4 * max(1, nseg), 0);
    coverLen.assign(4 * max(1, nseg), 0);
  }

  inline void pull(int v, int l, int r) {
    if (coverCount[v] > 0) {
      coverLen[v] = ys[r + 1] - ys[l];
    } else if (l == r) {
      coverLen[v] = 0;
    } else {
      coverLen[v] = coverLen[v << 1] + coverLen[v << 1 | 1];
    }
  }

  void addRange(int ql, int qr, int delta) { // over segment indices, inclusive
    if (nseg == 0 || ql > qr) return;
    addRangeRec(1, 0, nseg - 1, ql, qr, delta);
  }

  void addRangeRec(int v, int l, int r, int ql, int qr, int delta) {
    if (qr < l || r < ql) return;
    if (ql <= l && r <= qr) {
      coverCount[v] += delta;
      pull(v, l, r);
      return;
    }
    int m = (l + r) >> 1;
    addRangeRec(v << 1, l, m, ql, qr, delta);
    addRangeRec(v << 1 | 1, m + 1, r, ql, qr, delta);
    pull(v, l, r);
  }

  inline long long totalCoveredLen() const { return nseg == 0 ? 0 : coverLen[1]; }
};

struct Event {
  long long x;
  long long y1;
  long long y2; // half-open [y1, y2)
  int delta;
};

static inline vector<long long> sortedUnique(vector<long long> v) {
  sort(v.begin(), v.end());
  v.erase(unique(v.begin(), v.end()), v.end());
  return v;
}

static bool coveredAtX(const vector<pair<long long, long long>> &mummies, long long X) {
  const long long L = -X;
  const long long R = X;
  const long long targetYLen = (R + 1) - L; // = 2X+1, in half-open representation

  vector<Event> events;
  events.reserve(mummies.size() * 2);
  vector<long long> xs;
  vector<long long> ys;
  xs.reserve(mummies.size() * 2 + 2);
  ys.reserve(mummies.size() * 2 + 2);

  xs.push_back(L);
  xs.push_back(R + 1);
  ys.push_back(L);
  ys.push_back(R + 1);

  for (auto [mx, my] : mummies) {
    long long x1 = max(L, mx - X);
    long long x2 = min(R, mx + X);
    if (x1 > x2) continue;
    long long y1 = max(L, my - X);
    long long y2 = min(R, my + X);
    if (y1 > y2) continue;

    long long xs0 = x1;
    long long xe0 = x2 + 1; // half-open
    long long ys0 = y1;
    long long ye0 = y2 + 1; // half-open

    events.push_back(Event{xs0, ys0, ye0, +1});
    events.push_back(Event{xe0, ys0, ye0, -1});
    xs.push_back(xs0);
    xs.push_back(xe0);
    ys.push_back(ys0);
    ys.push_back(ye0);
  }

  xs = sortedUnique(std::move(xs));
  ys = sortedUnique(std::move(ys));
  if (xs.size() < 2 || ys.size() < 2) return false;

  sort(events.begin(), events.end(), [](const Event &a, const Event &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.delta > b.delta;
  });

  SegTreeCoverLen st(ys);

  size_t ei = 0;
  for (size_t xi = 0; xi + 1 < xs.size(); xi++) {
    const long long x = xs[xi];
    const long long nx = xs[xi + 1];

    while (ei < events.size() && events[ei].x == x) {
      int l = (int)(lower_bound(ys.begin(), ys.end(), events[ei].y1) - ys.begin());
      int r = (int)(lower_bound(ys.begin(), ys.end(), events[ei].y2) - ys.begin());
      // segments are [idx, idx+1), so apply on [l, r-1]
      st.addRange(l, r - 1, events[ei].delta);
      ++ei;
    }

    if (nx - x >= 1) {
      if (st.totalCoveredLen() < targetYLen) return false;
    }
  }

  return true;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  int tc = 1;
  while (cin >> n) {
    if (n == -1) break;

    vector<pair<long long, long long>> mummies;
    mummies.reserve(n);
    for (int i = 0; i < n; i++) {
      long long x, y;
      cin >> x >> y;
      mummies.push_back({x, y});
    }

    cout << "Case " << tc++ << ": ";

    if (n == 0) {
      cout << "never\n";
      continue;
    }

    // Necessary condition for eventual capture: each closed quadrant must contain a mummy.
    // Otherwise, some corner of [-X,X]^2 is never coverable for any X.
    bool qpp = false, qpn = false, qnp = false, qnn = false;
    for (auto [x, y] : mummies) {
      if (x >= 0 && y >= 0) qpp = true;
      if (x >= 0 && y <= 0) qpn = true;
      if (x <= 0 && y >= 0) qnp = true;
      if (x <= 0 && y <= 0) qnn = true;
    }
    if (!(qpp && qpn && qnp && qnn)) {
      cout << "never\n";
      continue;
    }

    long long lo = 0, hi = 1;
    while (!coveredAtX(mummies, hi)) {
      hi <<= 1;
      // With the quadrant condition, hi will eventually work. Guard anyway.
      if (hi > (1LL << 32)) break;
    }

    while (lo + 1 < hi) {
      long long mid = lo + ((hi - lo) >> 1);
      if (coveredAtX(mummies, mid))
        hi = mid;
      else
        lo = mid;
    }

    cout << hi << '\n';
  }

  return 0;
}
