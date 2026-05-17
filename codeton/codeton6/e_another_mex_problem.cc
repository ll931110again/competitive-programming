// Codeforces 1870 (CodeTON Round 6 (Div. 1 + Div. 2, Rated, Prizes!)) — E. Another MEX Problem
// Submission: https://codeforces.com/contest/1870/submission/307456599

#include <algorithm>
#include <iostream>
#include <cstring>
#include <queue>
#include <vector>
#include <unordered_set>
#define maxn 5005
#define maxxor 5005
using namespace std;

int T, n, k;
int a[maxn];

int min_index[maxn][maxn];
bool marked[maxn];

int dp[maxxor];
bool vis[maxxor];

int solve() {
  for (int j = 0; j <= n; j++) {
    min_index[n][j] = -1;
  }
  for (int i = n - 1; i >= 0; i--) {
    for (int j = 0; j <= n; j++) {
      min_index[i][j] = min_index[i + 1][j];
    }

    int value = 0;
    for (int j = i; j <= n; j++) {
      if (min_index[i][value] < 0 || min_index[i][value] > j) {
        min_index[i][value] = j;
      }

      marked[a[j]] = true;
      while (marked[value]) {
        value++;
      }
    }

    for (int j = i; j <= n; j++) {
      marked[a[j]] = false;
    }
  }

  for (int i = 0; i < maxxor; i++) {
    dp[i] = -1;
    vis[i] = false;
  }

  dp[0] = 0;
  vector<int> buckets[maxn];
  buckets[0].push_back(0);

  for (int pos = 0; pos <= n; pos++) {
    vector<int> valid_idx;
    for (int j = 1; j <= n; j++)
      if (min_index[pos][j] >= 0) {
        valid_idx.push_back(j);
      }
    for (auto xval : buckets[pos]) {
      if (vis[xval]) {
        continue;
      }

      vis[xval] = true;

      for (auto j : valid_idx) {
        int vx = min_index[pos][j], yval = xval ^ j;
        if (dp[yval] < 0 || dp[yval] > vx) {
          dp[yval] = vx;
          buckets[vx].push_back(yval);
        }
      }
    }
  }

  for (int j = maxxor - 1; j >= 0; j--)
    if (dp[j] >= 0) {
      return j;
    }

  return 0;
}

int main() {
  std::ios::sync_with_stdio(false);
  std::cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }

    cout << solve() << endl;
  }
  return 0;
}
