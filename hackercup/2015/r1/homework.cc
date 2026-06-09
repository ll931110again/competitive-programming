#include <bits/stdc++.h>
using namespace std;

#define MAXN 10000000

namespace {

bool is_prime[MAXN + 2];
int primacity[MAXN + 2];
int remains[MAXN + 2];

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  memset(is_prime, true, sizeof(is_prime));
  for (int i = 2; i <= MAXN; i++)
    remains[i] = i;

  for (int i = 2; i <= MAXN; i++)
    if (is_prime[i])
      for (int j = i; j <= MAXN; j += i) {
        is_prime[j] = false;
        primacity[j]++;
        while (remains[j] % i == 0) {
          remains[j] /= i;
        }
      }

  int T;
  cin >> T;
  for (int it = 1; it <= T; it++) {
    int A, B, K;
    cin >> A >> B >> K;
    int result = 0;
    for (int i = A; i <= B; i++)
      if (primacity[i] == K)
        result++;
    cout << "Case #" << it << ": " << result << '\n';
  }
}
