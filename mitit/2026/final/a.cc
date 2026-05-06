#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <vector>
#define maxn 2005
using namespace std;

int T, n, m;
int parity[maxn][maxn], value[maxn][maxn];

int par[2 * maxn], ans[2 * maxn];
vector<pair<int, int>> diffs;

int get_root(int x) {
  if (x != par[x]) {
    par[x] = get_root(par[x]);
  }
  return par[x];
}

void link(int u, int v) {
  u = get_root(u);
  v = get_root(v);
  if (u == v) {
    return;
  }
  par[v] = u;
}

int min3(int x, int y, int z) { return min(min(x, y), z); }

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    diffs.clear();

    cin >> n >> m;
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= m; j++) {
        cin >> parity[i][j];
      }
    }

    for (int i = 0; i <= n; i++) {
      value[i][0] = i;
      parity[i][0] = i % 2;
    }

    for (int j = 0; j <= m; j++) {
      value[0][j] = j;
      parity[0][j] = j % 2;
    }

    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= m; j++) {
        value[i][j] =
            min3(value[i - 1][j], value[i][j - 1], value[i - 1][j - 1]);
        if (value[i][j] % 2 != parity[i][j]) {
          value[i][j]++;
        }
      }
    }

    for (int i = 1; i <= n + m; i++) {
      par[i] = i;
    }

    bool impossible = false;
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= m; j++) {
        bool can_same = false, can_diff = false;
        if (value[i][j] == value[i - 1][j - 1]) {
          can_same = true;
        }

        if (value[i][j] ==
            min3(value[i - 1][j], value[i][j - 1], value[i - 1][j - 1]) + 1) {
          can_diff = true;
        }

        if (!can_same && !can_diff) {
          impossible = true;
          break;
        }

        if (can_same && can_diff) {
          continue;
        }

        if (can_same) {
          link(i, n + j);
        } else if (can_diff) {
          diffs.push_back({i, n + j});
        }
      }
    }

    for (auto [u, v] : diffs) {
      if (get_root(u) == get_root(v)) {
        impossible = true;
        break;
      }
    }

    if (impossible) {
      cout << -1 << endl;
    } else {
      for (int i = 1; i <= n; i++) {
        cout << get_root(i) << ' ';
      }
      cout << endl;
      for (int i = n + 1; i <= n + m; i++) {
        cout << get_root(i) << ' ';
      }
      cout << endl;
    }
  }

  return 0;
}
