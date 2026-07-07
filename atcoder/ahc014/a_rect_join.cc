// AHC014 A - RectJoin: https://atcoder.jp/contests/ahc014/tasks/ahc014_a
//
// Greedy rectangle placement: enumerate legal moves from each dot, prefer high-weight
// new corners and small rectangles; chain 1x1 placements after each pick.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int DXY[8][2] = {
    {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};
constexpr int PERP[8] = {2, 3, 4, 5, 6, 7, 0, 1};

struct Move {
  int p[4][2];
  int w;
  int area2;
};

int N, M, C;
vector<vector<bool>> has_point;
vector<vector<array<bool, 8>>> used;
vector<Move> ops;
vector<pair<int, int>> init;
mt19937 rng(42);

int weight(int x, int y) {
  const int dx = x - C;
  const int dy = y - C;
  return dx * dx + dy * dy + 1;
}

bool inside(int x, int y) { return 0 <= x && x < N && 0 <= y && y < N; }

int dir_id(int dx, int dy) {
  for (int d = 0; d < 8; ++d)
    if (DXY[d][0] == dx && DXY[d][1] == dy)
      return d;
  return -1;
}

bool check_rect(const array<pair<int, int>, 4>& rect) {
  if (has_point[rect[0].first][rect[0].second])
    return false;
  for (int i = 1; i < 4; ++i) {
    if (!has_point[rect[i].first][rect[i].second])
      return false;
  }

  const int dx01 = rect[1].first - rect[0].first;
  const int dy01 = rect[1].second - rect[0].second;
  const int dx03 = rect[3].first - rect[0].first;
  const int dy03 = rect[3].second - rect[0].second;
  if (dx01 * dx03 + dy01 * dy03 != 0)
    return false;
  if (dx01 != 0 && dy01 != 0 && abs(dx01) != abs(dy01))
    return false;
  if (rect[1].first + dx03 != rect[2].first || rect[1].second + dy03 != rect[2].second)
    return false;

  for (int i = 0; i < 4; ++i) {
    int x = rect[i].first, y = rect[i].second;
    const int tx = rect[(i + 1) % 4].first;
    const int ty = rect[(i + 1) % 4].second;
    const int dx = (tx - x) == 0 ? 0 : (tx - x) / abs(tx - x);
    const int dy = (ty - y) == 0 ? 0 : (ty - y) / abs(ty - y);
    const int d = dir_id(dx, dy);
    while (x != tx || y != ty) {
      if (!(x == rect[i].first && y == rect[i].second) && has_point[x][y])
        return false;
      if (used[x][y][d])
        return false;
      x += dx;
      y += dy;
      if (used[x][y][d ^ 4])
        return false;
    }
  }
  return true;
}

void apply_rect(const array<pair<int, int>, 4>& rect) {
  has_point[rect[0].first][rect[0].second] = true;
  for (int i = 0; i < 4; ++i) {
    int x = rect[i].first, y = rect[i].second;
    const int tx = rect[(i + 1) % 4].first;
    const int ty = rect[(i + 1) % 4].second;
    const int dx = (tx - x) == 0 ? 0 : (tx - x) / abs(tx - x);
    const int dy = (ty - y) == 0 ? 0 : (ty - y) / abs(ty - y);
    const int d = dir_id(dx, dy);
    while (x != tx || y != ty) {
      used[x][y][d] = true;
      x += dx;
      y += dy;
      used[x][y][d ^ 4] = true;
    }
  }
}

optional<pair<int, int>> first_dot(int x, int y, int d) {
  x += DXY[d][0];
  y += DXY[d][1];
  while (inside(x, y)) {
    if (has_point[x][y])
      return pair<int, int>{x, y};
    x += DXY[d][0];
    y += DXY[d][1];
  }
  return nullopt;
}

int rect_area2(const array<pair<int, int>, 4>& rect) {
  const int dx = abs(rect[1].first - rect[0].first);
  const int dy = abs(rect[1].second - rect[0].second);
  if (dx == 0 || dy == 0)
    return max(dx, dy);
  return dx * dy * 2;
}

bool try_move(const array<pair<int, int>, 4>& rect) {
  if (!check_rect(rect))
    return false;
  Move mv{};
  for (int i = 0; i < 4; ++i) {
    mv.p[i][0] = rect[i].first;
    mv.p[i][1] = rect[i].second;
  }
  mv.w = weight(rect[0].first, rect[0].second);
  mv.area2 = rect_area2(rect);
  ops.push_back(mv);
  apply_rect(rect);
  return true;
}

