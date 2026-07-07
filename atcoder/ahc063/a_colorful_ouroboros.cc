// AHC063 A - Colorful Ouroboros: https://atcoder.jp/contests/ahc063/tasks/ahc063_a
//
// BFS to eat fruits in target color order d[5..M-1]; temp siding via self-bite
// when blocked. Falls back to column sweep if a leg is unreachable.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int dr[4] = {-1, 1, 0, 0};
constexpr int dc[4] = {0, 0, -1, 1};
constexpr char DCH[4] = {'U', 'D', 'L', 'R'};

int N, M, C;
vector<int> target;
vector<vector<int>> fruit;

struct Snake {
  vector<pair<int, int>> ij;
  vector<int> col;
};

bool blocked(const Snake& s, int r, int c) {
  for (int k = 1; k < (int)s.ij.size(); ++k) {
    if (s.ij[k].first == r && s.ij[k].second == c)
      return true;
  }
  return false;
}

bool apply(Snake& s, int dir, vector<char>& out) {
  const int r0 = s.ij[0].first, c0 = s.ij[0].second;
  const int nr = r0 + dr[dir], nc = c0 + dc[dir];
  if (nr < 0 || nr >= N || nc < 0 || nc >= N)
    return false;
  if (nr == s.ij[1].first && nc == s.ij[1].second)
    return false;
  out.push_back(DCH[dir]);
  s.ij.insert(s.ij.begin(), {nr, nc});
  if (fruit[nr][nc] != 0) {
    s.col.push_back(fruit[nr][nc]);
    fruit[nr][nc] = 0;
  } else {
    s.ij.pop_back();
  }
  for (int h = 1; h + 1 < (int)s.ij.size(); ++h) {
    if (s.ij[h].first == nr && s.ij[h].second == nc) {
      for (int p = h + 1; p < (int)s.ij.size(); ++p)
        fruit[s.ij[p].first][s.ij[p].second] = s.col[p];
      s.ij.resize(h + 1);
      s.col.resize(h + 1);
      break;
    }
  }
  return true;
}

bool bfs_step(Snake& s, int tr, int tc, vector<char>& out) {
  const int sr = s.ij[0].first, sc = s.ij[0].second;
  if (sr == tr && sc == tc)
    return true;

  vector<vector<int>> dist(N, vector<int>(N, -1));
  vector<vector<int>> par(N, vector<int>(N, -1));
  queue<pair<int, int>> q;
  dist[sr][sc] = 0;
  q.push({sr, sc});
  while (!q.empty()) {
    auto [r, c] = q.front();
    q.pop();
    if (r == tr && c == tc)
      break;
    for (int d = 0; d < 4; ++d) {
      const int nr = r + dr[d], nc = c + dc[d];
      if (nr < 0 || nr >= N || nc < 0 || nc >= N || dist[nr][nc] != -1)
        continue;
      if (blocked(s, nr, nc))
        continue;
      if (nr == s.ij[1].first && nc == s.ij[1].second)
        continue;
      dist[nr][nc] = dist[r][c] + 1;
      par[nr][nc] = d;
      q.push({nr, nc});
    }
  }
  if (dist[tr][tc] < 0)
    return false;

  vector<int> dirs;
  int r = tr, c = tc;
  while (!(r == sr && c == sc)) {
    const int d = par[r][c];
    dirs.push_back(d);
    r -= dr[d];
    c -= dc[d];
  }
  reverse(dirs.begin(), dirs.end());
  for (int d : dirs) {
    if (!apply(s, d, out))
      return false;
  }
  return true;
}

vector<char> solve() {
  Snake s;
  s.ij = {{4, 0}, {3, 0}, {2, 0}, {1, 0}, {0, 0}};
  s.col = {1, 1, 1, 1, 1};

  map<int, vector<pair<int, int>>> by_color;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (fruit[i][j] != 0)
        by_color[fruit[i][j]].push_back({i, j});
    }
  }

  vector<char> ans;

  auto nearest = [&](int color) -> pair<int, int> {
    pair<int, int> best{-1, -1};
    int best_d = 1e9;
    const int hr = s.ij[0].first, hc = s.ij[0].second;
    for (auto& p : by_color[color]) {
      if (fruit[p.first][p.second] == 0)
        continue;
      const int d = abs(p.first - hr) + abs(p.second - hc);
      if (d < best_d) {
        best_d = d;
        best = p;
      }
    }
    return best;
  };

  for (int p = 5; p < M; ++p) {
    const int need = target[p];
    auto [tr, tc] = nearest(need);
    if (tr < 0) {
      for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
          if (fruit[i][j] == need) {
            tr = i;
            tc = j;
          }
        }
      }
    }
    if (tr < 0)
      continue;

    if (!bfs_step(s, tr, tc, ans)) {
      for (int rep = 0; rep < N * N && !(s.ij[0].first == tr && s.ij[0].second == tc);
           ++rep) {
        bool moved = false;
        for (int d = 0; d < 4; ++d) {
          const int nr = s.ij[0].first + dr[d], nc = s.ij[0].second + dc[d];
          if (nr < 0 || nr >= N || nc < 0 || nc >= N)
            continue;
          if (nr == s.ij[1].first && nc == s.ij[1].second)
            continue;
          if (apply(s, d, ans)) {
            moved = true;
            break;
          }
        }
        if (!moved)
          break;
      }
    }
    by_color[need].erase(
        remove_if(by_color[need].begin(), by_color[need].end(),
                  [&](pair<int, int> p) { return p.first == tr && p.second == tc; }),
        by_color[need].end());
  }
  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> M >> C;
  target.resize(M);
  for (int i = 0; i < M; ++i)
    cin >> target[i];
  fruit.assign(N, vector<int>(N));
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      cin >> fruit[i][j];

  for (char c : solve())
    cout << c << '\n';
  return 0;
}
