/**
 * Ocarina and Chime — harmonious starts.
 *
 * The idea is that, for a distribution to be uniform
 * we need to check that sum w^j (melody[i] + chime[s+i]) = 0 for j = 1..k-1.
 *
 * By conjugation, it suffices to check that sum w^j (melody[i] + chime[s+i]) = 0 for j = 1..k/2
 * To check it for a single j, we can use FFT to compute the convolution of melody and chime
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

// Ofast enables -fcx-limited-range, which inlines `_Complex double` multiplication as the naive
// (ac-bd, ad+bc) — without this flag, GCC emits a call to libgcc's __muldc3 (NaN/inf fixup) and
// the inner FFT butterfly stays un-inlined / un-vectorized.
#pragma GCC optimize("Ofast,unroll-loops")
// GCC/Clang C99 complex type. With -fcx-limited-range (from Ofast) the compiler inlines a*b as
// the four-mul/two-add formula; without it, the same call goes through __muldc3 like complex.

using cd = _Complex double;

inline cd make_cd(double r, double i) {
  cd z;
  __real__ z = r;
  __imag__ z = i;
  return z;
}

const double PI = acos(-1.0);

inline int mod_k(int x, int k) {
  x %= k;
  return x < 0 ? x + k : x;
}

vector<vector<cd>> tw_fwd, tw_inv;

void build_twiddles(int n) {
  tw_fwd.assign(n + 1, {});
  tw_inv.assign(n + 1, {});
  for (int len = 2; len <= n; len <<= 1) {
    const int half = len / 2;
    tw_fwd[len].resize(half);
    tw_inv[len].resize(half);
    for (int j = 0; j < half; ++j) {
      double ang = 2 * PI * (double)j / (double)len;
      const double c = cos(ang), s = sin(ang);
      tw_fwd[len][j] = make_cd(c, s);
      tw_inv[len][j] = make_cd(c, -s);
    }
  }
}

vector<int> rev_perm;

void fft_inplace(cd* a, int n, bool invert) {
  for (int i = 0; i < n; ++i)
    if (i < rev_perm[i])
      swap(a[i], a[rev_perm[i]]);

  for (int len = 2; len <= n; len <<= 1) {
    const int half = len / 2;
    const cd* tw = invert ? tw_inv[len].data() : tw_fwd[len].data();
    for (int i = 0; i < n; i += len) {
      cd* ai = a + i;
      for (int j = 0; j < half; ++j) {
        cd u = ai[j];
        cd v = ai[j + half] * tw[j];
        ai[j] = u + v;
        ai[j + half] = u - v;
      }
    }
  }

  if (invert) {
    const double inv_n = 1.0 / (double)n;
    for (int i = 0; i < n; ++i)
      a[i] *= inv_n;
  }
}

void convolution_complex(int n, cd* a, cd* b) {
  fft_inplace(a, n, false);
  fft_inplace(b, n, false);
  for (int i = 0; i < n; ++i)
    a[i] *= b[i];
  fft_inplace(a, n, true);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int M, N, k;
  cin >> M >> N >> k;
  vector<int> chime(M), melody(N);
  for (int i = 0; i < M; i++)
    cin >> chime[i];
  for (int i = 0; i < N; i++)
    cin >> melody[i];

  if (N > M) {
    cout << "0\n\n";
    return 0;
  }

  if (k == 1) {
    const int cnt = M - N + 1;
    cout << cnt << '\n';
    for (int i = 0; i < cnt; i++) {
      if (i)
        cout << ' ';
      cout << (i + 1);
    }
    cout << '\n';
    return 0;
  }

  if (N % k != 0) {
    cout << "0\n\n";
    return 0;
  }

  for (int& x : melody)
    x = mod_k(x, k);
  for (int& x : chime)
    x = mod_k(x, k);

  // Precomputed ω^{jr} (one 16-byte load per fill).
  vector<vector<cd>> W(k, vector<cd>(k));
  for (int j = 1; j < k; ++j) {
    for (int r = 0; r < k; ++r) {
      const double ang = 2 * PI * (double)j * (double)r / (double)k;
      W[j][r] = make_cd(cos(ang), sin(ang));
    }
  }

  int fft_len = 1;
  while (fft_len < M + N)
    fft_len <<= 1;

  // Double FFT: reject shift if |coeff|^2 > conv_tol2 (compare squared to skip sqrt).
  const double conv_tol = max(1e-7, 5e-13 * (double)N * max(1.0, log2((double)fft_len)));
  const double conv_tol2 = conv_tol * conv_tol;

  build_twiddles(fft_len);
  {
    const int lg = __builtin_ctz(fft_len);
    rev_perm.resize(fft_len);
    for (int i = 0; i < fft_len; ++i)
      rev_perm[i] = (rev_perm[i >> 1] >> 1) | ((i & 1) << (lg - 1));
  }

  const int S = M - N + 1;
  const int J = k / 2;
  const int off = N - 1;

  // FFT over j: coefficient s + (N-1) is sum_i ω^{j(melody[i]+chime[s+i])}; uniform iff all vanish.
  vector<char> cand(S, 1);
  int alive = S;
  vector<cd> a_fft(fft_len), b_fft(fft_len);
  cd* ap = a_fft.data();
  cd* bp = b_fft.data();

  for (int j = 1; j <= J && alive > 0; ++j) {
    // FFT scrambles whole buffers; full reset each round (memset is fine: all-zero bits = 0.0).
    memset(ap, 0, sizeof(cd) * (size_t)fft_len);
    memset(bp, 0, sizeof(cd) * (size_t)fft_len);
    for (int i = 0; i < N; ++i)
      ap[i] = W[j][melody[N - 1 - i]];
    for (int t = 0; t < M; ++t)
      bp[t] = W[j][chime[t]];

    convolution_complex(fft_len, ap, bp);

    for (int s = 0; s < S && alive > 0; ++s) {
      if (!cand[s])
        continue;
      const cd z = ap[s + off];
      const double zr = __real__ z, zi = __imag__ z;
      if (zr * zr + zi * zi > conv_tol2) {
        cand[s] = 0;
        --alive;
      }
    }
  }

  vector<int> ans;
  ans.reserve(alive);
  for (int s = 0; s < S; ++s) {
    if (cand[s])
      ans.push_back(s + 1);
  }

  cout << ans.size() << '\n';
  if (!ans.empty()) {
    for (size_t i = 0; i < ans.size(); i++) {
      if (i)
        cout << ' ';
      cout << ans[i];
    }
  }
  cout << '\n';
  return 0;
}
