#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

struct SegTreeMin {
  int n;
  int INF;
  vector<int> t;
  explicit SegTreeMin(int n_, int inf) : n(n_), INF(inf), t(2 * n_, inf) {}
  void set_val(int pos, int val) {
    pos += n;
    t[pos] = val;
    for (pos >>= 1; pos; pos >>= 1)
      t[pos] = min(t[pos << 1], t[pos << 1 | 1]);
  }
  int query(int l, int r) const { // inclusive
    if (l > r)
      return INF;
    l += n;
    r += n;
    int res = INF;
    while (l <= r) {
      if (l & 1)
        res = min(res, t[l++]);
      if (!(r & 1))
        res = min(res, t[r--]);
      l >>= 1;
      r >>= 1;
    }
    return res;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int N;
    cin >> N;
    vector<int> order(N);
    for (int i = 0; i < N; i++)
      cin >> order[i];

    vector<int> rem_pos(N), rem_val(N);
    for (int i = 0; i < N; i++)
      rem_pos[i] = rem_val[i] = i + 1;

    const int INF = 1e9;
    SegTreeMin seg(N + 1, INF); // values are 1..N, we'll ignore index 0
    vector<int> ans(N + 1, 0);

    for (int step = 0; step < N; step++) {
      int p = order[step];

      int m = (int)rem_pos.size();
      int k = (int)(lower_bound(rem_pos.begin(), rem_pos.end(), p) - rem_pos.begin());

      int chosen = -1;
      if (k < m - 1) {
        chosen = rem_val[k + 1];
      } else {
        if (m == 1) {
          chosen = rem_val[0];
        } else {
          int a = rem_val[m - 2]; // second largest remaining value
          int b = rem_val[m - 1]; // largest remaining value

          bool forced_b = (m <= 2);
          if (!forced_b && a + 1 <= b - 1) {
            int min_pos_in_gap = seg.query(a + 1, b - 1);
            if (min_pos_in_gap < p)
              forced_b = true;
          }

          // For lexicographically greatest deck, pick b only if forced.
          chosen = forced_b ? b : a;
        }
      }

      ans[p] = chosen;
      seg.set_val(chosen, p);

      // erase p and chosen from remaining sets
      rem_pos.erase(rem_pos.begin() + k);
      int iv = (int)(lower_bound(rem_val.begin(), rem_val.end(), chosen) - rem_val.begin());
      rem_val.erase(rem_val.begin() + iv);
    }

    cout << "Case #" << tc << ":";
    for (int i = 1; i <= N; i++)
      cout << " " << ans[i];
    cout << "\n";
  }
  return 0;
}
