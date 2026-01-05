/*
 * Solution for problem J: Pyramids.
 */

#ifdef ONLINE_JUDGE
  #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <unordered_set>
#include <vector>
#define maxn 1000005
#define maxk 7
using namespace std;

int n;
bool is_valid[maxn][maxk];
int choice[maxn][maxk];
int max_height[maxn];

vector< pair<int,int> > pyramids;

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
  for (int k = 1; k < maxk; k++) if (is_valid[n][k]) {
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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  for (int i = 2; ; i++) {
    int high = high_pyramid(i);
    if (high >= maxn) {
      break;
    }
    pyramids.push_back({high, i});
  }

  for (int i = 3; ; i++) {
    int low = low_pyramid(i);
    if (low >= maxn) {
      break;
    }
    pyramids.push_back({low, -i});
  }

  sort(pyramids.begin(), pyramids.end());

  for (int i = 1; i < maxn; i++) {
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
  for (int i = 1; i < maxn; i++) {
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

  for (int i = 1; ; i++) {
    cin >> n;
    if (n == 0) {
      break;
    }

    cout << "Case " << i << ": " << solve(n) << endl;
  }
  return 0;
}
