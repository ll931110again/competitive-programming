#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

using int64 = long long;
using i128 = __int128_t;

struct Line {
  int64 m = 0; // slope
  int64 b = numeric_limits<int64>::min() / 4;
  Line() = default;
  Line(int64 m_, int64 b_) : m(m_), b(b_) {}
  inline int64 value(int64 x) const {
    i128 v = (i128)m * (i128)x + (i128)b;
    if (v < (i128)numeric_limits<int64>::min())
      return numeric_limits<int64>::min();
    if (v > (i128)numeric_limits<int64>::max())
      return numeric_limits<int64>::max();
    return (int64)v;
  }
};

struct LiChaoMax {
  struct Node {
    Line ln;
    Node *left = nullptr;
    Node *right = nullptr;
    explicit Node(Line l) : ln(l) {}
  };

  int64 minX, maxX;
  Node *root = nullptr;

  LiChaoMax(int64 minX_, int64 maxX_) : minX(minX_), maxX(maxX_) {}

  void addLine(Line nw) { addLineRec(root, minX, maxX, nw); }
  int64 query(int64 x) const { return queryRec(root, minX, maxX, x); }

private:
  void addLineRec(Node *&node, int64 l, int64 r, Line nw) {
    if (!node) {
      node = new Node(nw);
      return;
    }

    int64 mid = l + ((r - l) >> 1);
    bool left = nw.value(l) > node->ln.value(l);
    bool m = nw.value(mid) > node->ln.value(mid);
    if (m)
      swap(nw, node->ln);

    if (l == r)
      return;
    if (left != m)
      addLineRec(node->left, l, mid, nw);
    else
      addLineRec(node->right, mid + 1, r, nw);
  }

  int64 queryRec(Node *node, int64 l, int64 r, int64 x) const {
    if (!node)
      return numeric_limits<int64>::min() / 4;
    int64 best = node->ln.value(x);
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
  vector<int64> a(n + 1);
  for (int i = 1; i <= n; i++)
    cin >> a[i];

  vector<int64> prefix(n + 1, 0);
  for (int i = 1; i <= n; i++)
    prefix[i] = prefix[i - 1] + a[i];

  int64 base = 0;
  for (int i = 1; i <= n; i++)
    base += (int64)i * a[i];

  const int64 minX = -1000000;
  const int64 maxX = 1000000;

  vector<int64> bestLeft(n + 1, numeric_limits<int64>::min() / 4);
  vector<int64> bestRight(n + 1, numeric_limits<int64>::min() / 4);

  // Prefix hull for move-left: max_{q<=p} (q*x - prefix[q-1]).
  {
    LiChaoMax hull(minX, maxX);
    for (int p = 1; p <= n; p++) {
      hull.addLine(Line(p, -prefix[p - 1]));
      bestLeft[p] = hull.query(a[p]);
    }
  }

  // Suffix hull for move-right: max_{q>=p} (q*x - prefix[q]).
  {
    LiChaoMax hull(minX, maxX);
    for (int p = n; p >= 1; p--) {
      hull.addLine(Line(p, -prefix[p]));
      bestRight[p] = hull.query(a[p]);
    }
  }

  int64 bestDelta = 0;
  for (int p = 1; p <= n; p++) {
    int64 x = a[p];
    // deltaLeft = (prefix[p-1] - p*x) + max_{q<=p}(q*x - prefix[q-1])
    int64 deltaLeft = (prefix[p - 1] - (int64)p * x) + bestLeft[p];
    // deltaRight = (prefix[p] - p*x) + max_{q>=p}(q*x - prefix[q])
    int64 deltaRight = (prefix[p] - (int64)p * x) + bestRight[p];
    bestDelta = max(bestDelta, max(deltaLeft, deltaRight));
  }

  cout << (base + bestDelta) << "\n";
  return 0;
}
