/*
 * Solution for problem H: Hands of Poker.
 *
 * Rank each of C(52,5) hands with a tuple consistent with the statement; sort and map to 1..7462.
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Score {
  array<int, 6> t{};
  bool operator<(const Score& o) const {
    return t < o.t;
  }
  bool operator==(const Score& o) const {
    return t == o.t;
  }
};

Score eval_hand(const array<int, 5>& ix) {
  int rnk[5], sut[5];
  for (int i = 0; i < 5; ++i) {
    rnk[i] = ix[i] % 13;
    sut[i] = ix[i] / 13;
  }
  sort(rnk, rnk + 5);
  int cnt[13] = {};
  for (int i = 0; i < 5; ++i)
    cnt[rnk[i]]++;

  bool flush = sut[0] == sut[1] && sut[1] == sut[2] && sut[2] == sut[3] && sut[3] == sut[4];

  auto straight_high = [&]() -> int {
    int x[5];
    for (int i = 0; i < 5; ++i)
      x[i] = rnk[i];
    bool wheel = (x[0] == 0 && x[1] == 1 && x[2] == 2 && x[3] == 3 && x[4] == 12);
    if (wheel)
      return 3;
    for (int i = 0; i < 4; ++i)
      if (x[i + 1] != x[i] + 1)
        return -1;
    return x[4];
  };

  int sh = straight_high();
  bool straight = sh >= 0;

  pair<int, int> freq[5];
  int fn = 0;
  for (int v = 0; v < 13; ++v)
    if (cnt[v])
      freq[fn++] = {cnt[v], v};
  sort(freq, freq + fn, [](const pair<int, int>& a, const pair<int, int>& b) {
    if (a.first != b.first)
      return a.first > b.first;
    return a.second > b.second;
  });

  Score s;

  if (straight && flush) {
    s.t = {8, sh, 0, 0, 0, 0};
    return s;
  }
  if (freq[0].first == 4) {
    int q = freq[0].second;
    int k = freq[1].second;
    s.t = {7, q, k, 0, 0, 0};
    return s;
  }
  if (freq[0].first == 3 && freq[1].first == 2) {
    int t = freq[0].second;
    int p = freq[1].second;
    s.t = {6, t, p, 0, 0, 0};
    return s;
  }
  if (flush) {
    s.t = {5, rnk[4], rnk[3], rnk[2], rnk[1], rnk[0]};
    return s;
  }
  if (straight) {
    s.t = {4, sh, 0, 0, 0, 0};
    return s;
  }
  if (freq[0].first == 3) {
    int t = freq[0].second;
    int k1 = freq[1].second;
    int k2 = freq[2].second;
    int hi = max(k1, k2);
    int lo = min(k1, k2);
    s.t = {3, t, hi, lo, 0, 0};
    return s;
  }
  if (freq[0].first == 2 && freq[1].first == 2) {
    int hip = freq[0].second;
    int lop = freq[1].second;
    int k = freq[2].second;
    s.t = {2, hip, lop, k, 0, 0};
    return s;
  }
  if (freq[0].first == 2) {
    int p = freq[0].second;
    int ks[3];
    int nk = 0;
    for (int i = 1; i < fn; ++i)
      ks[nk++] = freq[i].second;
    sort(ks, ks + nk, greater<int>());
    s.t = {1, p, ks[0], ks[1], ks[2], 0};
    return s;
  }

  s.t = {0, rnk[4], rnk[3], rnk[2], rnk[1], rnk[0]};
  return s;
}

vector<Score> all_scores;

// Stable LSD radix on t[5]..t[0]; keys fit in [0,15] for our Score encoding.
void radix_sort_scores(vector<Score>& v) {
  const int W = 16;
  vector<Score> tmp(v.size());
  int cnt[W];
  for (int pos = 5; pos >= 0; --pos) {
    for (int i = 0; i < W; ++i)
      cnt[i] = 0;
    for (const auto& s : v) {
      int k = s.t[pos];
      if (k < 0 || k >= W)
        k = W - 1;
      ++cnt[k];
    }
    for (int i = 1; i < W; ++i)
      cnt[i] += cnt[i - 1];
    for (int i = (int)v.size() - 1; i >= 0; --i) {
      int k = v[i].t[pos];
      if (k < 0 || k >= W)
        k = W - 1;
      tmp[--cnt[k]] = v[i];
    }
    v.swap(tmp);
  }
}

void build_table() {
  all_scores.reserve(2598960);
  array<int, 5> h{};
  for (h[0] = 0; h[0] < 52; ++h[0])
    for (h[1] = h[0] + 1; h[1] < 52; ++h[1])
      for (h[2] = h[1] + 1; h[2] < 52; ++h[2])
        for (h[3] = h[2] + 1; h[3] < 52; ++h[3])
          for (h[4] = h[3] + 1; h[4] < 52; ++h[4])
            all_scores.push_back(eval_hand(h));
  radix_sort_scores(all_scores);
  size_t w = 0;
  for (size_t i = 0; i < all_scores.size(); ++i) {
    if (w == 0 || !(all_scores[w - 1] == all_scores[i]))
      all_scores[w++] = all_scores[i];
  }
  all_scores.resize(w);
}

int card_index(char rk, char st) {
  const char* R = "23456789TJQKA";
  int r = strchr(R, rk) - R;
  int s = strchr("CDHS", st) - "CDHS";
  return s * 13 + r;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("hands.in", "r", stdin);
  freopen("hands.out", "w", stdout);

  build_table();

  array<int, 5> h{};
  for (int i = 0; i < 5; ++i) {
    string w;
    cin >> w;
    h[i] = card_index(w[0], w[1]);
  }
  Score s = eval_hand(h);
  int pos = int(lower_bound(all_scores.begin(), all_scores.end(), s) - all_scores.begin()) + 1;
  cout << pos << '\n';
  return 0;
}
