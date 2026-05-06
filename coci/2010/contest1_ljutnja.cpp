// #pragma comment(linker, "/STACK:16777216")
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>
using namespace std;

int m, n;
int a[100010];

int main() {
  //    freopen("lj.in","r",stdin);
  //    freopen("lj.ou","w",stdout);
  cin >> m >> n;
  for (int i = 1; i <= n; i++)
    cin >> a[i];
  sort(a + 1, a + n + 1);
  for (int i = n; i > 0; i--) {
    int delta = a[i] - a[i - 1], cnt = n - i + 1;
    if (m / cnt >= delta)
      m -= cnt * delta;
    else {
      delta = m / cnt;
      a[i] -= delta;
      m %= cnt;
      long long ret = 0;
      for (int j = 1; j < i; j++)
        ret += 1LL * a[j] * a[j];
      ret += 1LL * m * (a[i] - 1) * (a[i] - 1) + 1LL * (cnt - m) * a[i] * a[i];
      cout << ret << endl;
      return 0;
    }
  }
}
