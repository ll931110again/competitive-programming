// Local verifier for AHC011 A (matches tools/src/lib.rs scoring).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int DR[4] = {-1, 1, 0, 0};
constexpr int DC[4] = {0, 0, -1, 1};

int tree_size(int N, const vector<vector<int>> &tiles) {
  const int NN = N * N;
  vector<int> uf(NN), sz(NN, 1);
  vector<char> root(NN, 1), ok(NN, 1);
  iota(uf.begin(), uf.end(), 0);
  auto find = [&](auto &&self, int x) -> int {
    if (uf[x] == x)
      return x;
    return uf[x] = self(self, uf[x]);
  };
  auto unite = [&](int a, int b) {
    a = find(find, a);
    b = find(find, b);
    if (a == b) {
      ok[a] = 0;
      return;
    }
    if (sz[a] < sz[b])
      swap(a, b);
    sz[a] += sz[b];
    uf[b] = a;
    root[b] = 0;
    ok[a] = ok[a] && ok[b];
  };
  auto id = [&](int r, int c) { return r * N + c; };
  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c) {
      if (tiles[r][c] == 0)
        continue;
      if (r + 1 < N && tiles[r + 1][c] && (tiles[r][c] & 8) &&
          (tiles[r + 1][c] & 2))
        unite(id(r, c), id(r + 1, c));
      if (c + 1 < N && tiles[r][c + 1] && (tiles[r][c] & 4) &&
          (tiles[r][c + 1] & 1))
        unite(id(r, c), id(r, c + 1));
    }
  }
  int best = 0;
  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c) {
      if (tiles[r][c] == 0)
        continue;
      const int p = id(r, c);
      const int rt = find(find, p);
      if (root[rt] && ok[rt])
        best = max(best, sz[rt]);
    }
  }
  return best;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, T;
  cin >> N >> T;
  vector<vector<int>> init(N, vector<int>(N));
  for (int r = 0; r < N; ++r) {
    string s;
    cin >> s;
    for (int c = 0; c < N; ++c)
      init[r][c] =
          s[c] <= '9' ? s[c] - '0' : s[c] - 'a' + 10;
  }

  string ops;
  cin >> ops;

  vector<vector<pair<int, int>>> from(N, vector<pair<int, int>>(N));
  for (int r = 0; r < N; ++r)
    for (int c = 0; c < N; ++c)
      from[r][c] = {r, c};

  int er = 0, ec = 0;
  for (int r = 0; r < N; ++r)
    for (int c = 0; c < N; ++c)
      if (init[r][c] == 0)
        er = r, ec = c;

  for (int turn = 0; turn < (int)ops.size(); ++turn) {
    const char ch = ops[turn];
    int d = -1;
    if (ch == 'U')
      d = 0;
    else if (ch == 'D')
      d = 1;
    else if (ch == 'L')
      d = 2;
    else if (ch == 'R')
      d = 3;
    else {
      cout << "invalid move char at turn " << turn << '\n';
      return 1;
    }
    const int nr = er + DR[d], nc = ec + DC[d];
    if (nr < 0 || nr >= N || nc < 0 || nc >= N) {
      cout << "illegal move " << ch << " at turn " << turn << '\n';
      return 1;
    }
    swap(from[er][ec], from[nr][nc]);
    er = nr;
    ec = nc;
  }

  vector<vector<int>> tiles(N, vector<int>(N));
  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c) {
      const auto [orig_r, orig_c] = from[r][c];
      tiles[r][c] = init[orig_r][orig_c];
    }
  }

  if ((int)ops.size() > T) {
    cout << "too many moves K=" << ops.size() << " T=" << T << " score=0\n";
    return 0;
  }

  const int NN = N * N;
  const int S = tree_size(N, tiles);
  long long score;
  if (S == NN - 1) {
    score = llround(500000.0 *
                    (1.0 + (T - (int)ops.size()) * 1.0 / T));
  } else {
    score = llround(500000.0 * S / (NN - 1.0));
  }
  cout << "S=" << S << " K=" << ops.size() << " score=" << score << '\n';
  return 0;
}
