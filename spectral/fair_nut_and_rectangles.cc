#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

using int64 = long long;

static void printInt128(__int128 v) {
  if (v == 0) {
    cout << 0;
    return;
  }
  if (v < 0) {
    cout << '-';
    v = -v;
  }
  string s;
  while (v > 0) {
    int digit = (int)(v % 10);
    s.push_back((char)('0' + digit));
    v /= 10;
  }
  reverse(s.begin(), s.end());
  cout << s;
}

struct Rect {
  int64 x = 0;
  int64 y = 0;
  int64 a = 0;
};

struct Line {
  int64 slope = 0;
  __int128 intercept = 0;

  Line() = default;
  Line(int64 slope_, __int128 intercept_)
      : slope(slope_), intercept(intercept_) {}

  inline __int128 value(int64 x) const {
    return (__int128)slope * (__int128)x + intercept;
  }
};

// Li Chao tree for maximum on integer x in [minX, maxX] inclusive.
struct LiChaoMax {
  struct Node {
    Line line;
    Node *left = nullptr;
    Node *right = nullptr;
    explicit Node(Line ln) : line(ln) {}
  };

  int64 minX, maxX;
  Node *root = nullptr;

  LiChaoMax(int64 minX_, int64 maxX_) : minX(minX_), maxX(maxX_) {}

  void addLine(Line newLine) { addLineRec(root, minX, maxX, newLine); }

  __int128 query(int64 x) const { return queryRec(root, minX, maxX, x); }

private:
  void addLineRec(Node *&node, int64 l, int64 r, Line newLine) {
    if (!node) {
      node = new Node(newLine);
      return;
    }

    int64 mid = l + ((r - l) >> 1);
    bool betterAtLeft = newLine.value(l) > node->line.value(l);
    bool betterAtMid = newLine.value(mid) > node->line.value(mid);
    if (betterAtMid)
      swap(newLine, node->line);

    if (l == r)
      return;
    if (betterAtLeft != betterAtMid)
      addLineRec(node->left, l, mid, newLine);
    else
      addLineRec(node->right, mid + 1, r, newLine);
  }

  __int128 queryRec(Node *node, int64 l, int64 r, int64 x) const {
    if (!node)
      return -((__int128)1 << 120);
    __int128 best = node->line.value(x);
    if (l == r)
      return best;
    int64 mid = l + ((r - l) >> 1);
    if (x <= mid)
      return max(best, queryRec(node->left, l, mid, x));
    return max(best, queryRec(node->right, mid + 1, r, x));
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;

  vector<Rect> rects;
  rects.reserve(n);
  for (int i = 0; i < n; i++) {
    Rect r;
    cin >> r.x >> r.y >> r.a;
    rects.push_back(r);
  }

  // Sort by decreasing x. If x ties, keep larger y first (nested/duplicates can
  // be dropped later).
  sort(rects.begin(), rects.end(), [](const Rect &p, const Rect &q) {
    if (p.x != q.x)
      return p.x > q.x;
    return p.y > q.y;
  });

  // Keep rectangles with strictly increasing y (in this x-desc order), removing
  // dominated ones.
  vector<Rect> filtered;
  filtered.reserve(rects.size());
  int64 lastY = -1;
  for (const auto &r : rects) {
    if (r.y <= lastY)
      continue;
    filtered.push_back(r);
    lastY = r.y;
  }

  // DP:
  // dp[i] = x_i * y_i - a_i + max_j (dp[j] - x_i * y_j) over previously
  // processed j (with smaller y). Maintain lines: f_j(x) = (-y_j) * x + dp[j].
  LiChaoMax hull(0, (int64)1e9);
  hull.addLine(Line(0, 0)); // base: dp=0, y=0

  __int128 bestAnswer = 0;
  for (const auto &r : filtered) {
    __int128 bestPrev = hull.query(r.x);
    __int128 dp = (__int128)r.x * (__int128)r.y - (__int128)r.a + bestPrev;
    bestAnswer = max(bestAnswer, dp);
    hull.addLine(Line(-r.y, dp));
  }

  printInt128(bestAnswer);
  cout << "\n";
  return 0;
}
