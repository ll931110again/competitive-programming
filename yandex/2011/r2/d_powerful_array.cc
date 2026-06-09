// Codeforces 86 (Yandex.Algorithm 2011: Round 2) — D. Powerful array
// Submission: https://codeforces.com/contest/86/submission/216531011

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 200005;
constexpr int k_max_v = 1000005;
struct Query {
  int x, y, z;
  Query(int _x, int _y, int _z) {
    x = _x;
    y = _y;
    z = _z;
  }
};

bool cmp(Query A, Query B) {
  return A.y < B.y;
}

int n, Q, block;
int a[k_max_n], cnt[k_max_v];
vector<Query> bucket[k_max_n];
long long ret[k_max_n];
long long sum;

void modify(int pos, int delta) {
  long long prev = 1LL * cnt[pos] * cnt[pos] * pos;
  cnt[pos] += delta;
  long long next = 1LL * cnt[pos] * cnt[pos] * pos;
  sum += (next - prev);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  // freopen("d.in","r",stdin);

  cin >> n >> Q;
  block = (int)floor(sqrt(n));
  if (!block)
    block = 1;
  for (int i = 0; i < n; i++)
    cin >> a[i];

  for (int i = 0; i < Q; i++) {
    int u, v;
    cin >> u >> v;
    u--;
    v--;
    bucket[u / block].push_back(Query(u, v, i));
  }
  memset(cnt, 0, sizeof(cnt));

  for (int it = 0; it < n; it++)
    if (!bucket[it].empty()) {
      sum = 0;
      sort(bucket[it].begin(), bucket[it].end(), cmp);
      for (int i = bucket[it][0].x; i <= bucket[it][0].y; i++)
        modify(a[i], 1);
      ret[bucket[it][0].z] = sum;
      for (int j = 1; j < bucket[it].size(); j++) {
        for (int i = bucket[it][j - 1].y + 1; i <= bucket[it][j].y; i++)
          modify(a[i], 1);
        if (bucket[it][j - 1].x > bucket[it][j].x)
          for (int i = bucket[it][j - 1].x - 1; i >= bucket[it][j].x; i--)
            modify(a[i], 1);
        else
          for (int i = bucket[it][j - 1].x; i < bucket[it][j].x; i++)
            modify(a[i], -1);
        ret[bucket[it][j].z] = sum;
      }
      int sz = bucket[it].size() - 1;
      for (int i = bucket[it][sz].x; i <= bucket[it][sz].y; i++)
        modify(a[i], -1);
    }

  for (int i = 0; i < Q; i++)
    cout << ret[i] << '\n';
}
