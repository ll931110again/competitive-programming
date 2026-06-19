// Codeforces 82 (Yandex.Algorithm 2011: Qualification 2) — A. Double Cola
// Submission: https://codeforces.com/contest/82/submission/434629

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

string s[5] = {"Sheldon", "Leonard", "Penny", "Rajesh", "Howard"};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  i64 T = 1;
  while (1) {
    for (int iter = 0; iter < 5; iter++)
      if (n <= T) {
        cout << s[iter] << endl;
        return 0;
      } else
        n -= T;
    T *= 2LL;
  }
}
