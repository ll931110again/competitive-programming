#include <algorithm>
#include <iostream>
#include <map>
#include <queue>
#include <vector>

using namespace std;

static bool feasible_min_len(const map<int, int>& cnt, int k) {
  if (k <= 1)
    return true;

  priority_queue<int, vector<int>, greater<int>> heap; // lengths ending at prev value
  int prev = 0;
  bool has_prev = false;

  for (auto [v, c] : cnt) {
    if (!has_prev) {
      has_prev = true;
    } else if (v != prev + 1) {
      // Gap: all existing sequences must be complete.
      while (!heap.empty()) {
        if (heap.top() < k)
          return false;
        heap.pop();
      }
    }

    // Extend as many existing sequences as possible, prioritizing shortest.
    priority_queue<int, vector<int>, greater<int>> next_heap;

    int extend = min<int>(c, (int)heap.size());
    for (int i = 0; i < extend; i++) {
      int len = heap.top();
      heap.pop();
      next_heap.push(len + 1);
    }

    // Any sequences that couldn't be extended end at prev; must be complete.
    while (!heap.empty()) {
      if (heap.top() < k)
        return false;
      heap.pop();
    }

    // Start new sequences with remaining cards.
    int rem = c - extend;
    for (int i = 0; i < rem; i++)
      next_heap.push(1);

    heap.swap(next_heap);
    prev = v;
  }

  // All remaining sequences must be complete.
  while (!heap.empty()) {
    if (heap.top() < k)
      return false;
    heap.pop();
  }
  return true;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int N;
    cin >> N;
    map<int, int> cnt;
    for (int i = 0; i < N; i++) {
      int x;
      cin >> x;
      cnt[x]++;
    }

    int ans = 0;
    if (N > 0) {
      int lo = 1, hi = N;
      while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (feasible_min_len(cnt, mid)) {
          ans = mid;
          lo = mid + 1;
        } else {
          hi = mid - 1;
        }
      }
    }

    cout << "Case #" << tc << ": " << ans << "\n";
  }
  return 0;
}
