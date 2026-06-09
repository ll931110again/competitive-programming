// Codeforces 97 (Yandex.Algorithm 2011: Finals) — B. Superset
// Submission: https://codeforces.com/contest/97/submission/263711543

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 10005;
int n;
pair<int, int> pts[k_max_n];
set<pair<int, int>> ans;

void rec(int low, int high) {
  if (low >= high) {
    return;
  }

  int mid = (low + high) / 2;
  int mx = pts[mid].first;
  for (int i = low; i <= high; i++) {
    ans.insert(make_pair(mx, pts[i].second));
  }

  rec(low, mid);
  rec(mid + 1, high);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> pts[i].first >> pts[i].second;
  }
  sort(pts, pts + n);
  for (int i = 0; i < n; i++) {
    ans.insert(pts[i]);
  }
  rec(0, n - 1);

  cout << ans.size() << endl;
  for (auto it : ans) {
    cout << it.first << ' ' << it.second << endl;
  }
  return 0;
}
