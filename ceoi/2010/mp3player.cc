/*
 * Solution for CEOI 2010 Day 2: The MP3 Player.
 *
 * For a fixed lock time T, the final volume is non-decreasing in the initial
 * volume. Sweep critical values of T (gaps between presses) in descending order.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

int n, vmax, target;
vector<char> key_type;
vector<long long> key_time;

vector<char> active_keys(long long lock_time) {
  vector<char> active(n, 0);
  active[0] = 1;
  for (int i = 1; i < n; i++) {
    active[i] = (key_time[i] - key_time[i - 1] <= lock_time);
  }
  return active;
}

int final_volume(int initial, const vector<char>& active) {
  int volume = initial;
  for (int i = 0; i < n; i++) {
    if (!active[i]) {
      continue;
    }
    if (key_type[i] == '+') {
      volume = min(volume + 1, vmax);
    } else {
      volume = max(volume - 1, 0);
    }
  }
  return volume;
}

bool works_for_time(long long lock_time, int& best_initial) {
  auto active = active_keys(lock_time);

  int lo = 0;
  int hi = vmax;
  while (lo < hi) {
    int mid = (lo + hi + 1) / 2;
    if (final_volume(mid, active) <= target) {
      lo = mid;
    } else {
      hi = mid - 1;
    }
  }

  if (final_volume(lo, active) != target) {
    return false;
  }
  best_initial = lo;
  return true;
}

bool is_infinity() {
  for (int initial = 0; initial <= vmax; initial++) {
    bool ok = true;
    vector<long long> critical;
    for (int i = 1; i < n; i++) {
      critical.push_back(key_time[i] - key_time[i - 1]);
    }
    critical.push_back((1LL << 60));
    for (long long lock_time : critical) {
      if (final_volume(initial, active_keys(lock_time)) != target) {
        ok = false;
        break;
      }
    }
    if (ok) {
      return true;
    }
  }
  return false;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n >> vmax >> target;
  key_type.resize(n);
  key_time.resize(n);
  for (int i = 0; i < n; i++) {
    cin >> key_type[i] >> key_time[i];
  }

  if (is_infinity()) {
    cout << "infinity\n";
    return 0;
  }

  vector<long long> critical = {0};
  for (int i = 1; i < n; i++) {
    long long gap = key_time[i] - key_time[i - 1];
    critical.push_back(gap);
    if (gap > 0) {
      critical.push_back(gap - 1);
    }
  }
  sort(critical.begin(), critical.end(), greater<long long>());
  critical.erase(unique(critical.begin(), critical.end()), critical.end());

  long long best_time = 0;
  int best_initial = target;
  for (long long lock_time : critical) {
    int initial = 0;
    if (works_for_time(lock_time, initial)) {
      if (lock_time > best_time || (lock_time == best_time && initial > best_initial)) {
        best_time = lock_time;
        best_initial = initial;
      }
    }
  }

  cout << best_time << ' ' << best_initial << '\n';
  return 0;
}
