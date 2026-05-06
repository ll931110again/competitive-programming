#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>

using namespace std;

using int64 = long long;

static inline int64 clamp128ToI64(__int128 v) {
  if (v < (__int128)numeric_limits<int64>::min())
    return numeric_limits<int64>::min();
  if (v > (__int128)numeric_limits<int64>::max())
    return numeric_limits<int64>::max();
  return (int64)v;
}

struct Line {
  int64 slope = 0;     // m
  int64 intercept = 0; // c

  Line() = default;
  Line(int64 slope_, int64 intercept_) : slope(slope_), intercept(intercept_) {}

  inline int64 value(int64 x) const {
    return clamp128ToI64((__int128)slope * (__int128)x + (__int128)intercept);
  }
};

// Li Chao tree for minimum over x in [minX, maxX] (inclusive), with tie-break
// by smaller slope.
struct LiChaoMin {
  struct Node {
    Line line;
    Node *left = nullptr;
    Node *right = nullptr;
    explicit Node(Line ln) : line(ln) {}
  };

  int64 minX, maxX;
  Node *root = nullptr;

  LiChaoMin(int64 minX_, int64 maxX_) : minX(minX_), maxX(maxX_) {}

  void addLine(Line newLine) { addLineRec(root, minX, maxX, newLine); }

  // Returns the best line (not just value) at x.
  Line queryLine(int64 x) const { return queryLineRec(root, minX, maxX, x); }

private:
  static inline bool betterAt(const Line &a, const Line &b, int64 x) {
    int64 va = a.value(x);
    int64 vb = b.value(x);
    if (va != vb)
      return va < vb;
    return a.slope < b.slope;
  }

  void addLineRec(Node *&node, int64 l, int64 r, Line newLine) {
    if (!node) {
      node = new Node(newLine);
      return;
    }

    int64 mid = l + ((r - l) >> 1);

    // Ensure node->line is better at mid.
    if (betterAt(newLine, node->line, mid))
      swap(newLine, node->line);

    if (l == r)
      return;

    // Now decide where newLine can still win.
    if (betterAt(newLine, node->line, l) != betterAt(newLine, node->line, mid))
      addLineRec(node->left, l, mid, newLine);
    else
      addLineRec(node->right, mid + 1, r, newLine);
  }

  Line queryLineRec(Node *node, int64 l, int64 r, int64 x) const {
    if (!node)
      return Line(0, numeric_limits<int64>::max() / 4);
    Line best = node->line;
    if (l == r)
      return best;
    int64 mid = l + ((r - l) >> 1);
    Line cand = (x <= mid) ? queryLineRec(node->left, l, mid, x)
                           : queryLineRec(node->right, mid + 1, r, x);
    if (betterAt(cand, best, x))
      best = cand;
    return best;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int64 n;
  int m;
  cin >> n >> m;

  // Train cars live on an integer coordinate line.
  // Current cars occupy [headCoord, tailCoord] inclusive, with indices i =
  // coord - headCoord + 1.
  int64 headCoord = 0;
  int64 tailCoord = n - 1;

  // Global accumulated recalculation effect on existing cars:
  // For a car at coordinate p, each type-3 adds: b + s*(p -
  // headCoord_at_that_time). We maintain A(p) = (S * p + C) + offset(p), where
  // offset(p) is fixed at insertion time so that new cars start with A=0.
  int64 S = 0; // total slope in p
  int64 C = 0; // total intercept

  // x queries are S, which is nondecreasing. Upper bound: sum of s <= 1e18.
  LiChaoMin hull(0, (int64)1e18);

  auto addBatchRepresentative = [&](int64 coord) {
    // New car value should be 0 at insertion time:
    // offset = -(S*coord + C)
    // So (S*coord + C + offset) == 0 now, and later equals m*S' + intercept +
    // C'.
    int64 intercept =
        clamp128ToI64(-((__int128)S * (__int128)coord + (__int128)C));
    hull.addLine(Line(coord, intercept));
  };

  // Initial batch at time 0: only the smallest coordinate can ever be optimal
  // (S is nondecreasing).
  addBatchRepresentative(headCoord);

  for (int qi = 0; qi < m; qi++) {
    int type;
    cin >> type;
    if (type == 1) {
      int64 k;
      cin >> k;
      headCoord -= k;
      addBatchRepresentative(headCoord);
    } else if (type == 2) {
      int64 k;
      cin >> k;
      int64 firstNew = tailCoord + 1;
      tailCoord += k;
      addBatchRepresentative(firstNew);
    } else {
      int64 b, s;
      cin >> b >> s;
      // Add to all existing cars: b + s*(p - headCoord) = s*p + (b -
      // s*headCoord)
      S = clamp128ToI64((__int128)S + (__int128)s);
      C = clamp128ToI64((__int128)C + (__int128)b -
                        (__int128)s * (__int128)headCoord);
    }

    Line best = hull.queryLine(S);
    int64 minValue = clamp128ToI64((__int128)best.value(S) + (__int128)C);
    int64 bestCoord = best.slope;
    int64 bestIndex = bestCoord - headCoord + 1;
    cout << bestIndex << ' ' << minValue << "\n";
  }

  return 0;
}
