#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 10000005;
struct Fraction {
  int numerator;
  int denominator;
};

int n;
Fraction fs[k_max_n];

int min_divisor[k_max_n];
int denominator_idx[k_max_n];

int chosen_fractions_idx[k_max_n];
int cnt = 0;

bool cmp_denominator(Fraction x, Fraction y) {
  return x.denominator < y.denominator;
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(0);

  for (int i = 2; i < k_max_n; i++)
    if (!min_divisor[i]) {
      for (int j = i; j < k_max_n; j += i)
        if (!min_divisor[j]) {
          min_divisor[j] = i;
        }
    }

  cin >> n;
  n--;
  for (int i = 0; i < n; i++) {
    string s;
    cin >> s;

    int pos = -1;
    for (int j = 0; j < s.size(); j++)
      if (s[j] == '/') {
        pos = j;
        break;
      }

    int numerator = stoi(s.substr(0, pos));
    int denominator = stoi(s.substr(pos + 1, (int)s.size() - pos - 1));
    fs[i] = {.numerator = numerator, .denominator = denominator};
  }

  sort(fs, fs + n, cmp_denominator);
  memset(denominator_idx, -1, sizeof denominator_idx);
  int pos = 0;

  // First, choose the fraction fs[0].numerator/fs[0].denominator, where
  // fs[0].denominator = 2
  denominator_idx[2] = 0;
  chosen_fractions_idx[0] = 0;
  cnt++;

  while (true) {
    if (fs[pos].numerator % fs[pos].denominator == 0) {
      cout << 1 << endl;
      cout << fs[pos].numerator << "/" << fs[pos].denominator << endl;
      return 0;
    }

    // Then, choose the fraction with the smallest min factor on denominator
    // This is guaranteed to terminate since we will choose all the prime
    // numbers less than equal to N
    int factor = min_divisor[fs[pos].numerator];
    if (denominator_idx[factor] >= 0) {
      cout << cnt - denominator_idx[factor] << endl;
      for (int j = denominator_idx[factor]; j < cnt; j++) {
        int f_idx = chosen_fractions_idx[j];
        cout << fs[f_idx].numerator << "/" << fs[f_idx].denominator;
        cout << " ";
      }
      cout << endl;
      return 0;
    }

    denominator_idx[factor] = cnt;
    pos = factor - 2;
    chosen_fractions_idx[cnt] = pos;
    cnt++;
  }

  return 0;
}
