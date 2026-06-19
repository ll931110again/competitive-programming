#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 100000;
bool prime[k_max_n + 3];
int num_prime[k_max_n + 3][6];
int board[8][8], T, digit;
i64 ans;

inline void attempt(int x, int y, i64 way) {
  if (x >= digit) {
    ans += way;
    return;
  }
  if (y >= digit) {
    int val = 0;
    for (int i = 0; i < digit; i++)
      val = val * 10 + board[x][i];
    attempt(x + 1, 0, way * num_prime[val][digit - 1 - x]);
    return;
  }
  if (x >= y) {
    board[x][y] = board[y][x];
    attempt(x, y + 1, way);
  } else
    for (int k = 0; k < 10; k++) {
      board[x][y] = k;
      attempt(x, y + 1, way);
    }
}

i64 solve(int n) {
  memset(board, 0, sizeof(board));
  digit = 0;
  while (n) {
    board[0][digit] = n % 10;
    n /= 10;
    digit++;
  }
  for (int i = 0, j = digit - 1; i < j; i++, j--)
    swap(board[0][i], board[0][j]);
  ans = 0;
  attempt(1, 0, 1);
  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  prime[0] = prime[1] = true;
  for (int i = 2; i <= k_max_n; i++)
    if (!prime[i])
      for (int j = i + i; j <= k_max_n; j += i)
        prime[j] = true;
  for (int i = 0; i < k_max_n; i++) {
    int prod = 1;
    for (int j = 0; j < 6; j++) {
      int x = i;
      for (int k = 0; k < 10; k++) {
        if (x < k_max_n && !prime[x])
          num_prime[i][j]++;
        x += prod;
      }
      prod *= 10;
    }
  }
  cin >> T;
  while (T--) {
    int n;
    cin >> n;
    cout << solve(n) << endl;
  }
}