void enumerate_from(int px, int py, vector<Move>& cand) {
  for (int d = 0; d < 8; ++d) {
    for (int pd : {PERP[d], PERP[d] ^ 4}) {
      auto p2 = first_dot(px, py, d);
      auto p4 = first_dot(px, py, pd);
      if (!p2 || !p4)
        continue;
      const int x1 = p2->first + p4->first - px;
      const int y1 = p2->second + p4->second - py;
      if (!inside(x1, y1) || has_point[x1][y1])
        continue;
      array<pair<int, int>, 4> rect = {
          pair<int, int>{x1, y1}, *p2, pair<int, int>{px, py}, *p4};
      if (!check_rect(rect))
        continue;
      Move mv{};
      for (int i = 0; i < 4; ++i) {
        mv.p[i][0] = rect[i].first;
        mv.p[i][1] = rect[i].second;
      }
      mv.w = weight(x1, y1);
      mv.area2 = rect_area2(rect);
      cand.push_back(mv);
    }
  }
}

bool apply_unit_squares() {
  bool any = false;
  for (int x = 0; x + 1 < N; ++x) {
    for (int y = 0; y + 1 < N; ++y) {
      const array<pair<int, int>, 4> rects[4] = {
          {{{x, y}, {x + 1, y}, {x + 1, y + 1}, {x, y + 1}}},
          {{{x + 1, y}, {x + 1, y + 1}, {x, y + 1}, {x, y}}},
          {{{x, y + 1}, {x, y}, {x + 1, y}, {x + 1, y + 1}}},
          {{{x + 1, y + 1}, {x, y + 1}, {x, y}, {x + 1, y}}},
      };
      for (const auto& rect : rects)
        any |= try_move(rect);
    }
  }
  for (int x = 1; x + 1 < N; ++x) {
    for (int y = 0; y + 2 < N; ++y) {
      const array<pair<int, int>, 4> rects[4] = {
          {{{x - 1, y + 1}, {x, y}, {x + 1, y + 1}, {x, y + 2}}},
          {{{x, y}, {x + 1, y + 1}, {x, y + 2}, {x - 1, y + 1}}},
          {{{x + 1, y + 1}, {x, y + 2}, {x - 1, y + 1}, {x, y}}},
          {{{x, y + 2}, {x - 1, y + 1}, {x, y}, {x + 1, y + 1}}},
      };
      for (const auto& rect : rects)
        any |= try_move(rect);
    }
  }
  return any;
}

void chain_ok_rects() {
  while (apply_unit_squares()) {
  }
}

void reset_state() {
  has_point.assign(N, vector<bool>(N, false));
  used.assign(N, vector<array<bool, 8>>(N, array<bool, 8>{}));
  for (auto [x, y] : init)
    has_point[x][y] = true;
  ops.clear();
}

int current_score() {
  int num = 0;
  for (auto [x, y] : init)
    num += weight(x, y);
  for (const auto& mv : ops)
    num += mv.w;
  int den = 0;
  for (int x = 0; x < N; ++x)
    for (int y = 0; y < N; ++y)
      den += weight(x, y);
  return llround(1e6 * (1.0 * N * N / M) * (1.0 * num / den));
}

bool pick_best(double w_mul, int area_pen) {
  vector<pair<int, int>> dots;
  for (int x = 0; x < N; ++x)
    for (int y = 0; y < N; ++y)
      if (has_point[x][y])
        dots.emplace_back(x, y);

  vector<Move> cand;
  for (auto [x, y] : dots)
    enumerate_from(x, y, cand);
  if (cand.empty())
    return false;

  auto score = [&](const Move& mv) {
    return w_mul * mv.w - area_pen * mv.area2 + (rng() % 5);
  };

  int best = 0;
  for (int i = 1; i < (int)cand.size(); ++i)
    if (score(cand[i]) > score(cand[best]))
      best = i;

  array<pair<int, int>, 4> rect;
  for (int i = 0; i < 4; ++i)
    rect[i] = {cand[best].p[i][0], cand[best].p[i][1]};
  try_move(rect);
  chain_ok_rects();
  return true;
}

void solve(double w_mul, int area_pen) {
  while (pick_best(w_mul, area_pen)) {
  }
}

}  // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> M;
  C = N / 2;
  init.reserve(M);
  for (int i = 0; i < M; ++i) {
    int x, y;
    cin >> x >> y;
    init.emplace_back(x, y);
  }

  const auto t0 = chrono::steady_clock::now();
  vector<Move> best_ops;
  int best_score = -1;

  for (int trial = 0; trial < 300; ++trial) {
    const auto now = chrono::steady_clock::now();
    if (chrono::duration<double>(now - t0).count() > 4.6)
      break;

    rng.seed(trial * 9973 + 42);
    reset_state();
    const double w_mul = 1.0 + 0.03 * (trial % 11);
    const int area_pen = trial % 4;
    solve(w_mul, area_pen);

    const int score = current_score();
    if (score > best_score) {
      best_score = score;
      best_ops = ops;
    }
  }

  cout << best_ops.size() << '\n';
  for (const auto& mv : best_ops) {
    for (int i = 0; i < 4; ++i)
      cout << mv.p[i][0] << ' ' << mv.p[i][1] << (i + 1 == 4 ? '\n' : ' ');
  }
  return 0;
}
