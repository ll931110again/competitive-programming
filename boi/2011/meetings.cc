/*
 * Solution for BOI 2011 Day 1: Meetings.
 *
 * Try every divisor count k and split N into k nearly equal factors.
 */

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

i64 power(i64 base, int exponent) {
  i64 result = 1;
  for (int i = 0; i < exponent; i++) {
    result *= base;
  }
  return result;
}

i64 best_time(i64 people, int prep_time, int vote_time, int divisors) {
  i64 base = static_cast<i64>(pow(people, 1.0 / divisors));
  int increased = 0;
  while (power(base + 1, increased) * power(base, divisors - increased) < people) {
    increased++;
  }
  return (static_cast<i64>(divisors) * base + increased) * prep_time +
         static_cast<i64>(divisors) * vote_time;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  i64 people;
  int prep_time;
  int vote_time;
  cin >> people >> prep_time >> vote_time;

  if (people == 1) {
    cout << 0 << '\n';
    return 0;
  }

  i64 answer = best_time(people, prep_time, vote_time, 1);
  for (int divisors = 2; (2LL << divisors) <= people; divisors++) {
    answer = min(answer, best_time(people, prep_time, vote_time, divisors));
  }

  cout << answer << '\n';
  return 0;
}
