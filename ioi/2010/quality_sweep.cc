// IOI 2010 Day 1 – Quality of Living (quality)
//
// Binary search on the answer; each check slides an H×W window in O(R*C).

#include <bits/stdc++.h>
using namespace std;

namespace {

char good[3001][3001];
int col_cnt[3001];

bool scan_row(int c, int w, int need) {
  int win = 0;
  for (int j = 0; j < w; ++j) {
    win += col_cnt[j];
  }
  if (win >= need) {
    return true;
  }
  for (int left = 1; left <= c - w; ++left) {
    win += col_cnt[left + w - 1] - col_cnt[left - 1];
    if (win >= need) {
      return true;
    }
  }
  return false;
}

bool feasible(int r, int c, int h, int w, int mid, int need, int q[3001][3001]) {
  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      good[i][j] = q[i][j] <= mid;
    }
  }

  for (int j = 0; j < c; ++j) {
    int sum = 0;
    for (int i = 0; i < h; ++i) {
      sum += good[i][j];
    }
    col_cnt[j] = sum;
  }
  if (scan_row(c, w, need)) {
    return true;
  }

  for (int top = 1; top <= r - h; ++top) {
    const int out = top - 1;
    const int in = top + h - 1;
    for (int j = 0; j < c; ++j) {
      col_cnt[j] += good[in][j] - good[out][j];
    }
    if (scan_row(c, w, need)) {
      return true;
    }
  }

  return false;
}

int rectangle(int r, int c, int h, int w, int q[3001][3001]) {
  const int need = (h * w + 1) / 2;
  int lo = 1, hi = r * c;
  while (lo < hi) {
    const int mid = (lo + hi) >> 1;
    if (feasible(r, c, h, w, mid, need, q)) {
      hi = mid;
    } else {
      lo = mid + 1;
    }
  }
  return lo;
}

} // namespace
