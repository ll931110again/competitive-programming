// Codeforces 1842 (CodeTON Round 5 (Div. 1 + Div. 2, Rated, Prizes!)) — B. Tenzing and Books
// Submission: https://codeforces.com/contest/1842/submission/210977526

#include <iostream>
#define maxn 100005
using namespace std;

int T, n, x;
int a[3][maxn];

int main() {
  cin >> T;
  while (T--) {
    cin >> n >> x;
    for (int it = 0; it < 3; it++) {
      for (int i = 0; i < n; i++) {
        cin >> a[it][i];
      }
    }

    int idx[3];
    idx[0] = idx[1] = idx[2] = 0;

    int gap = 0;
    for (int i = 0; i < 3; i++) {
      while (idx[i] < n) {
        int tmp = a[i][idx[i]] | gap;
        if ((x & tmp) == tmp) {
          gap = tmp;
          idx[i]++;
        } else {
          break;
        }
      }
    }

    if (x == gap) {
      cout << "Yes" << endl;
    } else {
      cout << "No" << endl;
    }
  }
  return 0;
}
