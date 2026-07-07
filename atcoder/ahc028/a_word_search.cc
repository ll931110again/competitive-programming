// AHC028 A - Word Search: https://atcoder.jp/contests/ahc028/tasks/ahc028_a
//
// Spell every target word by walking the grid (Manhattan +1 per step). BFS each
// word onto the current collected string; skip words already present.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

int n, m;
int cr, cc;
vector<string> grid;
vector<string> words;
vector<pair<int, int>> path;

bool can_move(int r, int c, int d) {
  static const int dr[4] = {-1, 0, 1, 0};
  static const int dc[4] = {0, 1, 0, -1};
  const int nr = r + dr[d], nc = c + dc[d];
  return nr >= 0 && nr < n && nc >= 0 && nc < n;
}

void step(int d) {
  static const int dr[4] = {-1, 0, 1, 0};
  static const int dc[4] = {0, 1, 0, -1};
  cr += dr[d];
  cc += dc[d];
  path.emplace_back(cr, cc);
}

bool append_word(const string& w, string& s) {
  if (s.find(w) != string::npos)
    return true;

  struct Node {
    int r, c, k;
    int pr, pc, pk, pd;
  };
  queue<Node> q;
  map<tuple<int, int, int>, tuple<int, int, int, int>> par;
  q.push({cr, cc, 0, -1, -1, -1, -1});
  par[{cr, cc, 0}] = {-1, -1, -1, -1};

  while (!q.empty()) {
    Node cur = q.front();
    q.pop();
    if (cur.k == (int)w.size()) {
      vector<int> dirs;
      for (int k = cur.k, r = cur.r, c = cur.c; k > 0;) {
        auto key = make_tuple(r, c, k);
        auto [pr, pc, pk, pd] = par[key];
        dirs.push_back(pd);
        tie(r, c, k) = tie(pr, pc, pk);
      }
      reverse(dirs.begin(), dirs.end());
      for (int d : dirs)
        step(d);
      s += w;
      return true;
    }
    for (int d = 0; d < 4; ++d) {
      if (!can_move(cur.r, cur.c, d))
        continue;
      static const int dr[4] = {-1, 0, 1, 0};
      static const int dc[4] = {0, 1, 0, -1};
      const int nr = cur.r + dr[d], nc = cur.c + dc[d];
      const char ch = grid[nr][nc];
      int nk = cur.k;
      if (ch == w[cur.k])
        ++nk;
      else if (ch == w[0])
        nk = 1;
      else
        nk = 0;
      auto key = make_tuple(nr, nc, nk);
      if (par.count(key))
        continue;
      par[key] = {cur.r, cur.c, cur.k, d};
      q.push({nr, nc, nk, cur.r, cur.c, cur.k, d});
    }
  }
  return false;
}

void solve() {
  string collected;
  for (const string& w : words)
    append_word(w, collected);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  cin >> cr >> cc;
  grid.resize(n);
  for (int i = 0; i < n; ++i) {
    string row;
    cin >> row;
    grid[i] = row;
  }
  words.resize(m);
  for (int i = 0; i < m; ++i)
    cin >> words[i];

  solve();

  for (auto [r, c] : path)
    cout << r << ' ' << c << '\n';
  return 0;
}
