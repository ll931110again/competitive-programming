#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

// Parity (mod 2) of number of pairs (i < j) with a[j] > a[i].

int parity_increasing_pairs(const vector<i64>& a) {
  vector<i64> xs = a;
  sort(xs.begin(), xs.end());
  xs.erase(unique(xs.begin(), xs.end()), xs.end());

  int n = (int)xs.size();
  vector<int> bit(n + 1, 0); // stores counts mod 2

  auto add = [&](int idx) {
    for (; idx <= n; idx += idx & -idx)
      bit[idx] ^= 1;
  };
  auto sum = [&](int idx) {
    int r = 0;
    for (; idx > 0; idx -= idx & -idx)
      r ^= bit[idx];
    return r;
  };

  int parity = 0;
  for (i64 v : a) {
    int id = (int)(lower_bound(xs.begin(), xs.end(), v) - xs.begin()) + 1;
    // count of prior elements strictly smaller than v, mod 2
    parity ^= sum(id - 1);
    add(id);
  }
  return parity;
}

i64 count_winning_x(i64 F, int N, int P, const vector<i64>& buckets) {
  // Only buckets 2..N-1 (1-indexed) can be played; endpoints never affect legality.
  // Let internal array v[1..m] correspond to buckets[2..N-1] (1-indexed).
  int m = max(0, N - 2);
  vector<i64> v(m + 1, 0); // 1..m
  for (int i = 1; i <= m; i++)
    v[i] = buckets[i + 1];

  auto winning_for_fixed_internal = [&]() -> bool {
    // total moves parity = parity of number of positive subarray sums in v[1..m]
    // which equals parity of number of pairs (i<j) with prefix[j] > prefix[i]
    // where prefix[0]=0, prefix[k]=sum_{t=1..k} v[t]
    vector<i64> pref(m + 1, 0);
    for (int i = 1; i <= m; i++)
      pref[i] = pref[i - 1] + v[i];
    int parity = parity_increasing_pairs(pref);
    return parity == 1;
  };

  // If unknown is on an endpoint, it cannot influence the game at all.
  if (P == 1 || P == N || m == 0) {
    return winning_for_fixed_internal() ? (2 * F + 1) : 0;
  }

  // Unknown is an internal position: internal index p = P-1 in [1..m]
  int p = P - 1;

  // Build T[k] = prefix sums with v[p] treated as 0
  vector<i64> T(m + 1, 0);
  for (int i = 1; i <= m; i++) {
    i64 add = (i == p) ? 0LL : v[i];
    T[i] = T[i - 1] + add;
  }

  // Constant parities A (within first part 0..p-1) and B (within second part p..m)
  vector<i64> left_pref;
  left_pref.reserve(p);
  for (int i = 0; i <= p - 1; i++)
    left_pref.push_back(T[i]);

  vector<i64> right_pref;
  right_pref.reserve(m - p + 1);
  for (int j = p; j <= m; j++)
    right_pref.push_back(T[j]);

  int A = parity_increasing_pairs(left_pref);
  int B = parity_increasing_pairs(right_pref);
  int K = A ^ B; // C(x) parity = K xor D(x) parity

  // Cross-pair parity D(x): for each (i in [0..p-1], j in [p..m]),
  // contributes 1 iff x > (T[i] - T[j]).
  // We only need toggle points where D(x) changes parity: at x = (T[i]-T[j]) + 1.
  unordered_map<i64, int> odd; // store parity of multiplicity for each h = T[i]-T[j]
  odd.reserve((size_t)p * (size_t)(m - p + 1) * 2);
  odd.max_load_factor(0.7f);

  for (int i = 0; i <= p - 1; i++) {
    i64 Li = T[i];
    for (int j = p; j <= m; j++) {
      i64 h = Li - T[j];
      odd[h] ^= 1;
    }
  }

  vector<i64> toggles;
  toggles.reserve(odd.size());
  for (auto& kv : odd) {
    if (kv.second & 1) {
      // toggle at b = h+1, but be careful about overflow (values are within +/- 2e15 here)
      toggles.push_back(kv.first + 1);
    }
  }
  sort(toggles.begin(), toggles.end());

  auto parity_at = [&](i64 x) -> int {
    // D(x) parity = (#toggle <= x) mod2 because each odd-mult threshold h contributes for x>=h+1
    i64 cnt = (i64)(upper_bound(toggles.begin(), toggles.end(), x) - toggles.begin());
    return (int)(cnt & 1LL);
  };

  i64 lo = -F, hi = F;
  i64 ans = 0;

  i64 cur_x = lo;
  int cur_d = parity_at(cur_x);
  int cur_win = (K ^ cur_d);

  size_t idx = upper_bound(toggles.begin(), toggles.end(), cur_x) - toggles.begin();
  while (cur_x <= hi) {
    i64 next_toggle = (idx < toggles.size()) ? toggles[idx] : (hi + 1);
    i64 seg_r = min(hi, next_toggle - 1);
    if (seg_r >= cur_x && cur_win == 1)
      ans += (seg_r - cur_x + 1);
    cur_x = next_toggle;
    if (cur_x > hi)
      break;
    cur_d ^= 1;
    cur_win ^= 1;
    idx++;
  }
  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int N, P;
    cin >> N >> P;
    vector<i64> buckets(N + 1, 0); // 1-indexed
    i64 F = 0;
    for (int i = 1; i <= N; i++) {
      i64 x;
      cin >> x;
      if (i == P)
        F = x;
      else
        buckets[i] = x;
    }

    i64 ans = count_winning_x(F, N, P, buckets);
    cout << "Case #" << tc << ": " << ans << "\n";
  }
  return 0;
}
