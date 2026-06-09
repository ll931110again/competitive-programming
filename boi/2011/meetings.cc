/*
 * Solution for BOI 2011 Day 1: Meetings.
 *
 * Try every divisor count k and split N into k nearly equal factors.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

long long power(long long base, int exponent) {
  long long result = 1;
  for (int i = 0; i < exponent; i++) {
    result *= base;
  }
  return result;
}

long long best_time(long long people, int prep_time, int vote_time, int divisors) {
  long long base = static_cast<long long>(pow(people, 1.0 / divisors));
  int increased = 0;
  while (power(base + 1, increased) * power(base, divisors - increased) < people) {
    increased++;
  }
  return (static_cast<long long>(divisors) * base + increased) * prep_time +
         static_cast<long long>(divisors) * vote_time;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  long long people;
  int prep_time;
  int vote_time;
  cin >> people >> prep_time >> vote_time;

  if (people == 1) {
    cout << 0 << '\n';
    return 0;
  }

  long long answer = best_time(people, prep_time, vote_time, 1);
  for (int divisors = 2; (2LL << divisors) <= people; divisors++) {
    answer = min(answer, best_time(people, prep_time, vote_time, divisors));
  }

  cout << answer << '\n';
  return 0;
}
