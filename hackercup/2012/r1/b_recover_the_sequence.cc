#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 10005;
#define mod 1000003

int T, n, last;
string s;
int arr[k_max_n], temp[k_max_n], opt[k_max_n];

void merge_sort(int low, int high) {
  if (high - low <= 1)
    return;
  int mid = (low + high) / 2;
  merge_sort(low, mid);
  merge_sort(mid, high);

  int la = low, lb = mid, pos = low;
  while (mid - la > 0 && high - lb > 0) {
    if (s[last] == '1') {
      temp[pos] = arr[la];
      la++;
    } else {
      temp[pos] = arr[lb];
      lb++;
    }
    pos++;
    last++;
  }
  for (int i = la; i < mid; i++) {
    temp[pos] = arr[i];
    pos++;
  }
  for (int i = lb; i < high; i++) {
    temp[pos] = arr[i];
    pos++;
  }
  for (int i = low; i < high; i++)
    arr[i] = temp[i];
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("seq.in.1", "r", stdin);
  freopen("seq.out.1", "w", stdout);
  cin >> T;
  for (int it = 1; it <= T; it++) {
    cin >> n;
    cin >> s;
    for (int i = 0; i < n; i++)
      arr[i] = i;
    last = 0;
    merge_sort(0, n);
    for (int i = 0; i < n; i++)
      opt[arr[i]] = i;
    long long ret = 1;
    for (int i = 0; i < n; i++)
      ret = (ret * 31 + opt[i] + 1) % mod;
    cout << "Case #" << it << ": " << ret << '\n';
  }
}
