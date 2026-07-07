// AHC021 A - Triangle Sort: https://atcoder.jp/contests/ahc021/tasks/ahc021_a
//
// Max-heap target layout (largest at root). BFS-route each misplaced ball via
// adjacent swaps; repeat until heap order holds or turn budget is exhausted.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int N = 30;
constexpr int N2 = N * (N + 1) / 2;

using P = pair<int, int>;

int bs[N][N];
P pos[N2];
vector<P> ops;

bool is_adj(P a, P b) {
  auto [x1, y1] = a;
  auto [x2, y2] = b;
  if (x1 == x2)
    return y1 == y2 + 1 || y1 + 1 == y2;
  if (x1 + 1 == x2)
    return y1 == y2 || y1 + 1 == y2;
  if (x1 == x2 + 1)
    return y1 == y2 || y1 == y2 + 1;
  return false;
}

vector<P> neighbors(P p) {
  const int x = p.first, y = p.second;
  vector<P> res;
  if (y > 0)
    res.emplace_back(x, y - 1);
  if (y + 1 <= x)
    res.emplace_back(x, y + 1);
  if (x + 1 < N) {
    res.emplace_back(x + 1, y);
    if (y + 1 <= x + 1)
      res.emplace_back(x + 1, y + 1);
  }
  if (x > 0) {
    if (y > 0)
      res.emplace_back(x - 1, y - 1);
    if (y <= x - 1)
      res.emplace_back(x - 1, y);
  }
  return res;
}

void swap_cells(P a, P b) {
  const int v1 = bs[a.first][a.second];
  const int v2 = bs[b.first][b.second];
  bs[a.first][a.second] = v2;
  bs[b.first][b.second] = v1;
  pos[v1] = b;
  pos[v2] = a;
  ops.push_back(a);
  ops.push_back(b);
}

vector<P> bfs_path(P from, P to) {
  if (from == to)
    return {};
  queue<P> q;
  map<P, P> par;
  q.push(from);
  par[from] = {-1, -1};
  while (!q.empty()) {
    P cur = q.front();
    q.pop();
    if (cur == to)
      break;
    for (P nb : neighbors(cur)) {
      if (par.count(nb))
        continue;
      par[nb] = cur;
      q.push(nb);
    }
  }
  if (!par.count(to))
    return {};
  vector<P> path;
  for (P cur = to; par[cur].first >= 0; cur = par[cur])
    path.push_back(cur);
  reverse(path.begin(), path.end());
  return path;
}

int violations() {
  int num = 0;
  for (int x = 0; x + 1 < N; ++x) {
    for (int y = 0; y <= x; ++y) {
      if (bs[x][y] > bs[x + 1][y])
        ++num;
      if (bs[x][y] > bs[x + 1][y + 1])
        ++num;
    }
  }
  return num;
}

P target_pos[N2];

void build_targets() {
  vector<P> order;
  order.reserve(N2);
  queue<P> q;
  q.emplace(0, 0);
  vector<vector<bool>> vis(N, vector<bool>(N, false));
  vis[0][0] = true;
  while (!q.empty()) {
    P p = q.front();
    q.pop();
    order.push_back(p);
    for (P nb : neighbors(p)) {
      if (!vis[nb.first][nb.second]) {
        vis[nb.first][nb.second] = true;
        q.push(nb);
      }
    }
  }
  for (int r = 0; r < N2; ++r) {
    target_pos[N2 - 1 - r] = order[r];
  }
}

void move_ball(int v) {
  P goal = target_pos[v];
  while (pos[v] != goal && (int)ops.size() + 1 < 10000) {
    vector<P> path = bfs_path(pos[v], goal);
    if (path.size() < 2)
      break;
    swap_cells(pos[v], path[1]);
  }
}

void solve() {
  build_targets();
  for (int v = N2 - 1; v >= 0 && (int)ops.size() < 10000; --v)
    move_ball(v);

  while (violations() > 0 && (int)ops.size() + 1 < 10000) {
    bool improved = false;
    for (int x = 0; x + 1 < N && !improved; ++x) {
      for (int y = 0; y <= x && !improved; ++y) {
        const int v = bs[x][y];
        if (v > bs[x + 1][y]) {
          swap_cells({x, y}, {x + 1, y});
          improved = true;
        } else if (v > bs[x + 1][y + 1]) {
          swap_cells({x, y}, {x + 1, y + 1});
          improved = true;
        }
      }
    }
    if (!improved)
      break;
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  for (int i = 0; i < N; ++i) {
    for (int j = 0; j <= i; ++j) {
      cin >> bs[i][j];
      pos[bs[i][j]] = {i, j};
    }
  }

  solve();

  cout << ops.size() << '\n';
  for (size_t i = 0; i + 1 < ops.size(); i += 2) {
    cout << ops[i].first << ' ' << ops[i].second << ' ';
    cout << ops[i + 1].first << ' ' << ops[i + 1].second << '\n';
  }
  return 0;
}
