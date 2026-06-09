// AtCoder yuha_c83 A — Revenge of Voronoi (離散ボロノイの逆襲)
// https://atcoder.jp/contests/yuha_c83/tasks/yuha_c83_01
//
// Recover Manhattan-Voronoi seed coordinates from a discrete pixel diagram.
// Tie-break: smallest letter wins on equal distance.

#include <bits/stdc++.h>
using namespace std;

namespace {

int manhattan(int x1, int y1, int x2, int y2) {
  return abs(x1 - x2) + abs(y1 - y2);
}

char winner(int px, int py, const map<char, pair<int, int>>& pos) {
  int best = INT_MAX;
  char win = 0;
  for (const auto& [ch, p] : pos) {
    int d = manhattan(px, py, p.first, p.second);
    if (d < best || (d == best && ch < win)) {
      best = d;
      win = ch;
    }
  }
  return win;
}

bool matches(const vector<string>& grid, const map<char, pair<int, int>>& pos) {
  for (int y = 0; y < (int)grid.size(); ++y) {
    for (int x = 0; x < (int)grid[y].size(); ++x) {
      if (winner(x, y, pos) != grid[y][x]) {
        return false;
      }
    }
  }
  return true;
}

bool exists_center(const vector<pair<int, int>>& pixels, int r, int xl, int xr, int yl, int yr) {
  for (int x = xl; x <= xr; ++x) {
    for (int y = yl; y <= yr; ++y) {
      bool ok = true;
      for (const auto& [px, py] : pixels) {
        if (manhattan(x, y, px, py) > r) {
          ok = false;
          break;
        }
      }
      if (ok) {
        return true;
      }
    }
  }
  return false;
}

vector<pair<int, int>> candidates_for(const vector<pair<int, int>>& pixels, int pad) {
  int minx = INT_MAX, maxx = INT_MIN, miny = INT_MAX, maxy = INT_MIN;
  for (const auto& [px, py] : pixels) {
    minx = min(minx, px);
    maxx = max(maxx, px);
    miny = min(miny, py);
    maxy = max(maxy, py);
  }

  int lo = 0, hi = maxx - minx + maxy - miny + pad;
  while (lo < hi) {
    int mid = (lo + hi) / 2;
    int xl = maxx - mid, xr = minx + mid, yl = maxy - mid, yr = miny + mid;
    if (exists_center(pixels, mid, xl, xr, yl, yr)) {
      hi = mid;
    } else {
      lo = mid + 1;
    }
  }

  int r = lo;
  int xl = maxx - r, xr = minx + r, yl = maxy - r, yr = miny + r;
  xl -= 1;
  xr += 1;
  yl -= 1;
  yr += 1;

  vector<pair<int, int>> out;
  for (int x = xl; x <= xr; ++x) {
    for (int y = yl; y <= yr; ++y) {
      int mx = 0;
      for (const auto& [px, py] : pixels) {
        mx = max(mx, manhattan(x, y, px, py));
      }
      if (mx <= r + 1) {
        out.push_back({x, y});
      }
    }
  }
  return out;
}

bool dfs(int idx, const vector<char>& letters, const vector<vector<pair<int, int>>>& cands,
         const vector<string>& grid, map<char, pair<int, int>>& pos, set<pair<int, int>>& used) {
  if (idx == (int)letters.size()) {
    return matches(grid, pos);
  }
  char ch = letters[idx];
  for (const auto& p : cands[idx]) {
    if (used.count(p)) {
      continue;
    }
    pos[ch] = p;
    used.insert(p);
    if (dfs(idx + 1, letters, cands, grid, pos, used)) {
      return true;
    }
    used.erase(p);
  }
  pos.erase(ch);
  return false;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int w, h;
  cin >> w >> h;
  vector<string> grid(h);
  for (int i = 0; i < h; ++i) {
    cin >> grid[i];
  }

  map<char, vector<pair<int, int>>> cells;
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      cells[grid[y][x]].push_back({x, y});
    }
  }

  vector<char> letters;
  for (const auto& [ch, _] : cells) {
    letters.push_back(ch);
  }

  int pad = max(w, h);
  vector<vector<pair<int, int>>> cands;
  for (char ch : letters) {
    cands.push_back(candidates_for(cells[ch], pad));
  }

  vector<int> order(letters.size());
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(), [&](int a, int b) { return cands[a].size() < cands[b].size(); });

  vector<char> ordered_letters;
  vector<vector<pair<int, int>>> ordered_cands;
  for (int i : order) {
    ordered_letters.push_back(letters[i]);
    ordered_cands.push_back(cands[i]);
  }

  map<char, pair<int, int>> pos;
  set<pair<int, int>> used;
  if (!dfs(0, ordered_letters, ordered_cands, grid, pos, used)) {
    ordered_cands.clear();
    for (char ch : ordered_letters) {
      ordered_cands.push_back(candidates_for(cells[ch], 3 * pad));
    }
    if (!dfs(0, ordered_letters, ordered_cands, grid, pos, used)) {
      return 0;
    }
  }

  sort(letters.begin(), letters.end());
  for (char ch : letters) {
    auto p = pos.at(ch);
    cout << ch << ' ' << p.first << ' ' << p.second << '\n';
  }
  return 0;
}
