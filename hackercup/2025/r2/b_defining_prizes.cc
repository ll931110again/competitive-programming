
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int k_max_n = 1000005;
int T;
int n, m;
int scores[k_max_n], stockpile[k_max_n], a[k_max_n], cnt_a;

vector<int> score_bucket;
map<int, int> score_cnt;
int prefix[k_max_n];

bool get(int mid) {
  if (m < mid) {
    return false;
  }

  i64 balance = 0;
  for (int i = mid; i < m; i++) {
    balance += stockpile[i];
  }

  int ss = 0;
  for (int i = 0; i < mid; i++) {
    ss += score_bucket[i];
    balance += stockpile[mid - i - 1];
    balance -= ss;
    if (balance < 0) {
      return false;
    }
  }

  return true;
}

int solve() {
  int low = 1, high = score_bucket.size(), ans = 0;
  while (low <= high) {
    int mid = (low + high) / 2;
    if (get(mid)) {
      ans = mid;
      low = mid + 1;
    } else {
      high = mid - 1;
    }
  }

  int output = 0;
  for (int i = 0; i < ans; i++) {
    output += score_bucket[i];
  }
  return output;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  cin >> T;
  for (int it = 1; it <= T; it++) {
    cerr << it << endl;
    cin >> n >> m;
    for (int i = 0; i < n; i++) {
      cin >> scores[i];
    }
    sort(scores, scores + n);
    reverse(scores, scores + n);

    score_bucket.clear();
    for (int i = 0; i < n;) {
      int j = i + 1;
      while (j < n && scores[j] == scores[i]) {
        j++;
      }
      score_bucket.push_back(j - i);
      i = j;
    }

    for (int i = 0; i < m; i++) {
      cin >> stockpile[i];
    }
    sort(stockpile, stockpile + m);
    reverse(stockpile, stockpile + m);

    cout << "Case #" << it << ": " << solve() << endl;
  }

  return 0;
}
