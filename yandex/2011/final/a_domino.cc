// Codeforces 97 (Yandex.Algorithm 2011: Finals) — A. Domino
// Submission: https://codeforces.com/contest/97/submission/373764417

#include <bits/stdc++.h>
using namespace std;

using int64 = long long;

namespace {

int domino_index(int a, int b) {
  if (a > b)
    swap(a, b);
  int idx = 0;
  for (int i = 0; i < a; i++)
    idx += (7 - i);
  idx += (b - a);
  return idx;
}

void enumerate_tilings(int n, int m, const vector<string>& grid, vector<vector<int>>& cell_region,
                       vector<array<int, 2>>& squares_stack,
                       vector<vector<array<int, 2>>>& out_squares) {
  int si = -1, sj = -1;
  for (int i = 0; i < n && si < 0; i++) {
    for (int j = 0; j < m && si < 0; j++) {
      if (grid[i][j] != '.' && cell_region[i][j] < 0) {
        si = i;
        sj = j;
      }
    }
  }
  if (si < 0) {
    out_squares.push_back(squares_stack);
    return;
  }

  auto can_place = [&](int ti, int tj) -> bool {
    if (ti + 1 >= n || tj + 1 >= m)
      return false;
    for (int di = 0; di < 2; di++) {
      for (int dj = 0; dj < 2; dj++) {
        int i = ti + di, j = tj + dj;
        if (grid[i][j] == '.' || cell_region[i][j] >= 0)
          return false;
      }
    }
    return true;
  };

  // Try every 2x2 whose top-left is one of the four corners covering (si,sj).
  for (int ti : {si - 1, si}) {
    for (int tj : {sj - 1, sj}) {
      if (ti < 0 || tj < 0)
        continue;
      if (!can_place(ti, tj))
        continue;

      int sid = (int)squares_stack.size();
      for (int di = 0; di < 2; di++) {
        for (int dj = 0; dj < 2; dj++) {
          cell_region[ti + di][tj + dj] = sid;
        }
      }
      squares_stack.push_back({ti, tj});
      enumerate_tilings(n, m, grid, cell_region, squares_stack, out_squares);
      squares_stack.pop_back();
      for (int di = 0; di < 2; di++) {
        for (int dj = 0; dj < 2; dj++) {
          cell_region[ti + di][tj + dj] = -1;
        }
      }
    }
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m;
  cin >> n >> m;
  vector<string> grid(n);
  for (int i = 0; i < n; i++)
    cin >> grid[i];

  map<char, vector<pair<int, int>>> chips_cells;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      char ch = grid[i][j];
      if (ch != '.')
        chips_cells[ch].push_back({i, j});
    }
  }

  array<array<int, 7>, 7> idx7{};
  for (int a = 0; a < 7; a++) {
    for (int b = 0; b < 7; b++) {
      idx7[a][b] = domino_index(a, b);
    }
  }

  vector<vector<int>> cell_region(n, vector<int>(m, -1));
  vector<array<int, 2>> stack;
  vector<vector<array<int, 2>>> tilings;
  enumerate_tilings(n, m, grid, cell_region, stack, tilings);

  for (auto& t : tilings)
    sort(t.begin(), t.end());
  sort(tilings.begin(), tilings.end());
  tilings.erase(unique(tilings.begin(), tilings.end()), tilings.end());

  constexpr uint32_t FULL_DOMINO_MASK = (1u << 28) - 1;

  int64 total_ways = 0;
  vector<string> answer_grid;

  for (const auto& squares_top_left : tilings) {
    int R = (int)squares_top_left.size();

    vector<vector<int>> reg(n, vector<int>(m, -1));
    for (int s = 0; s < R; s++) {
      int ti = squares_top_left[s][0], tj = squares_top_left[s][1];
      for (int di = 0; di < 2; di++) {
        for (int dj = 0; dj < 2; dj++) {
          reg[ti + di][tj + dj] = s;
        }
      }
    }

    vector<pair<int, int>> chips;
    chips.reserve(chips_cells.size());
    for (const auto& kv : chips_cells) {
      const auto& cells = kv.second;
      assert((int)cells.size() == 2);
      int r0 = reg[cells[0].first][cells[0].second];
      int r1 = reg[cells[1].first][cells[1].second];
      int a = min(r0, r1), b = max(r0, r1);
      chips.push_back({a, b});
    }

    vector<vector<int>> chips_at_max(R);
    for (int i = 0; i < (int)chips.size(); i++) {
      int rv = chips[i].second;
      chips_at_max[rv].push_back(i);
    }

    array<int, 7> cnt{};
    vector<int> assign(R, -1);
    int64 ways = 0;

    auto dfs = [&](auto&& self, int k, uint32_t used_mask) -> void {
      if (k == R) {
        if (used_mask == FULL_DOMINO_MASK) {
          ways++;
          if (answer_grid.empty()) {
            answer_grid.assign(n, string(m, '.'));
            for (int i = 0; i < n; i++) {
              for (int j = 0; j < m; j++) {
                if (grid[i][j] != '.') {
                  int rr = reg[i][j];
                  answer_grid[i][j] = char('0' + assign[rr]);
                }
              }
            }
          }
        }
        return;
      }

      for (int d = 0; d <= 6; d++) {
        if (cnt[d] >= 2)
          continue;
        cnt[d]++;
        assign[k] = d;

        uint32_t mask = used_mask;
        bool bad = false;

        for (int cid : chips_at_max[k]) {
          int ru = chips[cid].first;
          int idx = idx7[assign[ru]][d];
          if (mask & (1u << idx)) {
            bad = true;
            break;
          }
          mask |= (1u << idx);
        }

        if (!bad)
          self(self, k + 1, mask);

        cnt[d]--;
        assign[k] = -1;
      }
    };

    dfs(dfs, 0, 0u);
    total_ways += ways;
  }

  cout << total_ways << "\n";
  for (const auto& row : answer_grid)
    cout << row << "\n";

  return 0;
}
