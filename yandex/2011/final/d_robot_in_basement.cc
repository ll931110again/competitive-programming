// Codeforces 97 (Yandex.Algorithm 2011: Finals) — D. Robot in Basement
// Submission: https://codeforces.com/contest/97/submission/263694228

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 150;
constexpr int k_max_v = 30;
#define maxk 5

int m, n, k;
string s[k_max_n];
string query;

pair<int, int> endpoint;
int cur[k_max_n][maxk], nxt[k_max_n][maxk], wall[k_max_n][maxk];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> m >> n >> k;
  for (int i = 0; i < m; i++) {
    cin >> s[i];
  }
  cin >> query;

  for (int i = m; i < k_max_n; i++) {
    for (int j = n; j < k_max_n; j++) {
      wall[i][j / k_max_v] |= (1 << (j % k_max_v));
    }
  }

  bool found_empty_space = false;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < n; j++) {
      if (s[i][j] == '.') {
        found_empty_space = true;
        cur[i][j / k_max_v] |= (1 << (j % k_max_v));
      } else if (s[i][j] == 'E') {
        endpoint = make_pair(i, j);
        cur[i][j / k_max_v] |= (1 << (j % k_max_v));
      } else {
        wall[i][j / k_max_v] |= (1 << (j % k_max_v));
      }
    }
  }

  auto is_empty = [&]() {
    if (!found_empty_space) {
      return true;
    }

    /*
    cerr << "step" << endl;
    for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                    if (cur[i][j / k_max_v] & (1 << (j % k_max_v))) {
                            cerr << 1;
                    } else {
                            cerr << 0;
                    }
            }
            cerr << endl;
    }
    cerr << endl;
    */

    for (int i = 0; i < m; i++) {
      for (int j = 0; j < maxk; j++) {
        if (i == endpoint.first && j == endpoint.second / k_max_v) {
          if (cur[i][j] != (1 << (endpoint.second % k_max_v))) {
            return false;
          }
        } else {
          if (cur[i][j]) {
            return false;
          }
        }
      }
    }
    return true;
  };

  for (int i = 0; i <= k; i++) {
    if (is_empty()) {
      cout << i << endl;
      return 0;
    }
    if (i == k) {
      cout << -1 << endl;
      return 0;
    }

    memset(nxt, 0, sizeof nxt);
    if (query[i] == 'U') {
      for (int u = 0; u < m; u++) {
        for (int v = 0; v < maxk; v++) {
          if (u == 0) {
            nxt[u][v] |= cur[u][v];
          } else {
            nxt[u - 1][v] |= (cur[u][v] & ~wall[u - 1][v]);
            nxt[u][v] |= (cur[u][v] & wall[u - 1][v]);
          }
        }
      }
    } else if (query[i] == 'D') {
      for (int u = 0; u < m; u++) {
        for (int v = 0; v < maxk; v++) {
          if (u == m - 1) {
            nxt[u][v] |= cur[u][v];
          } else {
            nxt[u + 1][v] |= (cur[u][v] & ~wall[u + 1][v]);
            nxt[u][v] |= (cur[u][v] & wall[u + 1][v]);
          }
        }
      }
    } else if (query[i] == 'L') {
      for (int u = 0; u < m; u++) {
        for (int v = 0; v < maxk; v++) {
          nxt[u][v] |= (cur[u][v] >> 1);
          if (v + 1 < maxk && (cur[u][v + 1] & 1)) {
            nxt[u][v] |= (1 << (k_max_v - 1));
          }

          nxt[u][v] |= (cur[u][v] & (wall[u][v] << 1));
          if (v == 0 || (wall[u][v - 1] & (1 << (k_max_v - 1)))) {
            nxt[u][v] |= (cur[u][v] & 1);
          }
          nxt[u][v] &= ~wall[u][v];
        }
      }
    } else if (query[i] == 'R') {
      for (int u = 0; u < m; u++) {
        for (int v = 0; v < maxk; v++) {
          nxt[u][v] |= ((cur[u][v] << 1) & ((1 << k_max_v) - 1));
          if (v > 0 && (cur[u][v - 1] & (1 << (k_max_v - 1)))) {
            nxt[u][v] |= 1;
          }

          nxt[u][v] |= (cur[u][v] & (wall[u][v] >> 1));
          if (v == maxk - 1 || (wall[u][v + 1] & 1)) {
            nxt[u][v] |= (cur[u][v] & (1 << (k_max_v - 1)));
          }
          nxt[u][v] &= ~wall[u][v];
        }
      }
    }

    for (int u = 0; u < m; u++) {
      for (int v = 0; v < maxk; v++) {
        cur[u][v] = nxt[u][v];
      }
    }
  }

  return 0;
}
