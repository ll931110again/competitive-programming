// Google Code Jam 2011 — Round 2 — B. Spinning Blade

#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Prefix2D {
  int R, C;
  vector<vector<long long>> sum, sumx, sumy;

  Prefix2D(int r, int c, const vector<vector<long long>>& w)
      : R(r), C(c), sum(r + 1, vector<long long>(c + 1, 0)),
        sumx(r + 1, vector<long long>(c + 1, 0)), sumy(r + 1, vector<long long>(c + 1, 0)) {
    for (int i = 0; i < r; i++) {
      for (int j = 0; j < c; j++) {
        sum[i + 1][j + 1] = sum[i][j + 1] + sum[i + 1][j] - sum[i][j] + w[i][j];
        sumx[i + 1][j + 1] = sumx[i][j + 1] + sumx[i + 1][j] - sumx[i][j] + w[i][j] * j;
        sumy[i + 1][j + 1] = sumy[i][j + 1] + sumy[i + 1][j] - sumy[i][j] + w[i][j] * i;
      }
    }
  }

  long long rect_sum(const vector<vector<long long>>& pref, int r1, int c1, int r2, int c2) const {
    return pref[r2 + 1][c2 + 1] - pref[r1][c2 + 1] - pref[r2 + 1][c1] + pref[r1][c1];
  }

  void blade_sums(int r1, int c1, int r2, int c2, long long& m, long long& mx, long long& my,
                  const vector<vector<long long>>& w) const {
    m = rect_sum(sum, r1, c1, r2, c2);
    mx = rect_sum(sumx, r1, c1, r2, c2);
    my = rect_sum(sumy, r1, c1, r2, c2);
    m -= w[r1][c1] + w[r1][c2] + w[r2][c1] + w[r2][c2];
    mx -= w[r1][c1] * c1 + w[r1][c2] * c2 + w[r2][c1] * c1 + w[r2][c2] * c2;
    my -= w[r1][c1] * r1 + w[r1][c2] * r1 + w[r2][c1] * r2 + w[r2][c2] * r2;
  }
};

static bool balanced(long long m, long long mx, long long my, int r1, int c1, int r2, int c2) {
  long long cx = c1 + c2;
  long long cy = r1 + r2;
  return 2 * mx == cx * m && 2 * my == cy * m;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int R, C, D;
    cin >> R >> C >> D;
    vector<vector<long long>> w(R, vector<long long>(C));
    for (int i = 0; i < R; i++) {
      string row;
      cin >> row;
      for (int j = 0; j < C; j++)
        w[i][j] = D + (row[j] - '0');
    }

    Prefix2D pref(R, C, w);
    int best = -1;

    for (int r1 = 0; r1 < R; r1++) {
      for (int c1 = 0; c1 < C; c1++) {
        int max_k = min(R - r1, C - c1);
        for (int k = max_k; k >= 3; k--) {
          int r2 = r1 + k - 1;
          int c2 = c1 + k - 1;
          long long m, mx, my;
          pref.blade_sums(r1, c1, r2, c2, m, mx, my, w);
          if (balanced(m, mx, my, r1, c1, r2, c2)) {
            best = max(best, k);
            break;
          }
        }
      }
    }

    cout << "Case #" << tc << ": ";
    if (best < 0)
      cout << "IMPOSSIBLE";
    else
      cout << best;
    cout << '\n';
  }
  return 0;
}
