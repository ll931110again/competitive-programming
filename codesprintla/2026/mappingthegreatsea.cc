#ifdef ONLINE_JUDGE
#include <bits/stdc++.h>
#endif
#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

struct Rect {
  int r1, r2, c1, c2;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  const int W = (n + 63) >> 6;
  const uint64_t last_word_mask = (n & 63) ? (~0ULL >> (64 - (n & 63))) : ~0ULL;

  vector<uint64_t> rowb((size_t)n * (size_t)W);
  size_t total_ones = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      int x;
      cin >> x;
      if (x) {
        rowb[(size_t)i * (size_t)W + (j >> 6)] |= 1ULL << (j & 63);
        total_ones++;
      }
    }
  }

  // Quick path: entire grid is 1 — single maximal rectangle, every cell exclusive
  if (total_ones == (size_t)n * (size_t)n) {
    cout << "1 1\n";
    return 0;
  }

  const int S = n + 1;
  vector<int> row_ps((size_t)n * (size_t)S, 0);
  int* const rp = row_ps.data();
  for (int i = 0; i < n; i++) {
    const size_t base = (size_t)i * (size_t)S;
    for (int j = 0; j < n; j++) {
      const int v = (int)((rowb[(size_t)i * (size_t)W + (j >> 6)] >> (j & 63)) & 1ULL);
      rp[base + (size_t)j + 1] = rp[base + (size_t)j] + v;
    }
  }

  auto row_seg = [rp, S](int row, int c0, int c1) -> int {
    const size_t b = (size_t)row * (size_t)S;
    return rp[b + (size_t)c1 + 1] - rp[b + (size_t)c0];
  };

  // Runs are maximal contiguous 1s in cur → horizontal extend is already blocked.
  auto maximal_strip = [&](int r1, int r2, int c0, int c1) -> bool {
    const int w = c1 - c0 + 1;
    if (r1 > 0 && row_seg(r1 - 1, c0, c1) == w) {
      return false;
    }
    if (r2 + 1 < n && row_seg(r2 + 1, c0, c1) == w) {
      return false;
    }
    return true;
  };

  auto maximal_full_width = [&](int r1, int r2) -> bool {
    if (r1 > 0 && row_seg(r1 - 1, 0, n - 1) == n) {
      return false;
    }
    if (r2 + 1 < n && row_seg(r2 + 1, 0, n - 1) == n) {
      return false;
    }
    return true;
  };

  vector<Rect> rects;
  rects.reserve((size_t)n * (size_t)n);

  vector<uint64_t> cur((size_t)W);

  for (int r1 = 0; r1 < n; r1++) {
    copy(rowb.begin() + (ptrdiff_t)r1 * W, rowb.begin() + (ptrdiff_t)(r1 + 1) * W, cur.begin());
    for (int r2 = r1; r2 < n; r2++) {
      if (r2 > r1) {
        const uint64_t* rb = rowb.data() + (size_t)r2 * (size_t)W;
        for (int w = 0; w < W; w++) {
          cur[(size_t)w] &= rb[(size_t)w];
        }
      }
      if (W > 0) {
        cur[(size_t)(W - 1)] &= last_word_mask;
      }

      bool any = false;
      for (int w = 0; w < W; w++) {
        if (cur[(size_t)w]) {
          any = true;
          break;
        }
      }
      if (!any) {
        break;
      }

      bool cur_full = true;
      for (int w = 0; w < W; w++) {
        const uint64_t want = (w + 1 == W) ? last_word_mask : ~0ULL;
        if (cur[(size_t)w] != want) {
          cur_full = false;
          break;
        }
      }
      if (cur_full) {
        if (maximal_full_width(r1, r2)) {
          rects.push_back({r1, r2, 0, n - 1});
        }
        continue;
      }

      int c = 0;
      while (c < n) {
        const int wi = c >> 6;
        uint64_t bits = cur[(size_t)wi] >> (c & 63);
        if (!bits) {
          c = (wi + 1) << 6;
          if (c > n) {
            break;
          }
          continue;
        }
        const int c0 = c + __builtin_ctzll(bits);
        int c1 = c0;
        const uint64_t* cp = cur.data();
        while (c1 < n) {
          const int wj = c1 >> 6;
          const int off = c1 & 63;
          const uint64_t x = cp[(size_t)wj] >> off;
          if (!x) {
            --c1;
            break;
          }
          const int room = min(64 - off, n - c1);
          const uint64_t mask = (room >= 64) ? ~0ULL : ((1ULL << room) - 1ULL);
          const uint64_t t = x & mask;
          if (!t) {
            --c1;
            break;
          }
          if (t == mask) {
            c1 += room;
            continue;
          }
          // First zero of t within mask: length of run of 1s from bit 0
          const int len = __builtin_ctzll((~t) & mask);
          c1 += len - 1;
          break;
        }
        if (c1 >= n) {
          c1 = n - 1;
        }
        if (c1 < c0) {
          c1 = c0;
        }
        if (maximal_strip(r1, r2, c0, c1)) {
          rects.push_back({r1, r2, c0, c1});
        }
        c = c1 + 1;
      }
    }
  }

  const int R = (int)rects.size();
  vector<long long> d_flat((size_t)(n + 2) * (size_t)(n + 2), 0);
#define DI(ii, jj) (d_flat[(size_t)(ii) * (size_t)(n + 2) + (size_t)(jj)])

  for (const Rect& re : rects) {
    DI(re.r1, re.c1) += 1;
    DI(re.r1, re.c2 + 1) -= 1;
    DI(re.r2 + 1, re.c1) -= 1;
    DI(re.r2 + 1, re.c2 + 1) += 1;
  }

  vector<long long> cov_flat((size_t)n * (size_t)n);
#define CI(ii, jj) (cov_flat[(size_t)(ii) * (size_t)n + (size_t)(jj)])

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      long long v = DI(i, j);
      if (i > 0) {
        v += CI(i - 1, j);
      }
      if (j > 0) {
        v += CI(i, j - 1);
      }
      if (i > 0 && j > 0) {
        v -= CI(i - 1, j - 1);
      }
      CI(i, j) = v;
    }
  }

  vector<int> E((size_t)(n + 1) * (size_t)(n + 1), 0);
#define EI(ii, jj) (E[(size_t)(ii) * (size_t)(n + 1) + (size_t)(jj)])

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      const int cell = (CI(i, j) == 1) ? 1 : 0;
      EI(i + 1, j + 1) = EI(i, j + 1) + EI(i + 1, j) - EI(i, j) + cell;
    }
  }

  auto exclusive_sum = [&](int r1, int c1, int r2, int c2) -> int {
    return EI(r2 + 1, c2 + 1) - EI(r1, c2 + 1) - EI(r2 + 1, c1) + EI(r1, c1);
  };

  long long type1 = R;
  long long type2 = 0;
  for (const Rect& re : rects) {
    if (exclusive_sum(re.r1, re.c1, re.r2, re.c2) > 0) {
      type2++;
    }
  }

#undef DI
#undef CI
#undef EI

  cout << type1 << ' ' << type2 << '\n';
  return 0;
}
