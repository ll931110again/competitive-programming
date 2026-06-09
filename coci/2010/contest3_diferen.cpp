// #pragma comment(linker, "/STACK:16777216")

#include <bits/stdc++.h>
using namespace std;

namespace {

long long f1[300010], f2[300010];
int a[300010];
int n;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //    freopen("diff.in","r",stdin);
  //    freopen("diff.ou","w",stdout);
  cin >> n;
  for (int i = 1; i <= n; i++)
    cin >> a[i];
  stack<int> s;
  f1[0] = 0;
  for (int i = 1; i <= n; i++) {
    while (!s.empty() && a[s.top()] >= a[i])
      s.pop();
    if (s.empty())
      f1[i] = 1LL * i * a[i];
    else
      f1[i] = f1[s.top()] + 1LL * (i - s.top()) * a[i];
    s.push(i);
  }
  while (!s.empty())
    s.pop();
  f2[0] = 0;
  for (int i = 1; i <= n; i++) {
    while (!s.empty() && a[s.top()] <= a[i])
      s.pop();
    if (s.empty())
      f2[i] = 1LL * i * a[i];
    else
      f2[i] = f2[s.top()] + 1LL * (i - s.top()) * a[i];
    s.push(i);
  }
  long long ret = 0;
  for (int i = 1; i <= n; i++)
    ret += (f2[i] - f1[i]);
  cout << ret << endl;
}
