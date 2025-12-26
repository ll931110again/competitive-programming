/*
 * Solution for problem G: Game of 10
 *
 */

#ifdef ONLINE_JUDGE
  #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <unordered_set>
#include <vector>
using namespace std;

struct Move {
  int x, y;
  int score;
};

int N = 4, total_sum = 10;
int board[5][5];
bool is_won = false;

Move read_move() {
  Move a;
  cin >> a.x >> a.y >> a.score;
  return a;
}

void write_move(Move a) {
  cout << a.x << ' ' << a.y << ' ' << a.score;
  if (is_won) {
    cout << " WIN";
  }
  cout << endl;
  cout << flush;
}

Move check_for_next_move(Move last_move) {
  for (int i = 1; i <= N; i++) {
    unordered_set<int> remains;
    for (int j = 1; j <= N; j++) {
      remains.insert(j);
    }
    int sum_value = 0;

    for (int j = 1; j <= N; j++) if (board[i][j]) {
      remains.erase(remains.find(j));
      sum_value += board[i][j];
    }

    if (remains.size() == 1 && total_sum - sum_value > 0 && total_sum - sum_value <= N) {
      is_won = true;
      return {.x = i, .y = *remains.begin(), .score = total_sum - sum_value};
    }
  }

  for (int j = 1; j <= N; j++) {
    unordered_set<int> remains;
    for (int i = 1; i <= N; i++) {
      remains.insert(i);
    }
    int sum_value = 0;

    for (int i = 1; i <= N; i++) if (board[i][j]) {
      remains.erase(remains.find(i));
      sum_value += board[i][j];
    }

    if (remains.size() == 1 && total_sum - sum_value > 0 && total_sum - sum_value <= N) {
      is_won = true;
      return {.x = *remains.begin(), .y = j, .score = total_sum - sum_value};
    }    
  }

  return {.x = last_move.x, .y = N + 1 - last_move.y, .score = N + 1 - last_move.score};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  while (!is_won) {
    Move a = read_move();
    board[a.x][a.y] = a.score;

    a = check_for_next_move(a);
    board[a.x][a.y] = a.score;

    write_move(a);
  }

  return 0;
}
