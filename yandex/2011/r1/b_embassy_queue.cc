// Codeforces 85 (Yandex.Algorithm 2011: Round 1) — B. Embassy Queue
// Submission: https://codeforces.com/contest/85/submission/461040

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int num[4], wait[4];
int n;
int a[100010];
priority_queue<i64> q[4];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  // freopen("b.in","r",stdin);
  // freopen("b.ou","w",stdout);

  for (int i = 1; i <= 3; i++)
    cin >> num[i];
  for (int i = 1; i <= 3; i++)
    cin >> wait[i];
  cin >> n;
  for (int i = 0; i < n; i++)
    cin >> a[i];

  for (int i = 1; i <= 3; i++) {
    num[i] = min(num[i], n);
    for (int j = 0; j < num[i]; j++)
      q[i].push(0);
  }

  i64 ret = 0;
  for (int i = 0; i < n; i++) {
    i64 wait_time = a[i];
    for (int j = 1; j <= 3; j++) {
      i64 used = -q[j].top();
      q[j].pop();
      wait_time = max(wait_time, used) + wait[j];
      q[j].push(-wait_time);
    }
    ret = max(ret, wait_time - a[i]);
  }
  cout << ret << endl;
}
