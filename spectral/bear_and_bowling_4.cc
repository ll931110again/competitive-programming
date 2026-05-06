#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

using int64 = long long;
using i128 = __int128_t;

struct Line {
  int64 m = 0;            // slope
  int64 b = (int64)-4e18; // intercept
  Line() = default;
  Line(int64 m_, int64 b_) : m(m_), b(b_) {}
  inline i128 value(int64 x) const { return (i128)m * (i128)x + (i128)b; }
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
  i128 query(int64 x) const { return queryRec(root, minX, maxX, x); }

private:
  void addLineRec(Node *&node, int64 l, int64 r, Line nw) {
    if (!node) {
      node = new Node(nw);
      return;
    }
    int64 mid = l + ((r - l) >> 1);

    bool betterAtLeft = nw.value(l) > node->ln.value(l);
    bool betterAtMid = nw.value(mid) > node->ln.value(mid);
    if (betterAtMid)
      swap(nw, node->ln);

    if (l == r)
      return;
    if (betterAtLeft != betterAtMid)
      addLineRec(node->left, l, mid, nw);
    else
      addLineRec(node->right, mid + 1, r, nw);
  }

  i128 queryRec(Node *node, int64 l, int64 r, int64 x) const {
    if (!node)
      return -(i128)1 << 120;
    i128 best = node->ln.value(x);
    if (l == r)
      return best;
    int64 mid = l + ((r - l) >> 1);
    if (x <= mid)
      return max(best, queryRec(node->left, l, mid, x));
    return max(best, queryRec(node->right, mid + 1, r, x));
  }
};

static void printI128(i128 v) {
  if (v == 0) {
    cout << 0 << "\n";
    return;
  }
  if (v < 0) {
    cout << '-';
    v = -v;
  }
  string s;
  while (v) {
    int digit = (int)(v % 10);
    s.push_back(char('0' + digit));
    v /= 10;
  }
  reverse(s.begin(), s.end());
  cout << s << "\n";
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<int64> a(n + 1);
  for (int i = 1; i <= n; i++)
    cin >> a[i];

  // Prefix sums:
  // P[i] = sum_{1..i} a
  // Q[i] = sum_{1..i} a[i] * i
  vector<int64> P(n + 1, 0);
  vector<int64> Q(n + 1, 0);
  for (int i = 1; i <= n; i++) {
    P[i] = P[i - 1] + a[i];
    Q[i] = Q[i - 1] + (int64)i * a[i];
  }

  int64 minP = P[0], maxP = P[0];
  for (int i = 1; i <= n; i++) {
    minP = min(minP, P[i]);
    maxP = max(maxP, P[i]);
  }

  LiChaoMax hull(minP, maxP);

  // For k = l-1, define line:
  // f_k(x) = (-k) * x + (k * P[k] - Q[k])
  // For r, score(l,r) = Q[r] + f_{l-1}(P[r]).
  // Start with k=0 (l=1).
  hull.addLine(Line(0, 0)); // k=0 => 0*x + 0

  i128 best = 0; // allowed to cancel all rolls => score 0

  for (int r = 1; r <= n; r++) {
    i128 candidate = (i128)Q[r] + hull.query(P[r]);
    if (candidate > best)
      best = candidate;

    int64 k = r; // make k=r available for future (l=r+1)
    i128 intercept = (i128)k * (i128)P[k] - (i128)Q[k];
    // intercept fits in 64-bit comfortably here, but store as int64 for Line.
    // Q up to ~ n^2*1e7 => 4e17, k*P up to 2e5*2e12=4e17.
    hull.addLine(Line(-k, (int64)intercept));
  }

  printI128(best);
  return 0;
}
