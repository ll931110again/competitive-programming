// AHC025 A - Product Packaging: https://atcoder.jp/contests/ahc025/tasks/ahc025_a
//
// Hidden item weights; comparison queries build a total order (cached pairwise
// asks). Greedy assign heaviest-first to the lightest bin to balance loads.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

int N, D, Q;
map<pair<int, int>, char> cmp_cache;

char ask_cmp(int a, int b) {
  if (a == b)
    return '=';
  const pair<int, int> key = minmax(a, b);
  if (cmp_cache.count(key))
    return cmp_cache[key];
  cout << "1 1 " << a << ' ' << b << endl;
  char res;
  cin >> res;
  cmp_cache[key] = res;
  return res;
}

bool less_item(int a, int b) {
  const char c = ask_cmp(a, b);
  return c == '<';
}

vector<int> order_items() {
  vector<int> idx(N);
  iota(idx.begin(), idx.end(), 0);
  sort(idx.begin(), idx.end(), [&](int a, int b) {
    if ((int)cmp_cache.size() >= Q)
      return a < b;
    return less_item(a, b);
  });
  return idx;
}

vector<int> assign_bins(const vector<int>& order) {
  vector<long long> load(D, 0);
  vector<int> ans(N, 0);
  for (int id : order) {
    int best = 0;
    for (int d = 1; d < D; ++d) {
      if (load[d] < load[best])
        best = d;
    }
    ans[id] = best;
    load[best] += 1; // placeholder; real weight unknown without more queries
  }
  return ans;
}

void dummy_queries() {
  while ((int)cmp_cache.size() < Q) {
    const int a = cmp_cache.size() % N;
    const int b = (a + 1) % N;
    ask_cmp(a, b);
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> D >> Q;

  const auto order = order_items();
  while ((int)cmp_cache.size() < Q)
    dummy_queries();

  vector<long long> load(D, 0);
  vector<int> ans(N, 0);
  for (int id : order) {
    int best = 0;
    for (int d = 1; d < D; ++d) {
      if (load[d] < load[best])
        best = d;
    }
    ans[id] = best;
    load[best] += id + 1;
  }

  for (int i = 0; i < N; ++i) {
    if (i)
      cout << ' ';
    cout << ans[i];
  }
  cout << '\n';
  return 0;
}
