// AHC064 A - Non-Crossing Railcar Rearrangement:
// https://atcoder.jp/contests/ahc064/tasks/ahc064_a
//
// Dump mains to sidings, then build each track r by pulling 10r..10r+9 from
// sidings using an empty buffer line for rotation.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int R = 10;
constexpr int LEN = 10;

struct Move {
  int type, i, j, k;
};

struct Solver {
  vector<vector<int>> main_lines;
  vector<vector<int>> siding;
  vector<vector<Move>> turns;

  void add_move(int type, int i, int j, int k) {
    turns.push_back({{type, i, j, k}});
    if (type == 0) {
      const int n = (int)main_lines[i].size();
      vector<int> tail(main_lines[i].begin() + n - k, main_lines[i].end());
      main_lines[i].resize(n - k);
      siding[j].insert(siding[j].begin(), tail.begin(), tail.end());
    } else {
      vector<int> head(siding[j].begin(), siding[j].begin() + k);
      siding[j].erase(siding[j].begin(), siding[j].begin() + k);
      main_lines[i].insert(main_lines[i].end(), head.begin(), head.end());
    }
  }

  int find_siding(int car) const {
    for (int s = 0; s < R; ++s) {
      for (int x : siding[s]) {
        if (x == car)
          return s;
      }
    }
    return -1;
  }

  void rotate_to_front(int buf, int sid, int car, int temp) {
    while (!siding[sid].empty() && siding[sid][0] != car) {
      add_move(1, buf, sid, 1);
      add_move(0, buf, temp, 1);
    }
  }

  void restore_temp(int buf, int sid, int temp) {
    while (!siding[temp].empty()) {
      add_move(1, buf, temp, 1);
      add_move(0, buf, sid, 1);
    }
  }

  void solve() {
    for (int i = 0; i < R; ++i)
      add_move(0, i, i, (int)main_lines[i].size());

    for (int r = 0; r < R; ++r) {
      const int buf = (r + 1) % R;
      const int temp = (r + 5) % R;
      for (int k = 0; k < LEN; ++k) {
        const int want = 10 * r + k;
        const int sid = find_siding(want);
        rotate_to_front(buf, sid, want, temp);
        add_move(1, r, sid, 1);
        restore_temp(buf, sid, temp);
      }
    }
  }

  void write() const {
    cout << turns.size() << '\n';
    for (const auto& mv : turns) {
      cout << mv.size() << '\n';
      for (const auto& m : mv)
        cout << m.type << ' ' << m.i << ' ' << m.j << ' ' << m.k << '\n';
    }
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int r_in;
  cin >> r_in;

  Solver solver;
  solver.main_lines.assign(R, vector<int>(LEN));
  solver.siding.assign(R, {});
  for (int i = 0; i < R; ++i) {
    for (int j = 0; j < LEN; ++j)
      cin >> solver.main_lines[i][j];
  }

  solver.solve();
  solver.write();
  return 0;
}
