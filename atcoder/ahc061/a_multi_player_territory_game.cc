// AHC061 A - Multi-Player Territory Game:
// https://atcoder.jp/contests/ahc061/tasks/ahc061_a
//
// Interactive expansion with one-step joint-turn simulation: score each candidate
// move after greedy AI responses and collision rules from the official judge.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int dr[4] = {-1, 1, 0, 0};
constexpr int dc[4] = {0, 0, -1, 1};

int N, M, T, U;
vector<vector<int>> V;

struct GameState {
  vector<pair<int, int>> pos;
  vector<vector<int>> owner;
  vector<vector<int>> level;

  GameState clone() const {
    return *this;
  }
};

i64 territory_score(const GameState& st, int player) {
  i64 s = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (st.owner[i][j] == player)
        s += 1LL * V[i][j] * st.level[i][j];
    }
  }
  return s;
}

vector<pair<int, int>> candidates(const GameState& st, int player) {
  vector<pair<int, int>> reach;
  vector<vector<char>> vis(N, vector<char>(N, 0));
  queue<pair<int, int>> q;
  const auto [sr, sc] = st.pos[player];
  q.push({sr, sc});
  vis[sr][sc] = 1;
  while (!q.empty()) {
    auto [x, y] = q.front();
    q.pop();
    bool ok = true;
    for (int i = 0; i < M; ++i) {
      if (i != player && st.pos[i].first == x && st.pos[i].second == y) {
        ok = false;
        break;
      }
    }
    if (ok)
      reach.push_back({x, y});
    if (st.owner[x][y] == player) {
      for (int d = 0; d < 4; ++d) {
        const int nx = x + dr[d], ny = y + dc[d];
        if (nx < 0 || nx >= N || ny < 0 || ny >= N || vis[nx][ny])
          continue;
        vis[nx][ny] = 1;
        q.push({nx, ny});
      }
    }
  }
  return reach;
}

double eval_cell(const GameState& st, int player, int x, int y) {
  const int o = st.owner[x][y];
  const int lv = st.level[x][y];
  const double val = V[x][y];
  if (o == -1)
    return val * 1.0;
  if (o == player) {
    if (lv < U)
      return val * 0.85;
    return 0.0;
  }
  if (lv == 1)
    return val * 1.05;
  return val * 0.35;
}

pair<int, int> greedy_move(const GameState& st, int player) {
  const auto cand = candidates(st, player);
  if (cand.empty())
    return st.pos[player];

  double best = -1;
  vector<pair<int, int>> best_cells;
  for (const auto [x, y] : cand) {
    double sc = eval_cell(st, player, x, y);
    if (st.owner[x][y] == -1)
      sc += V[x][y] * 0.05;
    if (st.owner[x][y] >= 0 && st.owner[x][y] != player && st.level[x][y] == 1)
      sc += V[x][y] * 0.1;
    if (sc > best + 1e-9) {
      best = sc;
      best_cells = {{x, y}};
    } else if (abs(sc - best) <= 1e-9) {
      best_cells.push_back({x, y});
    }
  }

  if (best_cells.empty())
    return st.pos[player];
  if (best_cells.size() == 1)
    return best_cells[0];

  pair<int, int> pick = best_cells[0];
  i64 best_proj = -1;
  for (const auto [x, y] : best_cells) {
    const i64 proj = 1LL * V[x][y] * (st.owner[x][y] == player ? st.level[x][y] + 1 : 1);
    if (proj > best_proj) {
      best_proj = proj;
      pick = {x, y};
    }
  }
  return pick;
}

bool apply_turn(GameState& st, const vector<pair<int, int>>& moves) {
  vector<pair<int, int>> temp = moves;
  map<pair<int, int>, int> cnt;
  for (const auto& m : moves)
    cnt[m]++;

  vector<char> collected(M, 0);
  for (int i = 0; i < M; ++i) {
    if (cnt[moves[i]] >= 2) {
      const int o = st.owner[moves[i].first][moves[i].second];
      if (i != o)
        collected[i] = 1;
    }
  }

  for (int i = 0; i < M; ++i) {
    if (collected[i])
      continue;
    const auto [x, y] = temp[i];
    const int o = st.owner[x][y];
    if (o == -1) {
      st.owner[x][y] = i;
      st.level[x][y] = 1;
    } else if (o == i) {
      if (st.level[x][y] < U)
        st.level[x][y]++;
    } else {
      st.level[x][y]--;
      if (st.level[x][y] == 0) {
        st.owner[x][y] = i;
        st.level[x][y] = 1;
      } else {
        collected[i] = 1;
      }
    }
  }

  for (int i = 0; i < M; ++i) {
    if (!collected[i])
      st.pos[i] = temp[i];
  }
  return true;
}

pair<int, int> choose_move(const GameState& st) {
  const auto cand = candidates(st, 0);
  if (cand.empty())
    return st.pos[0];

  vector<pair<double, pair<int, int>>> ranked;
  ranked.reserve(cand.size());
  for (const auto [x, y] : cand) {
    double sc = eval_cell(st, 0, x, y);
    if (st.owner[x][y] == -1)
      sc += V[x][y] * 0.05;
    if (st.owner[x][y] >= 0 && st.owner[x][y] != 0 && st.level[x][y] == 1)
      sc += V[x][y] * 0.1;
    ranked.push_back({sc, {x, y}});
  }
  sort(ranked.begin(), ranked.end(), [](const auto& a, const auto& b) {
    if (a.first != b.first)
      return a.first > b.first;
    return a.second < b.second;
  });

  const int k = min<int>(ranked.size(), 12);
  i64 best_score = LLONG_MIN;
  pair<int, int> best = ranked[0].second;

  vector<pair<int, int>> ai_moves(M - 1);
  for (int p = 1; p < M; ++p)
    ai_moves[p - 1] = greedy_move(st, p);

  for (int i = 0; i < k; ++i) {
    const auto [x, y] = ranked[i].second;
    GameState trial = st;
    vector<pair<int, int>> moves(M);
    moves[0] = {x, y};
    for (int p = 1; p < M; ++p)
      moves[p] = ai_moves[p - 1];
    apply_turn(trial, moves);
    const i64 sc = territory_score(trial, 0);
    if (sc > best_score) {
      best_score = sc;
      best = {x, y};
    }
  }
  return best;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> M >> T >> U;
  V.assign(N, vector<int>(N));
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      cin >> V[i][j];

  GameState st;
  st.pos.assign(M, {-1, -1});
  for (int p = 0; p < M; ++p)
    cin >> st.pos[p].first >> st.pos[p].second;

  st.owner.assign(N, vector<int>(N, -1));
  st.level.assign(N, vector<int>(N, 0));
  for (int p = 0; p < M; ++p) {
    st.owner[st.pos[p].first][st.pos[p].second] = p;
    st.level[st.pos[p].first][st.pos[p].second] = 1;
  }

  for (int turn = 0; turn < T; ++turn) {
    const auto [x, y] = choose_move(st);
    cout << x << ' ' << y << '\n' << flush;

    GameState next = st;
    for (int p = 0; p < M; ++p) {
      int sx, sy;
      cin >> sx >> sy;
    }
    next.pos.assign(M, {-1, -1});
    for (int p = 0; p < M; ++p)
      cin >> next.pos[p].first >> next.pos[p].second;

    next.owner.assign(N, vector<int>(N));
    next.level.assign(N, vector<int>(N));
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        cin >> next.owner[i][j];
    for (int i = 0; i < N; ++i)
      for (int j = 0; j < N; ++j)
        cin >> next.level[i][j];
    st = next;
  }
  return 0;
}
