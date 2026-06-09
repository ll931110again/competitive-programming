// K – Mapping the Undercity
// https://open.kattis.com/problems/mappingtheundercity
//
// Let L = dist(1,2). Each vertex i lies in the subtree rooted at the unique junction on the
// path 1–2 at distance u_i from 1: u_i = (a_i + L - b_i) / 2, with excess e_i = (a_i+b_i-L)/2
// the depth below that junction. Vertices on the spine satisfy e_i = 0; there must be exactly
// L+1 of them. Each attachment bucket u independently contributes ∏_k |L_{k-1}|^{|L_k|} rooted
// trees with prescribed depths from the spine vertex at u.

#include <bits/stdc++.h>
using namespace std;

namespace {

const long long MOD = 1000000007LL;

long long mod_pow(long long a, long long e) {
  long long r = 1 % MOD;
  while (e > 0) {
    if (e & 1)
      r = r * a % MOD;
    a = a * a % MOD;
    e >>= 1;
  }
  return r;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  if (!(cin >> N))
    return 0;
  vector<int> a(N), b(N);
  for (int i = 0; i < N; ++i)
    cin >> a[i];
  for (int i = 0; i < N; ++i)
    cin >> b[i];

  if (a[0] != 0 || b[1] != 0) {
    cout << 0 << '\n';
    return 0;
  }
  const int L = a[1];
  if (b[0] != L) {
    cout << 0 << '\n';
    return 0;
  }
  if (L > N - 1) {
    cout << 0 << '\n';
    return 0;
  }

  vector<vector<int>> bucket(L + 1);
  int on_path = 0;

  for (int i = 0; i < N; ++i) {
    long long sum = (long long)a[i] + b[i];
    long long excess = sum - L;
    if (excess < 0 || (excess & 1LL)) {
      cout << 0 << '\n';
      return 0;
    }
    long long unum = (long long)a[i] + L - b[i];
    if (unum < 0 || (unum & 1LL)) {
      cout << 0 << '\n';
      return 0;
    }
    long long u = unum / 2;
    if (u < 0 || u > L) {
      cout << 0 << '\n';
      return 0;
    }
    bucket[(int)u].push_back(i);
    if (sum == L)
      ++on_path;
  }

  if (on_path != L + 1) {
    cout << 0 << '\n';
    return 0;
  }

  long long ans = 1;

  for (int u = 0; u <= L; ++u) {
    if (bucket[u].empty()) {
      cout << 0 << '\n';
      return 0;
    }
    map<int, int> lvl_cnt;
    for (int idx : bucket[u]) {
      int h = a[idx] - u;
      if (h < 0) {
        cout << 0 << '\n';
        return 0;
      }
      lvl_cnt[h]++;
    }
    if (lvl_cnt[0] != 1) {
      cout << 0 << '\n';
      return 0;
    }
    int max_h = 0;
    for (const auto& pr : lvl_cnt)
      max_h = max(max_h, pr.first);
    for (int k = 1; k <= max_h; ++k) {
      int cnt_k = lvl_cnt.count(k) ? lvl_cnt[k] : 0;
      if (cnt_k == 0)
        continue;
      int cnt_km1 = lvl_cnt.count(k - 1) ? lvl_cnt[k - 1] : 0;
      if (cnt_km1 == 0) {
        cout << 0 << '\n';
        return 0;
      }
      ans = ans * mod_pow(cnt_km1, cnt_k) % MOD;
    }
  }

  cout << ans % MOD << '\n';
  return 0;
}
