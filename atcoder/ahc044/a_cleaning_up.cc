// AHC044 A - Cleaning Up: https://atcoder.jp/contests/ahc044/tasks/ahc044_a
//
// Hill-climb on (a_i, b_i) to match 500k-week cleaning counts to targets.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

int N, L;
vector<int> Tgt;
mt19937 rng(712367);

vector<int> simulate(const vector<int>& a, const vector<int>& b) {
  vector<int> cnt(N);
  int cur = 0;
  for (int w = 0; w < L; w++) {
    cnt[cur]++;
    if (w + 1 < L) {
      const int t = cnt[cur];
      cur = (t & 1) ? a[cur] : b[cur];
    }
  }
  return cnt;
}

i64 err(const vector<int>& cnt) {
  i64 s = 0;
  for (int i = 0; i < N; i++)
    s += llabs(cnt[i] - Tgt[i]);
  return s;
}


}  // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> L;
  Tgt.resize(N);
  for (int i = 0; i < N; i++)
    cin >> Tgt[i];

  vector<int> a(N), b(N);
  for (int i = 0; i < N; i++) {
    a[i] = (i + 1) % N;
    b[i] = (i + 2) % N;
  }

  vector<int> best_a = a, best_b = b;
  i64 best = err(simulate(a, b));

  const auto deadline = chrono::steady_clock::now() + chrono::milliseconds(1800);
  while (chrono::steady_clock::now() < deadline) {
    vector<int> na = best_a, nb = best_b;
    const int i = uniform_int_distribution<int>(0, N - 1)(rng);
    const int which = uniform_int_distribution<int>(0, 2)(rng);
    const int v = uniform_int_distribution<int>(0, N - 1)(rng);
    if (which == 0)
      na[i] = v;
    else if (which == 1)
      nb[i] = v;
    else {
      na[i] = v;
      nb[i] = uniform_int_distribution<int>(0, N - 1)(rng);
    }
    const i64 e = err(simulate(na, nb));
    if (e < best) {
      best = e;
      best_a = move(na);
      best_b = move(nb);
    }
  }

  for (int i = 0; i < N; i++)
    cout << best_a[i] << ' ' << best_b[i] << '\n';
  return 0;
}
