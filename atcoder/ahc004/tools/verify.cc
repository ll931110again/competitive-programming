// Local verifier for AHC004 A (matches tools/src/lib.rs scoring).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int N = 20;
constexpr int NN = N * N;

int mod_n(int x) {
  return x >= N ? x - N : x;
}

bool is_substring(const vector<string>& g, const string& t) {
  for (int r = 0; r < N; ++r) {
    for (int c = 0; c < N; ++c) {
      for (int d = 0; d < 2; ++d) {
        bool ok = true;
        for (int k = 0; k < (int)t.size(); ++k) {
          const int i = mod_n(r + d * k);
          const int j = mod_n(c + (1 - d) * k);
          if (g[i][j] != t[k]) {
            ok = false;
            break;
          }
        }
        if (ok)
          return true;
      }
    }
  }
  return false;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n_in, m;
  cin >> n_in >> m;
  vector<string> s(m);
  for (int i = 0; i < m; ++i)
    cin >> s[i];

  vector<string> g(N);
  for (int i = 0; i < N; ++i) {
    cin >> g[i];
    if ((int)g[i].size() != N) {
      cout << "invalid row length\n";
      return 1;
    }
    for (char ch : g[i]) {
      if (ch != '.' && (ch < 'A' || ch > 'H')) {
        cout << "invalid char\n";
        return 1;
      }
    }
  }

  int c = 0, d = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (g[i][j] == '.')
        ++d;
    }
  }
  for (const auto& t : s) {
    if (is_substring(g, t))
      ++c;
  }

  const long double score =
      (c < m) ? 1e8 * c / m : 1e8 * (2LL * NN) / (2LL * NN - d);
  cout << "c=" << c << " d=" << d << " score=" << llround(score) << '\n';
  return 0;
}
