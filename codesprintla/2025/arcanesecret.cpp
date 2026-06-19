// B – Arcane Secret
// https://open.kattis.com/problems/arcanesecret

#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, K;
  if (!(cin >> N >> K))
    return 0;
  const int g = N / K;
  vector<i64> s(N);
  for (int i = 0; i < N; ++i)
    cin >> s[i];

  i64 mn = s[0], mx = s[0];
  for (int i = 1; i < N; ++i) {
    mn = min(mn, s[i]);
    mx = max(mx, s[i]);
  }
  if (mn == mx) {
    cout << N << '\n';
    return 0;
  }

  vector<int> ord(N);
  iota(ord.begin(), ord.end(), 0);
  sort(ord.begin(), ord.end(), [&](int i, int j) {
    if (s[i] != s[j])
      return s[i] < s[j];
    return i < j;
  });

  // Rank positions p in sorted order (0 .. N-1) that can be the median rank among
  // the K group-maxima satisfy:
  //   p >= g(t+1)-1  and  p <= N-t-1
  // where t = (K-1)/2 (half of the other groups below / above in strength).
  const int t = (K - 1) / 2;
  int L = g * (t + 1) - 1;
  int R = N - t - 1;

  while (L > 0 && s[ord[L - 1]] == s[ord[L]])
    --L;
  while (R < N - 1 && s[ord[R + 1]] == s[ord[R]])
    ++R;

  vector<char> in(N, 0);
  for (int p = L; p <= R; ++p)
    in[ord[p]] = 1;
  int ans = 0;
  for (int i = 0; i < N; ++i)
    ans += in[i];
  cout << ans << '\n';
  return 0;
}
