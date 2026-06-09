
#include <bits/stdc++.h>
using namespace std;

using ll = long long;

namespace {

constexpr int MAXN = 250002;
constexpr int LOG = 20;

int T;
int n;
long long z;
int x[MAXN];
int q;

int nex[MAXN][LOG];
int nex2[MAXN][LOG];
ll step[MAXN][LOG];

void solve_one() {
  cin >> n >> z;
  for (int i = 0; i < n; ++i) {
    cin >> x[i];
  }

  nex[n][0] = n;
  for (int i = 0; i < n; ++i) {
    long long t = (long long)x[i] + z + 1;
    nex[i][0] = (int)(lower_bound(x, x + n, t) - x);
  }

  for (int k = 0; k + 1 < LOG; ++k) {
    for (int j = 0; j <= n; ++j) {
      nex[j][k + 1] = nex[nex[j][k]][k];
    }
  }

  for (int i = 0; i < n - 1; ++i) {
    int cur1 = i;
    int cur2 = i + 1;
    int st = 0;
    for (int j = LOG - 1; j >= 0; --j) {
      if (nex[cur1][j] != nex[cur2][j] && nex[nex[cur1][j]][0] != nex[cur2][j]) {
        cur1 = nex[cur1][j];
        cur2 = nex[cur2][j];
        st += 2 << j;
      }
    }
    if (nex[cur1][0] == cur2) {
      nex2[i][0] = nex[cur1][0];
      step[i][0] = st + 1;
    } else if (nex[cur1][0] == nex[cur2][0]) {
      nex2[i][0] = nex[cur1][0];
      step[i][0] = st + 2;
    } else {
      // Third collision type: parallel chains meet after one extra step on the left.
      nex2[i][0] = nex[cur1][1];
      step[i][0] = st + 3;
    }
  }
  nex2[n - 1][0] = n;
  nex2[n][0] = n;
  step[n][0] = 0;

  for (int k = 0; k + 1 < LOG; ++k) {
    for (int j = 0; j <= n; ++j) {
      nex2[j][k + 1] = nex2[nex2[j][k]][k];
      step[j][k + 1] = step[j][k] + step[nex2[j][k]][k];
    }
  }

  cin >> q;
  while (q--) {
    int L, R;
    cin >> L >> R;
    --L;
    // Half-open interval [L, R): 1-based inclusive [L+1, R] -> L = l-1, R = r (exclusive end).
    if (L + 1 == R) {
      cout << 1 << '\n';
      continue;
    }
    ll ret = 2;
    int cur = L;
    for (int j = LOG - 1; j >= 0; --j) {
      if (nex2[cur][j] < R) {
        ret += step[cur][j];
        cur = nex2[cur][j];
      }
    }
    if (cur + 1 == R) {
      --ret;
    } else {
      int a = cur;
      int b = cur + 1;
      for (int j = LOG - 1; j >= 0; --j) {
        if (nex[a][j] < R) {
          a = nex[a][j];
          ret += 1LL << j;
        }
        if (nex[b][j] < R) {
          b = nex[b][j];
          ret += 1LL << j;
        }
      }
    }
    cout << ret << '\n';
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    solve_one();
  }
  return 0;
}
