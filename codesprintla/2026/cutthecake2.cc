#include <bits/stdc++.h>
using namespace std;

namespace {

struct Cut {
  char t;
  int a, b, c;
};

int N, M, Kglobal;
int query_budget = 0;

int ask(int x1, int x2, int y1, int y2) {
  ++query_budget;
  cout << "? " << x1 << ' ' << x2 << ' ' << y1 << ' ' << y2 << '\n';
  cout.flush();
  int r;
  if (!(cin >> r)) {
    exit(0);
  }
  if (r < 0) {
    exit(0);
  }
  return r;
}

// Memoize oracle answers for axis-aligned boxes we actually query.
map<tuple<int, int, int, int>, int> memo_q;

int Q(int x1, int x2, int y1, int y2) {
  const auto key = make_tuple(x1, x2, y1, y2);
  auto it = memo_q.find(key);
  if (it != memo_q.end()) {
    return it->second;
  }
  const int v = ask(x1, x2, y1, y2);
  memo_q[key] = v;
  return v;
}

vector<Cut> cuts_out;

bool dfs(int xl, int xr, int yb, int yt, int k) {
  const int qfull = Q(xl, xr, yb, yt);
  if (k == 1) {
    return qfull == 0;
  }
  if (qfull == 0) {
    return false;
  }

  // Vertical split: line x for xl < x < xr; cut segment from (x, yb) to (x, yt).
  for (int x = xl + 1; x < xr; x++) {
    const int q_l = Q(xl, x, yb, yt);
    const int q_r = Q(x, xr, yb, yt);

    vector<pair<int, int>> cand;
    const bool z_l = (q_l == 0);
    const bool z_r = (q_r == 0);
    if (z_l && z_r) {
      continue;
    }
    if (z_l && !z_r) {
      cand.push_back({1, k - 1});
    } else if (!z_l && z_r) {
      cand.push_back({k - 1, 1});
    } else {
      for (int k_l = 2; k_l <= k - 2; k_l++) {
        cand.push_back({k_l, k - k_l});
      }
    }

    for (auto [k_l, k_r] : cand) {
      if (k_l <= 0 || k_r <= 0 || k_l + k_r != k) {
        continue;
      }
      const bool ok_l = (k_l == 1) == (q_l == 0);
      const bool ok_r = (k_r == 1) == (q_r == 0);
      if (!ok_l || !ok_r) {
        continue;
      }
      const size_t before = cuts_out.size();
      cuts_out.push_back({'v', x, yb, yt});
      if (dfs(xl, x, yb, yt, k_l) && dfs(x, xr, yb, yt, k_r)) {
        return true;
      }
      cuts_out.resize(before);
    }
  }

  // Horizontal split: line y for yb < y < yt; cut from (xl, y) to (xr, y).
  for (int y = yb + 1; y < yt; y++) {
    const int q_d = Q(xl, xr, yb, y);
    const int q_u = Q(xl, xr, y, yt);

    vector<pair<int, int>> cand;
    const bool z_d = (q_d == 0);
    const bool z_u = (q_u == 0);
    if (z_d && z_u) {
      continue;
    }
    if (z_d && !z_u) {
      cand.push_back({1, k - 1});
    } else if (!z_d && z_u) {
      cand.push_back({k - 1, 1});
    } else {
      for (int k_d = 2; k_d <= k - 2; k_d++) {
        cand.push_back({k_d, k - k_d});
      }
    }

    for (auto [k_d, k_u] : cand) {
      if (k_d <= 0 || k_u <= 0 || k_d + k_u != k) {
        continue;
      }
      const bool ok_d = (k_d == 1) == (q_d == 0);
      const bool ok_u = (k_u == 1) == (q_u == 0);
      if (!ok_d || !ok_u) {
        continue;
      }
      const size_t before = cuts_out.size();
      cuts_out.push_back({'h', xl, xr, y});
      if (dfs(xl, xr, yb, y, k_d) && dfs(xl, xr, y, yt, k_u)) {
        return true;
      }
      cuts_out.resize(before);
    }
  }

  return false;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  if (!(cin >> N >> M >> Kglobal)) {
    return 0;
  }

  cuts_out.clear();
  memo_q.clear();
  query_budget = 0;

  if (!dfs(0, N, 0, M, Kglobal)) {
    // Should not happen on judge data if invariants hold.
    cout << "! 0\n";
    cout.flush();
    return 0;
  }

  cout << "! " << cuts_out.size() << '\n';
  for (const Cut& ce : cuts_out) {
    if (ce.t == 'h') {
      cout << 'h' << ' ' << ce.a << ' ' << ce.b << ' ' << ce.c << '\n';
    } else {
      cout << 'v' << ' ' << ce.a << ' ' << ce.b << ' ' << ce.c << '\n';
    }
  }
  cout.flush();
  return 0;
}
