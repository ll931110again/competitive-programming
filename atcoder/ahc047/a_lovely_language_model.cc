// AHC047 A - Lovely Language Model: https://atcoder.jp/contests/ahc047/tasks/ahc047_a
//
// Chain Markov model prioritizing high-P strings as embedded paths.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

struct Input {
  int n = 0, m = 0, len = 0;
  vector<string> strings;
  vector<int> priority;
};

struct Solver {
  Input in;

  void solve() const {
    vector<int> ord(in.n);
    iota(ord.begin(), ord.end(), 0);
    sort(ord.begin(), ord.end(), [&](int a, int b) {
      if (in.priority[a] != in.priority[b])
        return in.priority[a] > in.priority[b];
      return in.strings[a] < in.strings[b];
    });

    string chars(in.m, 'a');
    vector<vector<int>> nxt(in.m, vector<int>(in.m, 0));
    for (int s = 0; s < in.m; ++s) {
      const string& ref = in.strings[ord[s % in.n]];
      chars[s] = ref[s % (int)ref.size()];
      nxt[s][(s + 1) % in.m] = 100;
    }

    for (int i = 0; i < in.m; ++i) {
      cout << chars[i];
      for (int j = 0; j < in.m; ++j)
        cout << ' ' << nxt[i][j];
      cout << '\n';
    }
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Input in;
  cin >> in.n >> in.m >> in.len;
  in.strings.resize(in.n);
  in.priority.resize(in.n);
  for (int i = 0; i < in.n; ++i)
    cin >> in.strings[i] >> in.priority[i];

  Solver{in}.solve();
  return 0;
}
