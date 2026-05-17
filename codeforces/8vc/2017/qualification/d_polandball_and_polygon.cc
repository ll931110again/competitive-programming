// Codeforces 755 (8VC Venture Cup 2017 - Elimination Round) — D. PolandBall and Polygon
// Submission: https://codeforces.com/contest/755/submission/23866898

#include <bits/stdc++.h>
#define MAXK 2000000
using namespace std;

int tree[5 * MAXK];
long long ret = 1;
int n, k, start = 0;

void add_tree(int i, int low, int high, int pos) {
  if (low == high) {
    tree[i]++;
    return;
  }
  int mid = (low + high) >> 1;
  if (pos <= mid) {
    add_tree(2 * i, low, mid, pos);
  } else {
    add_tree(2 * i + 1, mid + 1, high, pos);
  }
  tree[i] = tree[2 * i] + tree[2 * i + 1];
}

int query_tree(int i, int low, int high, int u, int v) {
  if (v < low || high < u) {
    return 0;
  }
  if (u <= low && high <= v) {
    return tree[i];
  }
  int mid = (low + high) >> 1;
  return query_tree(2 * i, low, mid, u, v) + query_tree(2 * i + 1, mid + 1, high, u, v);
}

void add(int start) {
  add_tree(1, 0, 2 * n - 1, start);
  add_tree(1, 0, 2 * n - 1, start + n);
}

int query(int start, int k) {
  int low = start - (k - 1), high = start + (k - 1);
  if (low < 0) {
    low += n;
    high += n;
  }
  return query_tree(1, 0, 2 * n - 1, low, high);
}

int main() {
  scanf("%d %d", &n, &k);
  if (k + k > n) {
    k = n - k;
  }
  for (int i = 0; i < n; i++) {
    add(start);
    ret += query(start, k);
    start += k;
    if (start >= n) {
      start -= n;
    }
    printf("%lld ", ret);
  }
  printf("\n");
  return 0;
}
