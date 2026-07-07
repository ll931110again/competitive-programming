// AHC029 A - Card Game: https://atcoder.jp/contests/ahc029/tasks/ahc029_a
//
// Interactive card/project management for 1000 turns. Greedy: work the cheapest
// finishable project with the strongest single-target card; buy cheapest card.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

struct Project {
  long long h, v;
};

struct Card {
  int ty;
  long long w;
};

int N, M, K, T;
vector<Card> hand;
vector<Project> projects;
long long money = 0;

int pick_project(const Card& card) {
  if (card.ty == 2 || card.ty == 3)
    return 0;
  int best = 0;
  long long best_rem = LLONG_MAX;
  for (int i = 0; i < M; ++i) {
    if (projects[i].h <= 0)
      continue;
    if (projects[i].h <= card.w && projects[i].h < best_rem) {
      best_rem = projects[i].h;
      best = i;
    }
  }
  for (int i = 0; i < M; ++i) {
    if (projects[i].h > 0 && projects[i].h <= card.w)
      return i;
  }
  return 0;
}

int pick_card() {
  int best = 0;
  for (int i = 0; i < N; ++i) {
    if (hand[i].ty == 0 || hand[i].ty == 2)
      return i;
  }
  return best;
}

int pick_buy(const vector<Card>& cand) {
  int best = 0;
  long long best_p = LLONG_MAX;
  for (int i = 0; i < K; ++i) {
    if (cand[i].p <= money && cand[i].p < best_p) {
      best_p = cand[i].p;
      best = i;
    }
  }
  return best;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> M >> K >> T;
  hand.resize(N);
  for (int i = 0; i < N; ++i)
    cin >> hand[i].ty >> hand[i].w;
  projects.resize(M);
  for (int i = 0; i < M; ++i)
    cin >> projects[i].h >> projects[i].v;

  for (int turn = 0; turn < T; ++turn) {
    const int c = pick_card();
    const int m = pick_project(hand[c]);
    cout << c << ' ' << m << '\n';

    for (int i = 0; i < M; ++i)
      cin >> projects[i].h >> projects[i].v;
    cin >> money;

    vector<Card> cand(K);
    for (int i = 0; i < K; ++i)
      cin >> cand[i].ty >> cand[i].w >> cand[i].p;

    const int r = pick_buy(cand);
    cout << r << '\n';
    hand[c] = cand[r];
  }
  return 0;
}
