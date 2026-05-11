#include <algorithm>
#include <cmath>
#include <complex>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

using cd = complex<double>;
static const double PI = acos(-1.0);

static void fft(vector<cd>& a, bool invert) {
  int n = (int)a.size();

  for (int i = 1, j = 0; i < n; i++) {
    int bit = n >> 1;
    for (; j & bit; bit >>= 1) j ^= bit;
    j ^= bit;
    if (i < j) swap(a[i], a[j]);
  }

  for (int len = 2; len <= n; len <<= 1) {
    double ang = 2 * PI / len * (invert ? -1 : 1);
    cd wlen(cos(ang), sin(ang));
    for (int i = 0; i < n; i += len) {
      cd w(1);
      for (int j = 0; j < len / 2; j++) {
        cd u = a[i + j];
        cd v = a[i + j + len / 2] * w;
        a[i + j] = u + v;
        a[i + j + len / 2] = u - v;
        w *= wlen;
      }
    }
  }

  if (invert) {
    for (cd& x : a) x /= n;
  }
}

static vector<long long> convolution01(const vector<int>& a, const vector<int>& b) {
  int need = (int)a.size() + (int)b.size() - 1;
  int n = 1;
  while (n < need) n <<= 1;

  vector<cd> fa(n), fb(n);
  for (int i = 0; i < (int)a.size(); i++) fa[i] = (double)a[i];
  for (int i = 0; i < (int)b.size(); i++) fb[i] = (double)b[i];

  fft(fa, false);
  fft(fb, false);
  for (int i = 0; i < n; i++) fa[i] *= fb[i];
  fft(fa, true);

  vector<long long> res(need);
  for (int i = 0; i < need; i++) res[i] = (long long)llround(fa[i].real());
  return res;
}

static int idx(char c) {
  if (c == 'A') return 0;
  if (c == 'C') return 1;
  if (c == 'G') return 2;
  return 3; // 'T'
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m, k;
  cin >> n >> m >> k;
  string s, t;
  cin >> s >> t;

  vector<int> needCount(4, 0);
  for (char c : t) needCount[idx(c)]++;

  // good[c][i] = 1 iff there exists p with |p-i| <= k and s[p] == c
  vector<vector<int>> good(4, vector<int>(n, 0));
  for (int c = 0; c < 4; c++) {
    vector<int> pref(n + 1, 0);
    for (int i = 0; i < n; i++) pref[i + 1] = pref[i] + (idx(s[i]) == c);
    for (int i = 0; i < n; i++) {
      int L = max(0, i - k);
      int R = min(n - 1, i + k);
      good[c][i] = (pref[R + 1] - pref[L] > 0) ? 1 : 0;
    }
  }

  vector<char> ok(n - m + 1, 1);

  for (int c = 0; c < 4; c++) {
    vector<int> a(n, 0), b(m, 0);
    for (int i = 0; i < n; i++) a[i] = good[c][i];
    for (int j = 0; j < m; j++) b[m - 1 - j] = (idx(t[j]) == c) ? 1 : 0;

    vector<long long> conv = convolution01(a, b);

    for (int i = 0; i <= n - m; i++) {
      if (!ok[i]) continue;
      long long matches = conv[i + m - 1];
      if (matches != needCount[c]) ok[i] = 0;
    }
  }

  int ans = 0;
  for (char v : ok) ans += (v != 0);
  cout << ans << '\n';
  return 0;
}

