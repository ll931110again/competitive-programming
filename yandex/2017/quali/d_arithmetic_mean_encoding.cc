#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
#define maxk 60

namespace {

int T;
i64 n;

bool found_solution(i64 n, int k) {
  i64 value = n * k;

  priority_queue<int> pq;
  for (int i = 0; i < maxk; i++)
    if (value & (1LL << i)) {
      pq.push(i);
    }

  if (pq.size() > k) {
    return false;
  }

  while (pq.size() < k) {
    int u = pq.top();
    pq.pop();

    pq.push(u - 1);
    pq.push(u - 1);
  }

  cout << k << ' ';
  while (!pq.empty()) {
    int u = pq.top();
    pq.pop();
    cout << u << ' ';
  }
  cout << endl;
  return true;
}

void solve(i64 n) {
  for (int k = 1;; k++) {
    if (found_solution(n, k)) {
      return;
    }
  }
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  while (T--) {
    cin >> n;
    solve(n);
  }

  return 0;
}
