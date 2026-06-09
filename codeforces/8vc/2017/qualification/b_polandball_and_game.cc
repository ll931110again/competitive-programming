// Codeforces 755 (8VC Venture Cup 2017 - Elimination Round) — B. PolandBall and Game
// Submission: https://codeforces.com/contest/755/submission/23848984

#include <bits/stdc++.h>
using namespace std;

namespace {

int n, m;
map<string, int> mp;
int acount, bcount, shared;
string input;

bool win() {
  int turn = 0;
  while (true) {
    if (turn == 0) {
      if (!acount && !shared)
        return false;
      if (shared)
        shared--;
      else
        acount--;
    } else {
      if (!bcount && !shared)
        return true;
      if (shared)
        shared--;
      else
        bcount--;
    }
    turn ^= 1;
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> m;
  for (int i = 0; i < n; i++) {
    cin >> input;
    mp[input]++;
  }
  for (int i = 0; i < m; i++) {
    cin >> input;
    if (mp[input] > 0) {
      shared++;
    }
  }
  acount = n - shared;
  bcount = m - shared;
  if (win()) {
    printf("YES\n");
  } else {
    printf("NO\n");
  }
  return 0;
}
