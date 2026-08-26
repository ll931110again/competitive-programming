// Codeforces Spectral::Cup 2026 Round 3 — Connect Connect See
// https://codeforces.com/contest/2245/problem/H
//
// Count connectable pairs via BFS from each non-zero cell (<= 2 turns).
// Complexity: O(k * n * m) per test, sum(n*m) <= 2e6.

#include <bits/stdc++.h>
using namespace std;

namespace {

struct PairHash {
  size_t operator()(uint64_t key) const { return std::hash<uint64_t>{}(key); }
};

uint64_t pack_pair(int a, int b) {
  if (a > b) {
    swap(a, b);
  }
  return (static_cast<uint64_t>(a) << 32) | static_cast<uint32_t>(b);
}

void find_reachable(const vector<vector<int>>& grid,
                    int start_r,
                    int start_c,
                    vector<char>& reachable,
                    vector<int>& seen,
                    int& stamp) {
  const int n = static_cast<int>(grid.size());
  const int m = static_cast<int>(grid[0].size());
  const int value = grid[start_r][start_c];
  const int start_id = start_r * m + start_c;
  const array<int, 4> dr = {-1, 1, 0, 0};
  const array<int, 4> dc = {0, 0, -1, 1};

  fill(reachable.begin(), reachable.end(), 0);
  stamp++;

  auto touch = [&](int index) {
    if (seen[index] != stamp) {
      seen[index] = stamp;
      return true;
    }
    return false;
  };

  queue<array<int, 4>> states;
  for (int dir = 0; dir < 4; dir++) {
    states.push({start_r, start_c, dir, 0});
  }

  while (!states.empty()) {
    const auto [r, c, dir, turns] = states.front();
    states.pop();

    const int ndr = dr[dir];
    const int ndc = dc[dir];
    int nr = r + ndr;
    int nc = c + ndc;
    while (0 <= nr && nr < n && 0 <= nc && nc < m) {
      if (grid[nr][nc] != 0) {
        if (grid[nr][nc] == value) {
          const int id = nr * m + nc;
          if (id != start_id) {
            reachable[id] = 1;
          }
        }
        break;
      }

      const int state_index = ((nr * m + nc) * 4 + dir) * 3 + turns;
      if (!touch(state_index)) {
        nr += ndr;
        nc += ndc;
        continue;
      }

      if (turns < 2) {
        for (int ndir = 0; ndir < 4; ndir++) {
          if (ndir == dir) {
            continue;
          }
          const int turned_index = ((nr * m + nc) * 4 + ndir) * 3 + (turns + 1);
          if (touch(turned_index)) {
            states.push({nr, nc, ndir, turns + 1});
          }
        }
      }

      nr += ndr;
      nc += ndc;
    }
  }
}

int64_t solve_case() {
  int n, m;
  cin >> n >> m;
  vector<vector<int>> grid(n, vector<int>(m));
  vector<pair<int, int>> cells;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      cin >> grid[i][j];
      if (grid[i][j] > 0) {
        cells.push_back({i, j});
      }
    }
  }

  const int total = n * m;
  vector<char> reachable(total);
  vector<int> seen(total * 4 * 3, 0);
  int stamp = 0;
  unordered_set<uint64_t, PairHash> pairs;
  pairs.reserve(cells.size() * 4);

  for (const auto& [r, c] : cells) {
    const int id1 = r * m + c;
    find_reachable(grid, r, c, reachable, seen, stamp);
    for (const auto& [r2, c2] : cells) {
      const int id2 = r2 * m + c2;
      if (id2 <= id1 || grid[r2][c2] != grid[r][c]) {
        continue;
      }
      if (reachable[id2]) {
        pairs.insert(pack_pair(id1, id2));
      }
    }
  }

  return static_cast<int64_t>(pairs.size());
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int tests;
  cin >> tests;
  while (tests--) {
    cout << solve_case() << '\n';
  }
  return 0;
}
