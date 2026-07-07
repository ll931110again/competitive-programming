// AHC033 A - Container Handling with Cranes: https://atcoder.jp/contests/ahc033/tasks/ahc033_a
//
// Joint simulation: each row's crane shuttles containers right when the lane is
// clear; the large crane (row 0) may pass over other containers.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int MAXT = 10000;

int N;
int A[5][5];
int board[5][5];
int pos_r[5], pos_c[5];
bool alive[5];
bool hold[5];
int hold_id[5];
int done_cnt[5];
int next_in[5];
string moves[5];

bool inside(int r, int c) { return 0 <= r && r < N && 0 <= c && c < N; }

bool can_move(int i, int dr, int dc) {
  if (!alive[i])
    return false;
  const int nr = pos_r[i] + dr, nc = pos_c[i] + dc;
  if (!inside(nr, nc))
    return false;
  for (int j = 0; j < N; ++j) {
    if (!alive[j] || j == i)
      continue;
    if (pos_r[j] == nr && pos_c[j] == nc)
      return false;
    if (pos_r[j] == nr && pos_c[j] == nc && pos_r[i] == pos_r[j] && pos_c[i] == pos_c[j])
      return false;
  }
  if (i > 0 && hold[i] && board[nr][nc] != -1)
    return false;
  return true;
}

void feed() {
  for (int i = 0; i < N; ++i) {
    if (next_in[i] >= N || board[i][0] != -1)
      continue;
    bool blocked = false;
    for (int k = 0; k < N; ++k)
      if (alive[k] && pos_r[k] == i && pos_c[k] == 0 && hold[k])
        blocked = true;
    if (!blocked)
      board[i][0] = A[i][next_in[i]++];
  }
}

void dispatch() {
  for (int i = 0; i < N; ++i)
    if (board[i][N - 1] != -1)
      board[i][N - 1] = -1, ++done_cnt[i];
}

void apply(int i, char mv) {
  if (!alive[i]) {
    moves[i].push_back('.');
    return;
  }
  auto fail = [&] { moves[i].push_back('.'); };
  if (mv == '.') {
    moves[i].push_back('.');
    return;
  }
  if (mv == 'P') {
    if (!hold[i] && board[pos_r[i]][pos_c[i]] != -1) {
      hold[i] = true;
      hold_id[i] = board[pos_r[i]][pos_c[i]];
      board[pos_r[i]][pos_c[i]] = -1;
      moves[i].push_back('P');
    } else
      fail();
    return;
  }
  if (mv == 'Q') {
    if (hold[i] && board[pos_r[i]][pos_c[i]] == -1) {
      board[pos_r[i]][pos_c[i]] = hold_id[i];
      hold[i] = false;
      hold_id[i] = -1;
      moves[i].push_back('Q');
    } else
      fail();
    return;
  }
  int dr = 0, dc = 0;
  if (mv == 'U')
    dr = -1;
  else if (mv == 'D')
    dr = 1;
  else if (mv == 'L')
    dc = -1;
  else if (mv == 'R')
    dc = 1;
  else if (mv == 'B') {
    if (!hold[i]) {
      alive[i] = false;
      moves[i].push_back('B');
    } else
      fail();
    return;
  } else {
    fail();
    return;
  }
  if (can_move(i, dr, dc)) {
    pos_r[i] += dr;
    pos_c[i] += dc;
    moves[i].push_back(mv);
  } else
    fail();
}

int target(int row) { return row * N + done_cnt[row]; }

char plan(int i) {
  if (!alive[i])
    return '.';
  const int row = i;
  const int tgt = target(row);

  if (hold[i]) {
    if (pos_c[i] < N - 1 && can_move(i, 0, 1))
      return 'R';
    if (pos_c[i] > 0 && can_move(i, 0, -1))
      return 'L';
    return '.';
  }

  if (board[row][pos_c[i]] != -1) {
    if (board[row][pos_c[i]] == tgt)
      return 'P';
    if (pos_c[i] < N - 1 && can_move(i, 0, 1))
      return 'R';
    return '.';
  }

  if (pos_c[i] > 0 && can_move(i, 0, -1))
    return 'L';
  return '.';
}

void turn(const vector<char>& mv) {
  feed();
  for (int i = 0; i < N; ++i)
    apply(i, mv[i]);
  dispatch();
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N;
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      cin >> A[i][j];

  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      board[i][j] = -1;
  for (int i = 0; i < N; ++i) {
    pos_r[i] = i;
    pos_c[i] = 0;
    alive[i] = true;
    hold[i] = false;
    hold_id[i] = -1;
    done_cnt[i] = next_in[i] = 0;
  }

  int dispatched = 0;
  for (int step = 0; step < MAXT && dispatched < N * N; ++step) {
    vector<char> mv(N, '.');
    for (int i = 0; i < N; ++i)
      mv[i] = plan(i);
    turn(mv);
    dispatched = accumulate(done_cnt, done_cnt + N, 0);
  }

  int T = 1;
  for (int i = 0; i < N; ++i)
    T = max(T, (int)moves[i].size());
  for (int i = 0; i < N; ++i) {
    while ((int)moves[i].size() < T)
      moves[i].push_back('.');
    cout << moves[i] << '\n';
  }
  return 0;
}
