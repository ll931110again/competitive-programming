// GCJ 2010 Round 3, Problem C — Hot Dog Proliferation
//
// Invariants (editorial): sum of positions is fixed; each move increases sum of squares by 2.
// So answer = (sum_sq_final - sum_sq_initial) / 2.
//
// Build sum_sq_final by processing piles left → right (given sorted by P). Each pile becomes a
// stabilized "segment" (full interval or H-segment with one hole). While it overlaps the stack top,
// pop and merge using only (K, total_sum) — Observation 1 uniquely fixes the merged terminal shape.

#include <bits/stdc++.h>
using namespace std;

using i128 = __int128_t;
using i64 = long long;
namespace {

i64 div_floor(i64 a, i64 b) {
  if (b <= 0)
    exit(1);
  if (a >= 0)
    return a / b;
  return -((-a + b - 1) / b);
}

// Sum of squares of n consecutive integers starting at `first`.
i128 sum_sq_consecutive(i64 first, i64 n) {
  if (n <= 0)
    return 0;
  i128 f = first;
  i128 nn = n;
  // sum (f+i)^2 for i=0..n-1 = n*f^2 + 2*f*n*(n-1)/2 + (n-1)n(2n-1)/6
  i128 s1 = nn * f * f;
  i128 s2 = f * nn * (nn - 1);
  i128 s3 = (nn - 1) * nn * (2 * nn - 1) / 6;
  return s1 + s2 + s3;
}

struct Interval {
  i64 lo, hi;
};

struct Seg {
  i64 K = 0;
  i64 S = 0;
  i128 Q = 0;
  vector<Interval> occ;
};

bool intervals_overlap(i64 a1, i64 b1, i64 a2, i64 b2) {
  return a1 <= b2 && a2 <= b1;
}

bool seg_overlap(const Seg& A, const Seg& B) {
  for (const auto& ia : A.occ)
    for (const auto& ib : B.occ)
      if (intervals_overlap(ia.lo, ia.hi, ib.lo, ib.hi))
        return true;
  return false;
}

// Terminal configuration for K chips on Z with total sum S (one component, ≤ one hole).
Seg terminal_from_KS(i64 K, i64 S) {
  Seg g;
  g.K = K;
  g.S = S;
  if (K < 1)
    exit(1);

  if (K == 1) {
    g.Q = (i128)S * S;
    g.occ.push_back({S, S});
    return g;
  }

  // Full consecutive K integers: x, x+1, ..., x+K-1
  {
    i64 num = 2 * S - K * (K - 1);
    i64 den = 2 * K;
    i64 r = num % den;
    if (r < 0)
      r += den;
    if (r == 0) {
      i64 x = num / den;
      if (K * x + K * (K - 1) / 2 == S) {
        i64 hi = x + K - 1;
        g.Q = sum_sq_consecutive(x, K);
        g.occ.push_back({x, hi});
        return g;
      }
    }
  }

  // H-segment: K+1 lattice points x..x+K, exactly one empty at y, x < y ≤ x+K.
  // Sum of all points x..x+K minus y equals S.
  // Bounds from x > (2S - K(K+1)) / (2K) and x ≤ (2K + 2S - K(K+1)) / (2K).
  i64 low = div_floor(2 * S - K * (K + 1), 2 * K) + 1;
  i64 high = div_floor(2 * K + 2 * S - K * (K + 1), 2 * K);
  low -= 3;
  high += 3;

  for (i64 x = low; x <= high; ++x) {
    i128 T = (i128)(K + 1) * (2 * x + K) / 2;
    i128 Sp = S;
    i128 y128 = T - Sp;
    if (y128 < (i128)x + 1 || y128 > (i128)x + K)
      continue;
    i64 y = (i64)y128;
    if ((i128)y != y128)
      continue;

    i128 all_sq = sum_sq_consecutive(x, K + 1);
    i128 ysq = (i128)y * y;
    g.Q = all_sq - ysq;

    if (y > x)
      g.occ.push_back({x, y - 1});
    if (y < x + K)
      g.occ.push_back({y + 1, x + K});

    if (g.occ.empty())
      exit(1);
    return g;
  }

  cerr << "terminal_from_KS failed K=" << K << " S=" << S << "\n";
  exit(1);
}

Seg from_pile(i64 V, i64 P) {
  return terminal_from_KS(V, V * P);
}

Seg merge_seg(const Seg& A, const Seg& B) {
  return terminal_from_KS(A.K + B.K, A.S + B.S);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int C;
    cin >> C;
    vector<pair<i64, i64>> piles(C);
    i128 sq_init = 0;
    for (int i = 0; i < C; i++) {
      cin >> piles[i].first >> piles[i].second;
      i64 P = piles[i].first;
      i64 V = piles[i].second;
      sq_init += (i128)V * (i128)P * (i128)P;
    }

    vector<Seg> st;
    for (const auto& pr : piles) {
      Seg cur = from_pile(pr.second, pr.first);
      // A new pile's stabilized segment can overlap a western segment without overlapping
      // the current stack top (easternmost segment). Merge with every overlapping segment.
      while (true) {
        bool merged = false;
        for (int i = (int)st.size() - 1; i >= 0; --i) {
          if (seg_overlap(st[(size_t)i], cur)) {
            cur = merge_seg(st[(size_t)i], cur);
            st.erase(st.begin() + i);
            merged = true;
            break;
          }
        }
        if (!merged)
          break;
      }
      st.push_back(cur);
    }

    i128 sq_final = 0;
    for (const Seg& s : st)
      sq_final += s.Q;
    i128 moves = (sq_final - sq_init) / 2;

    i64 ans = (i64)moves;
    cout << "Case #" << tc << ": " << ans << "\n";
  }
  return 0;
}
