// N – Olympic Ceremony (interactive)
// https://open.kattis.com/problems/olympicceremony
//
// Districts form contiguous blocks in the original line: "? l r" returns 1 iff all
// athletes l..r share one district. Discover maximal blocks by binary search.

#include <bits/stdc++.h>
using namespace std;

static int ask(int l, int r) {
  cout << "? " << l << ' ' << r << '\n';
  cout.flush();
  string tok;
  cin >> tok;
  int k;
  cin >> k;
  return k;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  if (!(cin >> N)) {
    return 0;
  }

  vector<vector<int>> blocks;
  int i = 1;
  while (i <= N) {
    int lo = i, hi = N, best = i;
    while (lo <= hi) {
      int mid = (lo + hi) / 2;
      const int d = ask(i, mid);
      if (d == 1) {
        best = mid;
        lo = mid + 1;
      } else {
        hi = mid - 1;
      }
    }
    vector<int> cur;
    for (int x = i; x <= best; ++x) {
      cur.push_back(x);
    }
    blocks.push_back(move(cur));
    i = best + 1;
  }

  cout << "! ";
  bool first = true;
  for (const auto& blk : blocks) {
    for (int x : blk) {
      if (!first) {
        cout << ' ';
      }
      first = false;
      cout << x;
    }
  }
  cout << '\n';
  cout.flush();
  return 0;
}
