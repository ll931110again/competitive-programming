#include <cstdlib>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
using namespace std;

static string manhattan_path(pair<int, int> start, pair<int, int> end) {
  string out;
  for (int i = 0; i < abs(start.first - end.first); i++) {
    out += start.first < end.first ? 'D' : 'U';
  }
  for (int i = 0; i < abs(start.second - end.second); i++) {
    out += start.second < end.second ? 'R' : 'L';
  }
  return out;
}

int main() {
  vector<string> board;

  string s;
  while (cin >> s) {
    board.push_back(s);
  }

  int n = (int)board.size();
  if (n == 0) {
    cout << "\n";
    return 0;
  }

  const vector<pair<int, int>> corners = {
      {0, 0}, {0, n - 1}, {n - 1, 0}, {n - 1, n - 1}};

  string best;
  for (const auto &candidate : corners) {
    string cmd = "";
    if (candidate.first == 0) {
      for (int i = 0; i + 1 < n; i++) {
        cmd += "U";
      }
    }
    if (candidate.second == 0) {
      for (int i = 0; i + 1 < n; i++) {
        cmd += "L";
      }
    }
    if (candidate.first == n - 1) {
      for (int i = 0; i + 1 < n; i++) {
        cmd += "D";
      }
    }
    if (candidate.second == n - 1) {
      for (int i = 0; i + 1 < n; i++) {
        cmd += "R";
      }
    }

    pair<int, int> black_cell = {-1, -1};
    int min_distance = n * n;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (board[i][j] == '1') {
          if (black_cell == make_pair(-1, -1) ||
              abs(i - candidate.first) + abs(j - candidate.second) <
                  min_distance) {
            black_cell = {i, j};
            min_distance = abs(i - candidate.first) + abs(j - candidate.second);
          }
        }
      }
    }

    cmd += manhattan_path(candidate, black_cell);

    if (best.empty() || cmd.size() < best.size() ||
        (cmd.size() == best.size() && cmd < best)) {
      best = cmd;
    }
  }

  cout << best << endl;
  return 0;
}
