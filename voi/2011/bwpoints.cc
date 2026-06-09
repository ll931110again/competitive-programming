#include <bits/stdc++.h>
using namespace std;

namespace {

int n;
vector<pair<int, int>> v;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  //  freopen("BWPOINTS.INP","r",stdin);
  //  freopen("BWPOINTS.OUT","w",stdout);
  cin >> n;
  for (int i = 0; i < n; i++) {
    int x;
    cin >> x;
    v.push_back(make_pair(x, 0));
  }
  for (int i = 0; i < n; i++) {
    int x;
    cin >> x;
    v.push_back(make_pair(x, 1));
  }
  sort(v.begin(), v.end());
  int col = -1, ret = 0;
  for (int i = 0; i < v.size(); i++)
    if (col < 0 || col == v[i].second)
      col = v[i].second;
    else {
      ret++;
      col = -1;
    }
  cout << ret << '\n';
}
