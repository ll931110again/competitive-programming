#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <vector>
#define maxn 200005
using namespace std;

int n, m;
int color[maxn], layer[maxn];

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);

  cin >> n >> m;
  for (int i = 0; i < n; i++) {
    cin >> color[i];
  }
  for (int i = 0; i < n; i++) {
    cin >> layer[i];
  }

  int total_cnt = 0, max_decrement = 0;
  int i = 0;
  while (i < n) {
    int j = i + 1;
    while (j < n && color[j] == color[i]) {
      j++;
    }

    for (int k = i; k + 1 < j; k++) {
      if (layer[k] <= layer[k + 1]) {
        total_cnt++;
      }
    }

    int decrement = 0;
    for (int k = i; k + 1 < j; k++) {
      if (m > 1 && layer[k] <= layer[k + 1]) {
        if (layer[k] != 1 || layer[k + 1] != m) {
          decrement = 1;
        }
      }
    }

    max_decrement = max(max_decrement, decrement);
    i = j;
  }

  cout << total_cnt - max_decrement << endl;

  return 0;
}