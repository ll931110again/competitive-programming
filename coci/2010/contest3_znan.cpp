// #pragma comment(linker, "/STACK:16777216")

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Rec {
  pair<int, int> v;
  int pos;
} a[1010];

bool cmp(Rec A, Rec B) {
  return A.v < B.v;
}

string s[1010];
int m, n;
int order[1010];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //    freopen("znan.in","r",stdin);
  //    freopen("znan.ou","w",stdout);
  cin >> m >> n;
  for (int i = 0; i < m; i++)
    cin >> s[i];
  memset(order, 0, sizeof(order));
  for (int i = m - 1; i; i--) {
    for (int j = 0; j < n; j++) {
      a[j].v.first = order[j];
      a[j].v.second = s[i][j];
      a[j].pos = j;
    }
    sort(a, a + n, cmp);
    int cnt = -1;
    for (int j = 0; j < n; j++) {
      if (!j || a[j].v > a[j - 1].v)
        cnt++;
      order[a[j].pos] = cnt;
    }
    if (cnt >= n - 1) {
      cout << i << '\n';
      return 0;
    }
  }
  cout << "0\n";
}
