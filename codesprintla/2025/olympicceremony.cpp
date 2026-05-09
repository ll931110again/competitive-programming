// N – Olympic Ceremony (interactive)
// https://open.kattis.com/problems/olympicceremony
//
// Template: binary-search / merge-by-queries pattern depends on full analysis.

#include <bits/stdc++.h>
using namespace std;

int main() {
  ios::sync_with_stdio(false);
  int N;
  if (!(cin >> N))
    return 0;
  // TODO: replace with a correct interactive strategy under 10^4 queries.
  cout << "! ";
  for (int i = 1; i <= N; ++i)
    cout << i << (i == N ? '\n' : ' ');
  cout.flush();
  return 0;
}
