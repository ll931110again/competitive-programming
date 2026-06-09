#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 1000005;
#define maxk 26
#define max_q 10005

int nxt[k_max_n][maxk];
int mask_value[max_q];
unordered_set<int> in_mask;
unordered_map<int, int> counter;
string s;
int n, Q;

pair<int, int> order[26];
int cnt = 0;

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> s;
  n = s.size();
  memset(nxt, -1, sizeof(nxt));
  for (int i = n - 1; i >= 0; i--)
    for (int j = 0; j < 26; j++)
      nxt[i][j] = (s[i] == j + 'a') ? i : nxt[i + 1][j];
  cin >> Q;
  for (int it = 0; it < Q; it++) {
    string query;
    cin >> query;
    int mask = 0;
    for (int i = 0; i < query.size(); i++)
      mask |= 1 << (query[i] - 'a');
    in_mask.insert(mask);
    mask_value[it] = mask;
  }
  for (int i = 0; i < n; i++) {
    if (i && s[i] == s[i - 1])
      continue;
    cnt = 0;
    for (int j = 0; j < 26; j++)
      if (nxt[i][j] >= 0)
        order[cnt++] = make_pair(nxt[i][j], j);
    sort(order, order + cnt);
    int mask = 0;
    for (int j = 0; j < cnt; j++) {
      int pos = order[j].second;
      if (i && pos + 'a' == s[i - 1])
        break;
      mask |= 1 << pos;
      if (in_mask.find(mask) != in_mask.end())
        counter[mask]++;
    }
  }
  for (int i = 0; i < Q; i++)
    cout << counter[mask_value[i]] << '\n';
}
