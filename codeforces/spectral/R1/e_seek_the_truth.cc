// Codeforces Spectral::Cup 2026 Round 1 — Seek the Truth
// https://codeforces.com/contest/2222/problem/E
//
// Sketch
// ------
// Interactive. Distinguish AND / OR / XOR of a hidden mask by inserting 0 and
// the all-ones value, then recover bits by inserting single-bit probes.

#include <bits/stdc++.h>
using namespace std;

namespace {

int T, n;

void initial_insert(int64_t a) {
  cout << a << endl;
  cout << flush;
}

int insert(int64_t x) {
  cout << "I " << x << endl;
  cout << flush;

  int cnt;
  cin >> cnt;
  return cnt;
}

int query(int64_t y) {
  cout << "Q " << y << endl;
  cout << flush;

  int cnt;
  cin >> cnt;
  return cnt;
}

void answer(int k, int64_t c) {
  cout << "A " << k << ' ' << c << endl;
  cout << flush;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  while (T--) {
    cin >> n;

    initial_insert(0);

    insert(0);
    int cnt = query(1);
    int ans_k = -1, ans_c = -1;

    if (cnt == 0) {
      ans_k = 1; // and operation
    } else {
      insert((1LL << n) - 1);
      cnt = query((1LL << n) - 1);
      if (cnt == 0) {
        ans_k = 2; // or operation
      } else {
        ans_k = 3; // xor operation
      }
    }

    ans_c = 0;
    if (ans_k == 1) {
      int prev_cnt = cnt;
      for (int i = 0; i < n; i++) {
        cnt = insert(1LL << i);
        if (prev_cnt < cnt) {
          ans_c |= (1LL << i);
        }
        prev_cnt = cnt;
      }
    } else if (ans_k == 2) {
      int prev_cnt = cnt;
      ans_c = (1LL << n) - 1;
      for (int i = 0; i < n; i++) {
        cnt = insert(1LL << i);
        if (prev_cnt < cnt) {
          ans_c ^= (1LL << i);
        }
        prev_cnt = cnt;
      }
    } else {
    }

    answer(ans_k, ans_c);
  }

  return 0;
}
