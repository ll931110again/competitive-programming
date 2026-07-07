// Local verifier for AHC006 A (matches tools/src/lib.rs rules).

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int kN = 1000;
constexpr int kM = 50;
constexpr int kOffice = 400;

int manhattan(int x1, int y1, int x2, int y2) {
  return abs(x1 - x2) + abs(y1 - y2);
}

}  // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<pair<int, int>> from(kN), to(kN);
  for (int i = 0; i < kN; ++i) {
    cin >> from[i].first >> from[i].second >> to[i].first >> to[i].second;
  }

  int m;
  cin >> m;
  vector<int> chosen(m);
  for (int i = 0; i < m; ++i)
    cin >> chosen[i];
  for (int& id : chosen)
    --id;

  int n;
  cin >> n;
  vector<pair<int, int>> path(n);
  for (int i = 0; i < n; ++i)
    cin >> path[i].first >> path[i].second;

  long long time = 0;
  for (int i = 1; i < n; ++i)
    time += manhattan(path[i - 1].first, path[i - 1].second, path[i].first,
                      path[i].second);

  auto fail = [&](const string& msg) {
    cout << msg << " time=" << time << " score=0\n";
    return 1;
  };

  if (m != kM)
    return fail("m != 50");
  for (int i = 0; i < m; ++i) {
    if (chosen[i] < 0 || chosen[i] >= kN)
      return fail("bad order id");
    for (int j = 0; j < i; ++j)
      if (chosen[i] == chosen[j])
        return fail("duplicate order");
  }
  for (const auto& [x, y] : path) {
    if (x < 0 || x > 800 || y < 0 || y > 800)
      return fail("coord out of range");
  }
  if (n == 0 || path.front() != pair<int, int>{kOffice, kOffice} ||
      path.back() != pair<int, int>{kOffice, kOffice})
    return fail("route must start/end at office");

  map<pair<int, int>, int> first_visit, last_visit;
  for (int i = 0; i < n; ++i) {
    if (!first_visit.count(path[i]))
      first_visit[path[i]] = i;
    last_visit[path[i]] = i;
  }

  for (int id : chosen) {
    const auto pick = from[id];
    const auto drop = to[id];
    if (!first_visit.count(pick) || !last_visit.count(drop))
      return fail("delivery incomplete");
    if (first_visit[pick] >= last_visit[drop])
      return fail("pickup after delivery");
  }

  const long long score = llround(1e8 / (1000.0 + time));
  cout << "T=" << time << " score=" << score << '\n';
  return 0;
}
