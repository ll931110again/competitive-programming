#include <bits/stdc++.h>
using namespace std;

namespace {

long long n;
long long fibo[90];
int cnt;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //    freopen("hrpa.in","r",stdin);
  //    freopen("hrpa.ou","w",stdout);
  cin >> n;
  fibo[0] = fibo[1] = 1;
  for (int i = 2;; i++) {
    fibo[i] = fibo[i - 1] + fibo[i - 2];
    if (fibo[i] >= n) {
      cnt = i;
      break;
    }
  }
  long long ret = n;
  for (int i = cnt; i >= 0; i--)
    if (n >= fibo[i]) {
      n -= fibo[i];
      ret = fibo[i];
    }
  cout << ret << endl;
}
