/*
 * Solution for problem J: Pyramids.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 1000005;
#define maxk 7

int n;
bool is_valid[k_max_n][maxk];
int choice[k_max_n][maxk];
int max_height[k_max_n];

vector<pair<int, int>> pyramids;

int high_pyramid(int height) {
  return height * (height + 1) * (2 * height + 1) / 6;
}

int low_pyramid(int height) {
  if (height & 1) {
    int k = (height + 1) / 2;
    return k * (4 * k * k - 1) / 3;
  } else {
    return 4 * high_pyramid(height / 2);
  }
}

string solve(int n) {
  int best_k = -1;
  for (int k = 1; k < maxk; k++)
    if (is_valid[n][k]) {
      best_k = k;
      break;
    }

  if (best_k < 0) {
    return "impossible";
  }

  string ans = "";
  int ts = pyramids.size() - 1;
  while (n > 0 && best_k > 0) {
    while (ts >= 0 && pyramids[ts].first > n) {
      ts--;
    }
    while (ts >= 0 && !is_valid[n - pyramids[ts].first][best_k - 1]) {
      ts--;
    }
    n -= pyramids[ts].first;
    best_k--;

    if (!ans.empty()) {
      ans += ' ';
    }
    int choice = pyramids[ts].second;
    ans += to_string(abs(choice));
    ans += (choice > 0) ? "H" : "L";

    ts--;
  }

  return ans;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  for (int i = 2;; i++) {
    int high = high_pyramid(i);
    if (high >= k_max_n) {
      break;
    }
    pyramids.push_back({high, i});
  }

  for (int i = 3;; i++) {
    int low = low_pyramid(i);
    if (low >= k_max_n) {
      break;
    }
    pyramids.push_back({low, -i});
  }

  sort(pyramids.begin(), pyramids.end());

  for (int i = 1; i < k_max_n; i++) {
    max_height[i] = max_height[i - 1];
    while (max_height[i] < pyramids.size() && pyramids[max_height[i]].first <= i) {
      max_height[i]++;
    }
    max_height[i]--;
    if (max_height[i] < 0) {
      max_height[i] = 0;
    }
  }

  memset(is_valid, false, sizeof is_valid);
  is_valid[0][0] = true;
  choice[0][0] = -1;
  for (int i = 1; i < k_max_n; i++) {
    for (int j = 1; j < maxk; j++) {
      for (int t = 0; t <= max_height[i]; t++) {
        int h = pyramids[t].first;
        if (i < h) {
          break;
        }

        if (is_valid[i - h][j - 1] && t > choice[i - h][j - 1]) {
          is_valid[i][j] = true;
          choice[i][j] = t;
          break;
        }
      }
    }
  }

  for (int i = 1;; i++) {
    cin >> n;
    if (n == 0) {
      break;
    }

    cout << "Case " << i << ": " << solve(n) << endl;
  }
  return 0;
}
