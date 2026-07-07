// AHC055 A - Break the Dungeon
// https://atcoder.jp/contests/ahc055/tasks/ahc055_a
//
// Open treasures in order; use best available weapon (or bare hands) each attack.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int N;
vector<int> hp, cnt;
vector<vector<int>> atk;

void attack(int weapon, int target, vector<pair<int, int>>& ops) {
  if (weapon == -1) {
    hp[target] -= 1;
    ops.emplace_back(-1, target);
  } else {
    hp[target] -= atk[weapon][target];
    cnt[weapon]--;
    ops.emplace_back(weapon, target);
  }
  if (hp[target] <= 0)
    hp[target] = 0;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N;
  hp.resize(N);
  cnt.resize(N);
  for (int i = 0; i < N; ++i)
    cin >> hp[i];
  for (int i = 0; i < N; ++i)
    cin >> cnt[i];
  atk.assign(N, vector<int>(N));
  for (int i = 0; i < N; ++i)
    for (int j = 0; j < N; ++j)
      cin >> atk[i][j];

  vector<char> opened(N, 0);
  vector<pair<int, int>> ops;

  for (int t = 0; t < N; ++t) {
    while (hp[t] > 0) {
      int best = -1;
      int best_dmg = 1;
      for (int w = 0; w < N; ++w) {
        if (!opened[w] || cnt[w] == 0)
          continue;
        if (atk[w][t] > best_dmg) {
          best_dmg = atk[w][t];
          best = w;
        }
      }
      attack(best, t, ops);
    }
    opened[t] = 1;
  }

  for (const auto [w, t] : ops)
    cout << w << ' ' << t << '\n';
  return 0;
}
