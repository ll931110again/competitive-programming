// Codeforces Spectral::Cup 2026 Round 1 — Counting Sort?
// https://codeforces.com/contest/2222/problem/H
//
// Sketch
// ------
// f(b)_i = (# of times i appears in b). g(b) is the number of distinct arrays
// on the orbit b, f(b), f(f(b)), ... until a repeat.
//
// The orbit (except a few small cases) depends only on the multiset M of
// positive histogram counts of a. Sweep values k = n, ..., 1: from a state M
// with sum s, choose cnt in 0..R_k - s indices that may still equal k
// (R_k = |{i : r_i >= k}|), multiply by C(R_k - s, cnt), and insert cnt into M
// if cnt > 0.
//
// After the sweep:
//   M empty              -> g = 1 (the zero array)
//   M = {1}              -> split: a_1 = 1 and rest 0 has g = 1; other
//                          single-1 arrays have g = 2; remaining one-nonzero
//                          arrays have g = 3
//   otherwise            -> g = 1 + dep(M), where dep iterates "run-length
//                          counts of M, sorted decreasing"
//
// Complexity: number of integer-partition states for n <= 50, times O(n)
// per layer.

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int kMod = 998244353;
constexpr int kMaxN = 50;

struct ModInt {
  int value = 0;

  ModInt() = default;
  explicit ModInt(int64_t x) {
    x %= kMod;
    if (x < 0) {
      x += kMod;
    }
    value = static_cast<int>(x);
  }

  ModInt& operator+=(ModInt other) {
    value += other.value;
    if (value >= kMod) {
      value -= kMod;
    }
    return *this;
  }

  ModInt& operator-=(ModInt other) {
    value -= other.value;
    if (value < 0) {
      value += kMod;
    }
    return *this;
  }

  ModInt& operator*=(ModInt other) {
    value = static_cast<int>(int64_t(value) * other.value % kMod);
    return *this;
  }

  friend ModInt operator+(ModInt a, ModInt b) {
    return a += b;
  }
  friend ModInt operator-(ModInt a, ModInt b) {
    return a -= b;
  }
  friend ModInt operator*(ModInt a, ModInt b) {
    return a *= b;
  }

  friend ostream& operator<<(ostream& os, ModInt a) {
    return os << a.value;
  }
};

struct Combinatorics {
  vector<ModInt> fact;
  vector<ModInt> inv_fact;

  explicit Combinatorics(int n) : fact(n + 1), inv_fact(n + 1) {
    fact[0] = ModInt(1);
    for (int i = 1; i <= n; i++) {
      fact[i] = fact[i - 1] * ModInt(i);
    }
    inv_fact[n] = pow(fact[n], kMod - 2);
    for (int i = n; i > 0; i--) {
      inv_fact[i - 1] = inv_fact[i] * ModInt(i);
    }
  }

  ModInt binom(int n, int k) const {
    if (k < 0 || k > n) {
      return ModInt(0);
    }
    return fact[n] * inv_fact[k] * inv_fact[n - k];
  }

private:
  static ModInt pow(ModInt base, int exp) {
    ModInt out(1);
    while (exp) {
      if (exp & 1) {
        out *= base;
      }
      base *= base;
      exp >>= 1;
    }
    return out;
  }
};

unordered_map<string, int> state_id;
vector<string> state_value;
vector<int> state_sum;
vector<int> depth_memo;
vector<array<int, kMaxN + 1>> go;

int get_id(const string& s) {
  auto it = state_id.find(s);
  if (it != state_id.end()) {
    return it->second;
  }
  const int id = static_cast<int>(state_value.size());
  state_id[s] = id;
  state_value.push_back(s);
  int sum = 0;
  for (unsigned char c : s) {
    sum += c;
  }
  state_sum.push_back(sum);
  depth_memo.push_back(-1);
  array<int, kMaxN + 1> nxt;
  nxt.fill(-1);
  go.push_back(nxt);
  return id;
}

int add_part(int u, int x) {
  if (x == 0) {
    return u;
  }
  if (go[u][x] != -1) {
    return go[u][x];
  }
  string s = state_value[u];
  int p = 0;
  while (p < static_cast<int>(s.size()) && static_cast<unsigned char>(s[p]) >= x) {
    p++;
  }
  s.insert(s.begin() + p, static_cast<char>(x));
  return go[u][x] = get_id(s);
}

string next_multiset(const string& s) {
  vector<int> runs;
  for (int i = 0; i < static_cast<int>(s.size());) {
    int j = i;
    while (j < static_cast<int>(s.size()) && s[j] == s[i]) {
      j++;
    }
    runs.push_back(j - i);
    i = j;
  }
  sort(runs.rbegin(), runs.rend());
  string t;
  for (int x : runs) {
    t += static_cast<char>(x);
  }
  return t;
}

int depth(int u) {
  if (depth_memo[u] != -1) {
    return depth_memo[u];
  }
  const string& s = state_value[u];
  if (s.empty()) {
    return depth_memo[u] = 1;
  }
  if (s.size() == 1 && static_cast<unsigned char>(s[0]) == 1) {
    return depth_memo[u] = 2;
  }
  return depth_memo[u] = depth(get_id(next_multiset(s))) + 1;
}

void solve_one(const Combinatorics& comb) {
  int n, k;
  cin >> n >> k;
  vector<int> cap(n);
  for (int i = 0; i < n; i++) {
    cin >> cap[i];
  }
  vector<int> can_take(n + 1);
  for (int x = 1; x <= n; x++) {
    for (int i = 0; i < n; i++) {
      can_take[x] += cap[i] >= x;
    }
  }

  const int root = get_id("");
  vector<ModInt> dp(state_value.size());
  vector<int> live;
  dp[root] = ModInt(1);
  live.push_back(root);

  for (int x = n; x >= 1; x--) {
    vector<ModInt> ndp(state_value.size());
    vector<int> next_live;
    vector<int> seen(state_value.size());
    const int stamp = x;
    for (int u : live) {
      const int free = can_take[x] - state_sum[u];
      for (int cnt = 0; cnt <= free; cnt++) {
        const int v = add_part(u, cnt);
        if (static_cast<int>(ndp.size()) < static_cast<int>(state_value.size())) {
          ndp.resize(state_value.size());
          seen.resize(state_value.size());
        }
        ndp[v] += dp[u] * comb.binom(free, cnt);
        if (seen[v] != stamp) {
          seen[v] = stamp;
          next_live.push_back(v);
        }
      }
    }
    dp = std::move(ndp);
    live = std::move(next_live);
  }

  vector<ModInt> ans(k + 1);
  const int singleton_one = get_id(string(1, static_cast<char>(1)));
  for (int u : live) {
    const ModInt weight = dp[u];
    if (state_value[u].empty()) {
      if (k >= 1) {
        ans[1] += weight;
      }
    } else if (u == singleton_one) {
      const int ones = can_take[1];
      const int first_can_be_one = cap[0] >= 1;
      if (k >= 1) {
        ans[1] += ModInt(first_can_be_one);
      }
      if (k >= 2) {
        ans[2] += ModInt(ones - first_can_be_one);
      }
      if (k >= 3) {
        ans[3] += weight - ModInt(ones);
      }
    } else {
      const int g = depth(u) + 1;
      if (g <= k) {
        ans[g] += weight;
      }
    }
  }

  for (int p = 1; p <= k; p++) {
    cout << ans[p] << (p == k ? '\n' : ' ');
  }
}

} // namespace

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  Combinatorics comb(kMaxN);
  int test_count;
  cin >> test_count;
  while (test_count--) {
    solve_one(comb);
  }
  return 0;
}
