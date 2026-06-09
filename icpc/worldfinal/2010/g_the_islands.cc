#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr double INF = 1e100;

struct Pt {
  double x, y;
};

double dist(const Pt& a, const Pt& b) {
  double dx = a.x - b.x;
  double dy = a.y - b.y;
  return sqrt(dx * dx + dy * dy);
}

struct Parent {
  int pa;
  int pb;
  int pm;
  char add_to; // 'F' or 'R'

  Parent() : pa(-1), pb(-1), pm(-1), add_to('?') {}
  Parent(int a, int b, int m, char c) : pa(a), pb(b), pm(m), add_to(c) {}
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, b1, b2;
  int case_num = 1;
  while (cin >> n >> b1 >> b2) {
    if (n == 0 && b1 == 0 && b2 == 0)
      break;
    vector<Pt> p(n);
    for (int i = 0; i < n; i++)
      cin >> p[i].x >> p[i].y;

    // DP state: endpoints (a,b) where
    // - a is endpoint of forward (east-going) pass, which MUST contain island 1
    // - b is endpoint of the other pass
    // After processing points 0..k, we have max(a,b)=k.
    //
    // m is 2-bit mask: bit0= (b1 in forward pass), bit1=(b2 in forward pass)
    vector<vector<array<double, 4>>> dp(n, vector<array<double, 4>>(n));
    vector<vector<array<Parent, 4>>> par(n, vector<array<Parent, 4>>(n));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++)
        dp[i][j].fill(INF);
    }

    // Initialize: forward pass starts 0->1, other pass is at 0.
    int init_mask = 0;
    if (b1 == 1)
      init_mask |= 1;
    if (b2 == 1)
      init_mask |= 2;
    dp[1][0][init_mask] = dist(p[0], p[1]);

    for (int k = 1; k <= n - 2; k++) {
      int next = k + 1;
      for (int a = 0; a <= k; a++) {
        for (int b = 0; b <= k; b++) {
          if (a == b)
            continue;
          if (max(a, b) != k)
            continue;
          for (int m = 0; m < 4; m++) {
            double cur = dp[a][b][m];
            if (cur >= INF / 2)
              continue;

            // Add 'next' to forward pass (endpoint a -> next).
            {
              int nm = m;
              if (next == b1)
                nm |= 1;
              if (next == b2)
                nm |= 2;
              double cand = cur + dist(p[a], p[next]);
              if (cand + 1e-12 < dp[next][b][nm]) {
                dp[next][b][nm] = cand;
                par[next][b][nm] = Parent{a, b, m, 'F'};
              }
            }

            // Add 'next' to return pass (endpoint b -> next).
            {
              int nm = m;
              // If next is a special island, it belongs to return pass, so bits
              // stay 0.
              double cand = cur + dist(p[b], p[next]);
              if (cand + 1e-12 < dp[a][next][nm]) {
                dp[a][next][nm] = cand;
                par[a][next][nm] = Parent{a, b, m, 'R'};
              }
            }
          }
        }
      }
    }

    // Finish: forward pass must end at n-1. Close by connecting endpoints.
    double best = INF;
    int best_b = -1, best_m = -1;
    int a_final = n - 1;
    for (int b = 0; b <= n - 2; b++) {
      if (b == a_final)
        continue;
      for (int m = 0; m < 4; m++) {
        // Require b1 and b2 are in different passes:
        // forward-membership bits must differ.
        int f1 = (m & 1) ? 1 : 0;
        int f2 = (m & 2) ? 1 : 0;
        if ((f1 ^ f2) == 0)
          continue;

        double cur = dp[a_final][b][m];
        if (cur >= INF / 2)
          continue;
        double cand = cur + dist(p[a_final], p[b]);
        if (cand + 1e-12 < best) {
          best = cand;
          best_b = b;
          best_m = m;
        }
      }
    }

    // Reconstruct which points went to which pass.
    vector<int> added_f, added_r;
    int a = a_final, b = best_b, m = best_m;
    for (int k = n - 1; k >= 2; k--) {
      Parent pr = par[a][b][m];
      if (pr.add_to == 'F')
        added_f.push_back(k);
      else if (pr.add_to == 'R')
        added_r.push_back(k);
      else
        break;
      a = pr.pa;
      b = pr.pb;
      m = pr.pm;
    }

    // Build full visit order:
    // forward pass: 0, 1, ..., n-1
    // then return pass: best_b, ..., 0
    vector<int> forward = {0, 1};
    for (int i = (int)added_f.size() - 1; i >= 0; i--)
      forward.push_back(added_f[i]);
    vector<int> ret = {0};
    for (int i = (int)added_r.size() - 1; i >= 0; i--)
      ret.push_back(added_r[i]);

    // ret is 0 -> ... -> best_b in increasing x (forward direction of
    // return-chain). We traverse it backward from best_b back to 0.
    vector<int> tour = forward;
    for (int i = (int)ret.size() - 1; i >= 0; i--)
      tour.push_back(ret[i]);

    cout << "Case " << case_num++ << ": " << fixed << setprecision(2) << best << "\n";
    for (int i = 0; i < (int)tour.size(); i++) {
      if (i)
        cout << " ";
      cout << tour[i];
    }
    cout << "\n";
  }
  return 0;
}
