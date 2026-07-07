// AHC023 A - Farm Planning: https://atcoder.jp/contests/ahc023/tasks/asprocon10_a
//
// Schedule one planting per crop on reachable empty cells from the start row.
// Plant each crop k at month 1 (within its S[k] deadline) on a BFS cell.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

int T, H, W, i0, K;
vector<string> h_raw, v_raw;
vector<int> S, D;
vector<vector<bool>> blocked_h, blocked_v;

bool can_move(int r, int c, int nr, int nc) {
  if (nr < 0 || nc < 0 || nr >= H || nc >= W)
    return false;
  if (nr == r + 1)
    return !blocked_h[r][c];
  if (nr == r - 1)
    return !blocked_h[r - 1][c];
  if (nc == c + 1)
    return !blocked_v[r][c];
  if (nc == c - 1)
    return !blocked_v[r][c - 1];
  return false;
}

vector<pair<int, int>> reachable_empty(const vector<vector<int>>& occ) {
  vector<pair<int, int>> cells;
  vector<vector<char>> vis(H, vector<char>(W, 0));
  queue<pair<int, int>> q;
  q.emplace(i0, 0);
  vis[i0][0] = 1;
  while (!q.empty()) {
    auto [r, c] = q.front();
    q.pop();
    if (occ[r][c] == -1)
      cells.emplace_back(r, c);
    static const int dr[4] = {-1, 1, 0, 0};
    static const int dc[4] = {0, 0, -1, 1};
    for (int d = 0; d < 4; ++d) {
      const int nr = r + dr[d], nc = c + dc[d];
      if (vis[nr][nc] || !can_move(r, c, nr, nc))
        continue;
      if (occ[nr][nc] != -1)
        continue;
      vis[nr][nc] = 1;
      q.emplace(nr, nc);
    }
  }
  return cells;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T >> H >> W >> i0;
  h_raw.resize(H - 1);
  v_raw.resize(H);
  for (int i = 0; i < H - 1; ++i)
    cin >> h_raw[i];
  for (int i = 0; i < H; ++i)
    cin >> v_raw[i];
  blocked_h.assign(H - 1, vector<bool>(W, false));
  blocked_v.assign(H, vector<bool>(W - 1, false));
  for (int i = 0; i < H - 1; ++i)
    for (int j = 0; j < W; ++j)
      blocked_h[i][j] = h_raw[i][j] == '1';
  for (int i = 0; i < H; ++i)
    for (int j = 0; j < W - 1; ++j)
      blocked_v[i][j] = v_raw[i][j] == '1';

  cin >> K;
  S.resize(K);
  D.resize(K);
  for (int k = 0; k < K; ++k)
    cin >> S[k] >> D[k];

  vector<vector<int>> occ(H, vector<int>(W, -1));
  vector<tuple<int, int, int, int>> works;

  for (int k = 0; k < K; ++k) {
    auto cells = reachable_empty(occ);
    if (cells.empty())
      continue;
    const auto [r, c] = cells.front();
    occ[r][c] = k;
    works.emplace_back(k + 1, r, c, 1);
  }

  cout << works.size() << '\n';
  for (auto [k, r, c, s] : works)
    cout << k << ' ' << r << ' ' << c << ' ' << s << '\n';
  return 0;
}
