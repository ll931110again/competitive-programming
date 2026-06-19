// C – Balancing the Council
// https://open.kattis.com/problems/balancingthecouncil
//
// Seat k is at complex ζ^k with ζ = exp(2πi/N). A balanced arrangement of M
// distinct seats has ∑ ζ^{k_i} = 0.
//
// Full proof (complement + Lam–Leung necessity + Sivek sufficiency + even‑N
// appendix): balancingthecouncil_proof.md in this directory.
//
// Primary reference: Gary Sivek, “On Vanishing Sums of Distinct Roots of Unity”,
// Integers 10 (3) (2010), 365–368. DOI: 10.1515/integ.2010.031
// https://doi.org/10.1515/integ.2010.031
//
// - M = 1 or M = N−1: impossible (N−1 complement is a single nonzero root).
// - M even, 2 ≤ M ≤ N: use M/2 disjoint opposite pairs; each pair sums to 0.
// - M odd, 1 < M < N−1: let p be the smallest odd prime dividing N. If N is a
//   power of 2, no such p and odd M is impossible. Otherwise, pick M with
//   p ≤ M ≤ N−p (and such a subset exists). Verified against exhaustive search
//   for all even N ≤ 30.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

i64 smallest_odd_prime_factor(i64 n) {
  while (n % 2 == 0)
    n /= 2;
  if (n == 1)
    return 0; // N was a power of 2
  for (i64 d = 3; d * d <= n; d += 2) {
    if (n % d == 0)
      return d;
  }
  return n;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  i64 N, M;
  if (!(cin >> N >> M))
    return 0;
  if (M == 1 || M == N - 1) {
    cout << "No\n";
    return 0;
  }
  if (M % 2 == 0) {
    cout << "Yes\n";
    return 0;
  }
  // odd M, 1 < M < N−1
  i64 p = smallest_odd_prime_factor(N);
  if (p == 0) {
    cout << "No\n";
    return 0;
  }
  if (M >= p && M <= N - p)
    cout << "Yes\n";
  else
    cout << "No\n";
  return 0;
}
