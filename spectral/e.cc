#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

int T, n;

void initial_insert(long long a) {
  cout << a << endl;
  cout << flush;
}

int insert(long long x) {
  cout << "I " << x << endl;
  cout << flush;

  int cnt;
  cin >> cnt;
  return cnt;
}

int query(long long y) {
  cout << "Q " << y << endl;
  cout << flush;

  int cnt;
  cin >> cnt;
  return cnt;
}

void answer(int k, long long c) {
  cout << "A " << k << ' ' << c << endl;
  cout << flush;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

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
