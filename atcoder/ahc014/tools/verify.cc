// Local scorer for AHC014 RectJoin (mirrors tools/src/lib.rs).

#include <bits/stdc++.h>
using namespace std;

constexpr int DXY[8][2] = {
    {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1}, {0, -1}, {1, -1}};

struct Input {
  int N;
  vector<pair<int, int>> ps;
};

struct State {
  int N;
  vector<vector<bool>> has_point;
  vector<vector<array<bool, 8>>> used;

  explicit State(const Input& in) : N(in.N) {
    has_point.assign(N, vector<bool>(N, false));
    used.assign(N, vector<array<bool, 8>>(N, array<bool, 8>{}));
    for (auto [x, y] : in.ps)
      has_point[x][y] = true;
  }

  int dir_id(int dx, int dy) const {
    for (int d = 0; d < 8; ++d)
      if (DXY[d][0] == dx && DXY[d][1] == dy)
        return d;
    return -1;
  }

  string check_move(const array<pair<int, int>, 4>& rect) const {
    for (int i = 1; i < 4; ++i) {
      if (!has_point[rect[i].first][rect[i].second])
        return "missing dot";
    }
    if (has_point[rect[0].first][rect[0].second])
      return "p1 has dot";

    const int dx01 = rect[1].first - rect[0].first;
    const int dy01 = rect[1].second - rect[0].second;
    const int dx03 = rect[3].first - rect[0].first;
    const int dy03 = rect[3].second - rect[0].second;
    if (dx01 * dx03 + dy01 * dy03 != 0)
      return "illegal rectangle";
    if (dx01 != 0 && dy01 != 0 && abs(dx01) != abs(dy01))
      return "illegal rectangle";
    if (rect[1].first + dx03 != rect[2].first || rect[1].second + dy03 != rect[2].second)
      return "illegal rectangle";

    for (int i = 0; i < 4; ++i) {
      int x = rect[i].first, y = rect[i].second;
      const int tx = rect[(i + 1) % 4].first;
      const int ty = rect[(i + 1) % 4].second;
      const int dx = (tx - x) == 0 ? 0 : (tx - x) / abs(tx - x);
      const int dy = (ty - y) == 0 ? 0 : (ty - y) / abs(ty - y);
      const int d = dir_id(dx, dy);
      while (x != tx || y != ty) {
        if (!(x == rect[i].first && y == rect[i].second) && has_point[x][y])
          return "obstacle on perimeter";
        if (used[x][y][d])
          return "overlapped rectangles";
        x += dx;
        y += dy;
        if (used[x][y][d ^ 4])
          return "overlapped rectangles";
      }
    }
    return {};
  }

  void apply_move(const array<pair<int, int>, 4>& rect) {
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
};

int weight(int x, int y, int N) {
  const int c = N / 2;
  const int dx = x - c;
  const int dy = y - c;
  return dx * dx + dy * dy + 1;
}

Input read_input(istream& in) {
  Input input;
  int M;
  in >> input.N >> M;
  input.ps.resize(M);
  for (int i = 0; i < M; ++i)
    in >> input.ps[i].first >> input.ps[i].second;
  return input;
}

vector<array<pair<int, int>, 4>> read_output(istream& in, const Input& input) {
  int K;
  if (!(in >> K))
    return {};
  vector<array<pair<int, int>, 4>> out(K);
  for (int t = 0; t < K; ++t)
    for (int i = 0; i < 4; ++i)
      in >> out[t][i].first >> out[t][i].second;
  return out;
}

int main(int argc, char** argv) {
  if (argc < 3) {
    cerr << "usage: " << argv[0] << " <input> <output>\n";
    return 1;
  }
  ifstream fin(argv[1]);
  ifstream fout(argv[2]);
  const Input input = read_input(fin);
  const auto out = read_output(fout, input);

  State state(input);
  for (int t = 0; t < (int)out.size(); ++t) {
    const string err = state.check_move(out[t]);
    if (!err.empty()) {
      cout << "0\n" << err << " (turn " << t << ")\n";
      return 0;
    }
    state.apply_move(out[t]);
  }

  int num = 0;
  for (auto [x, y] : input.ps)
    num += weight(x, y, input.N);
  for (const auto& rect : out)
    num += weight(rect[0].first, rect[0].second, input.N);
  int den = 0;
  for (int x = 0; x < input.N; ++x)
    for (int y = 0; y < input.N; ++y)
      den += weight(x, y, input.N);
  const int score =
      llround(1e6 * (1.0 * input.N * input.N / (int)input.ps.size()) * (1.0 * num / den));
  cout << score << '\n';
  return 0;
}
