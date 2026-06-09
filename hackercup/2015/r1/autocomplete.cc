#include <bits/stdc++.h>
using namespace std;

#define MAXK 1000005

namespace {

vector<int> trie[MAXK];
int T, N, K;

void solve_case(int it) {
  K = 2;
  trie[1] = vector<int>(26);
  cin >> N;
  int result = 0;

  for (int i = 0; i < N; i++) {
    string input;
    bool is_typed = false;
    cin >> input;

    int root = 1;
    for (int j = 0; j < input.size(); j++) {
      int trie_node = trie[root][input[j] - 'a'];
      if (trie_node <= 0 && !is_typed) {
        result += j + 1;
        is_typed = true;
      }
      if (trie_node <= 0) {
        trie[K] = vector<int>(26);
        trie[root][input[j] - 'a'] = K++;
      }
      root = trie[root][input[j] - 'a'];
    }

    if (!is_typed)
      result += input.size();
  }
  cout << "Case #" << it << ": " << result << '\n';
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> T;
  for (int it = 1; it <= T; it++)
    solve_case(it);
}
