#include <bits/stdc++.h>
using namespace std;

namespace {

string compress_runs(const string& s) {
  string t;
  for (char c : s) {
    if (t.empty() || t.back() != c)
      t += c;
  }
  return t;
}

int solve_case(const string& original) {
  // Per analysis: optimal strategy satisfies:
  // - never push same as top
  // - after push, immediately print
  // - never have XYX on stack
  // Under these rules, stacks (height >= 2) are a 3-cycle like ABCABC...
  // Therefore the stack is determined by (first two letters pattern, height).
  const string s = compress_runs(original);
  const int n = (int)s.size();

  const int INF = 1e9;

  // Patterns are permutations of 3 letters describing the cycle.
  // If pattern = "ABC", then height=1 top=A, height=2 top=B, height=3 top=C, ...
  const array<array<int, 3>, 6> pat = {{
      {{0, 1, 2}}, // ABC
      {{0, 2, 1}}, // ACB
      {{1, 0, 2}}, // BAC
      {{1, 2, 0}}, // BCA
      {{2, 0, 1}}, // CAB
      {{2, 1, 0}}, // CBA
  }};

  // Map ordered pair (bottom, top) to pattern index.
  int pat_id[3][3];
  for (int a = 0; a < 3; a++)
    for (int b = 0; b < 3; b++)
      pat_id[a][b] = -1;
  for (int p = 0; p < 6; p++)
    pat_id[pat[p][0]][pat[p][1]] = p;

  int dp0 = 0;       // empty stack
  array<int, 3> dp1; // height=1, only top known
  dp1.fill(INF);
  vector<array<int, 6>> max_h; // unused, keep simple; heights up to n

  vector<vector<int>> dp2(6, vector<int>(n + 2, INF));
  // dp2[p][h] valid for h >= 2

  auto close_pops = [&]() {
    // propagate pops within dp2
    for (int p = 0; p < 6; p++) {
      for (int h = n; h >= 3; h--) {
        int v = dp2[p][h];
        if (v < INF && v + 1 < dp2[p][h - 1])
          dp2[p][h - 1] = v + 1;
      }
      if (dp2[p][2] < INF) {
        int top_after_pop_to_1 = pat[p][0];
        dp1[top_after_pop_to_1] = min(dp1[top_after_pop_to_1], dp2[p][2] + 1);
      }
    }
    for (int c = 0; c < 3; c++) {
      if (dp1[c] < INF)
        dp0 = min(dp0, dp1[c] + 1);
    }
  };

  for (int i = 0; i < n; i++) {
    close_pops();

    const int need = s[i] - 'A';

    int ndp0 = INF;
    array<int, 3> ndp1;
    ndp1.fill(INF);
    vector<vector<int>> ndp2(6, vector<int>(n + 2, INF));

    // Print if possible.
    if (dp1[need] < INF)
      ndp1[need] = min(ndp1[need], dp1[need] + 1);
    for (int p = 0; p < 6; p++) {
      for (int h = 2; h <= n; h++) {
        int v = dp2[p][h];
        if (v >= INF)
          continue;
        int top = pat[p][(h - 1) % 3];
        if (top == need) {
          ndp2[p][h] = min(ndp2[p][h], v + 1);
        }
      }
    }

    // Push + immediate print (cost 2).
    if (dp0 < INF)
      ndp1[need] = min(ndp1[need], dp0 + 2);

    for (int top = 0; top < 3; top++) {
      if (dp1[top] >= INF)
        continue;
      if (top == need)
        continue; // never push same as top
      int p = pat_id[top][need];
      ndp2[p][2] = min(ndp2[p][2], dp1[top] + 2);
    }

    for (int p = 0; p < 6; p++) {
      for (int h = 2; h <= n; h++) {
        int v = dp2[p][h];
        if (v >= INF)
          continue;
        int next_push = pat[p][h % 3];
        if (next_push == need) {
          ndp2[p][h + 1] = min(ndp2[p][h + 1], v + 2);
        }
      }
    }

    dp0 = ndp0;
    dp1 = ndp1;
    dp2.swap(ndp2);
  }

  // After printing everything, pop everything.
  close_pops();
  return dp0;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;

  for (int tc = 1; tc <= T; tc++) {
    string s;
    cin >> s;

    int compressed_ops = solve_case(s);
    // Each run-compressed character corresponds to at least one print,
    // but the real input prints each original character.
    // Replacing a run of identical letters with one letter doesn't change
    // the required pushes/pops, only reduces prints by (run_len - 1).
    const int extra_prints = (int)s.size() - (int)compress_runs(s).size();
    cout << "Case #" << tc << ": " << (compressed_ops + extra_prints) << '\n';
  }

  return 0;
}
