#include <algorithm>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

using int64 = long long;

static const int64 INF64 = (int64)4e18;

struct Line {
  int64 slope = 0;         // m
  int64 intercept = INF64; // b

  Line() = default;
  Line(int64 slope_, int64 intercept_) : slope(slope_), intercept(intercept_) {}

  inline int64 value(int64 x) const {
    __int128 v = (__int128)slope * (__int128)x + (__int128)intercept;
    if (v < (__int128)LLONG_MIN)
      return LLONG_MIN;
    if (v > (__int128)LLONG_MAX)
      return LLONG_MAX;
    return (int64)v;
  }
};

// Li Chao tree for minimum on a discrete x-domain (coordinate-compressed).
struct LiChaoMin {
  struct Node {
    Line line;
    Node *left = nullptr;
    Node *right = nullptr;
    explicit Node(Line line_) : line(line_) {}
  };

  const vector<int64> *xs = nullptr;
  Node *root = nullptr;

  explicit LiChaoMin(const vector<int64> &xs_) : xs(&xs_) {}

  bool empty() const { return root == nullptr; }

  void addLine(Line newLine) {
    addLineRec(root, 0, (int)xs->size() - 1, newLine);
  }

  int64 query(int64 x) const {
    int idx = (int)(lower_bound(xs->begin(), xs->end(), x) - xs->begin());
    return queryRec(root, 0, (int)xs->size() - 1, idx);
  }

private:
  void addLineRec(Node *&node, int segLeft, int segRight, Line newLine) {
    if (!node) {
      node = new Node(newLine);
      return;
    }

    int mid = segLeft + ((segRight - segLeft) >> 1);
    int64 xL = (*xs)[segLeft];
    int64 xM = (*xs)[mid];

    bool betterAtLeft = newLine.value(xL) < node->line.value(xL);
    bool betterAtMid = newLine.value(xM) < node->line.value(xM);
    if (betterAtMid)
      swap(newLine, node->line);

    if (segLeft == segRight)
      return;

    if (betterAtLeft != betterAtMid)
      addLineRec(node->left, segLeft, mid, newLine);
    else
      addLineRec(node->right, mid + 1, segRight, newLine);
  }

  int64 queryRec(Node *node, int segLeft, int segRight, int xIdx) const {
    if (!node)
      return INF64;
    int64 x = (*xs)[xIdx];
    int64 best = node->line.value(x);
    if (segLeft == segRight)
      return best;
    int mid = segLeft + ((segRight - segLeft) >> 1);
    if (xIdx <= mid)
      return min(best, queryRec(node->left, segLeft, mid, xIdx));
    return min(best, queryRec(node->right, mid + 1, segRight, xIdx));
  }
};

struct Hull {
  LiChaoMin lichao;
  vector<Line> lines; // for DSU merging (reinsert into a larger hull)

  explicit Hull(const vector<int64> &xs) : lichao(xs) {}

  void addLine(const Line &ln) {
    lichao.addLine(ln);
    lines.push_back(ln);
  }

  int64 query(int64 x) const { return lichao.query(x); }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;

  vector<int64> a(n + 1), b(n + 1);
  for (int i = 1; i <= n; i++)
    cin >> a[i];
  for (int i = 1; i <= n; i++)
    cin >> b[i];

  vector<vector<int>> adj(n + 1);
  for (int i = 0; i < n - 1; i++) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }

  // Root at 1 and build parent/children order.
  vector<int> parent(n + 1, 0);
  vector<int> order;
  order.reserve(n);
  parent[1] = -1;
  {
    vector<int> st;
    st.push_back(1);
    while (!st.empty()) {
      int u = st.back();
      st.pop_back();
      order.push_back(u);
      for (int v : adj[u]) {
        if (v == parent[u])
          continue;
        parent[v] = u;
        st.push_back(v);
      }
    }
  }

  vector<vector<int>> children(n + 1);
  children.assign(n + 1, {});
  for (int u = 2; u <= n; u++)
    children[parent[u]].push_back(u);

  vector<int64> xs;
  xs.reserve(n);
  for (int i = 1; i <= n; i++)
    xs.push_back(a[i]);
  sort(xs.begin(), xs.end());
  xs.erase(unique(xs.begin(), xs.end()), xs.end());

  vector<int64> dp(n + 1, 0);
  vector<Hull *> hullPtr(n + 1, nullptr);

  for (int idx = (int)order.size() - 1; idx >= 0; idx--) {
    int u = order[idx];

    if (children[u].empty()) {
      dp[u] = 0;
      auto *h = new Hull(xs);
      h->addLine(Line(b[u], dp[u]));
      hullPtr[u] = h;
      continue;
    }

    // Pick the child hull with the most lines as the base.
    int heavyChild = children[u][0];
    for (int v : children[u]) {
      if (hullPtr[v]->lines.size() > hullPtr[heavyChild]->lines.size())
        heavyChild = v;
    }
    Hull *base = hullPtr[heavyChild];

    // Merge smaller child hulls into the base.
    for (int v : children[u]) {
      if (v == heavyChild)
        continue;
      for (const auto &ln : hullPtr[v]->lines)
        base->addLine(ln);
    }

    // dp[u] = min_{descendant v} (a[u] * b[v] + dp[v])
    dp[u] = base->query(a[u]);

    // Add u itself so ancestors can jump to u.
    base->addLine(Line(b[u], dp[u]));
    hullPtr[u] = base;
  }

  for (int i = 1; i <= n; i++) {
    cout << dp[i] << (i == n ? '\n' : ' ');
  }
  return 0;
}
