#include <bits/stdc++.h>
using namespace std;

using u128 = unsigned __int128;

namespace {

string to_binary_u128(u128 x) {
  if (x == 0)
    return "0";
  string s;
  while (x > 0) {
    s.push_back((x & 1) ? '1' : '0');
    x >>= 1;
  }
  reverse(s.begin(), s.end());
  return s;
}

bool matches_pattern(const string& pat, const string& full) {
  if (pat.size() != full.size())
    return false;
  for (int i = 0; i < (int)pat.size(); i++) {
    if (pat[i] != '?' && pat[i] != full[i])
      return false;
  }
  return true;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    string S;
    cin >> S;
    // Pad to even length for pairwise (base-4) sqrt algorithm.
    if ((S.size() & 1) == 1)
      S = "0" + S;
    int L = (int)S.size();
    int pairs = L / 2;

    // For each pair position i (from MSB to LSB), determine allowed values 0..3.
    vector<vector<int>> allowed(pairs);
    for (int i = 0; i < pairs; i++) {
      char a = S[2 * i];
      char b = S[2 * i + 1];
      for (int v = 0; v < 4; v++) {
        char aa = (v & 2) ? '1' : '0';
        char bb = (v & 1) ? '1' : '0';
        if (a != '?' && a != aa)
          continue;
        if (b != '?' && b != bb)
          continue;
        allowed[i].push_back(v);
      }
    }

    string answer = "";

    // Backtracking digit-by-digit square root in base 2 (processing 2 bits at a time).
    // Standard algorithm:
    // rem = (rem<<2) + next_pair
    // cand = (root<<2) + 1
    // if rem >= cand: rem-=cand, root=(root<<1)+1 else root<<=1
    // This constructs floor(sqrt(N)); for perfect squares remainder ends at 0.
    auto dfs = [&](int i, u128 rem, unsigned long long root) {
      if (!answer.empty())
        return;
      if (i == pairs) {
        if (rem != 0)
          return;
        u128 sq = (u128)root * (u128)root;
        string out = to_binary_u128(sq);
        // Remove potential leading zeros introduced by padding.
        if ((int)out.size() < L)
          out = string(L - out.size(), '0') + out;
        if (matches_pattern(S, out)) {
          // Strip the padding if we added it.
          if (out.size() >= 2 && out[0] == '0')
            out.erase(out.begin());
          answer = out;
        }
        return;
      }

      for (int v : allowed[i]) {
        u128 rem2 = (rem << 2) + (u128)v;
        u128 cand = ((u128)root << 2) + 1;
        if (rem2 >= cand) {
          dfs(i + 1, rem2 - cand, (root << 1) + 1ULL);
        } else {
          dfs(i + 1, rem2, (root << 1));
        }
        if (!answer.empty())
          return;
      }
    };

    dfs(0, 0, 0);

    cout << "Case #" << tc << ": " << answer << "\n";
  }
  return 0;
}
