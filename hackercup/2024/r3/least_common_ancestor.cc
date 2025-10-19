#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 3000005
using namespace std;

template <unsigned M_> struct ModInt {
  static constexpr unsigned M = M_;
  unsigned x;
  constexpr ModInt() : x(0U) {}
  constexpr ModInt(unsigned x_) : x(x_ % M) {}
  constexpr ModInt(unsigned long long x_) : x(x_ % M) {}
  constexpr ModInt(int x_) : x(((x_ %= static_cast<int>(M)) < 0) ? (x_ + static_cast<int>(M)) : x_) {}
  constexpr ModInt(long long x_) : x(((x_ %= static_cast<long long>(M)) < 0) ? (x_ + static_cast<long long>(M)) : x_) {}
  ModInt &operator+=(const ModInt &a) { x = ((x += a.x) >= M) ? (x - M) : x; return *this; }
  ModInt &operator-=(const ModInt &a) { x = ((x -= a.x) >= M) ? (x + M) : x; return *this; }
  ModInt &operator*=(const ModInt &a) { x = (static_cast<unsigned long long>(x) * a.x) % M; return *this; }
  ModInt &operator/=(const ModInt &a) { return (*this *= a.inv()); }
  ModInt pow(long long e) const {
    if (e < 0) return inv().pow(-e);
    ModInt a = *this, b = 1U; for (; e; e >>= 1) { if (e & 1) b *= a; a *= a; } return b;
  }
  ModInt inv() const {
    unsigned a = M, b = x; int y = 0, z = 1;
    for (; b; ) { const unsigned q = a / b; const unsigned c = a - q * b; a = b; b = c; const int w = y - static_cast<int>(q) * z; y = z; z = w; }
    return ModInt(y);
  }
  ModInt operator+() const { return *this; }
  ModInt operator-() const { ModInt a; a.x = x ? (M - x) : 0U; return a; }
  ModInt operator+(const ModInt &a) const { return (ModInt(*this) += a); }
  ModInt operator-(const ModInt &a) const { return (ModInt(*this) -= a); }
  ModInt operator*(const ModInt &a) const { return (ModInt(*this) *= a); }
  ModInt operator/(const ModInt &a) const { return (ModInt(*this) /= a); }
  template <class T> friend ModInt operator+(T a, const ModInt &b) { return (ModInt(a) += b); }
  template <class T> friend ModInt operator-(T a, const ModInt &b) { return (ModInt(a) -= b); }
  template <class T> friend ModInt operator*(T a, const ModInt &b) { return (ModInt(a) *= b); }
  template <class T> friend ModInt operator/(T a, const ModInt &b) { return (ModInt(a) /= b); }
  explicit operator bool() const { return x; }
  bool operator==(const ModInt &a) const { return (x == a.x); }
  bool operator!=(const ModInt &a) const { return (x != a.x); }
  friend std::ostream &operator<<(std::ostream &os, const ModInt &a) { return os << a.x; }
};
 
constexpr unsigned MOD = 998'244'353;
using Mint = ModInt<MOD>;

int T, n;
int p[maxn];
string s[maxn];

int up[maxn], down[maxn];
set<string> universes;
map<string, int> indices;
vector<int> adj[maxn];

int occurences[maxn];
set< pair<int,int> > values;

int down_ref[maxn];
unordered_map<int,int> down_occurences[maxn];
set< pair<int,int> > down_values[maxn];

void update(pair<int,int> old_value, pair<int,int> new_value) {
    values.erase(old_value);
    
    if (new_value.first > 0) {
        values.insert(new_value);
    }
}

int find_min_index() {
    if (values.empty()) {
        return 0;
    }
    return values.begin()->second;
}

void recurse_up(int i) {
    up[i] = find_min_index();
   
    int ix = indices[s[i]];
    update({occurences[ix], ix}, {occurences[ix] + 1, ix});
    occurences[ix]++;

    for (auto v : adj[i]) {
        recurse_up(v);
    }

    update({occurences[ix], ix}, {occurences[ix] - 1, ix});
    occurences[ix]--;
}

int get(int u) {
    if (u != down_ref[u]) {
        down_ref[u] = get(down_ref[u]);
    }
    return down_ref[u];
}

void merge(int u, int v) {
    u = get(u);
    v = get(v);

    if (down_values[u].size() < down_values[v].size()) {
        swap(u, v);
    }

    down_ref[v] = u;
    for (auto it : down_occurences[v]) {
        int k = it.first, cnt = it.second;
        down_values[u].erase({down_occurences[u][k], k});
        down_occurences[u][k] += cnt;
        down_values[u].insert({down_occurences[u][k], k});
    }

    down_occurences[v].clear();
    down_values[v].clear();
}

void recurse_down(int i) {
    for (auto v : adj[i]) {
        recurse_down(v);
    }
    
    for (auto v : adj[i]) {
        merge(i, v);
    }

    int ref = get(i);
    if (down_values[ref].empty()) {
        down[i] = 0;
    } else {
        down[i] = down_values[ref].begin()->second;
    }

    // Add s[i]'s in
    int ix = indices[s[i]];
    down_values[ref].erase({down_occurences[ref][ix], ix});
    down_occurences[ref][ix]++;
    down_values[ref].insert({down_occurences[ref][ix], ix});
}

int solve() {
    universes.clear();
    indices.clear();

    for (int i = 1; i <= n; i++) {
        universes.insert(s[i]);
    }

    for (int i = 1; i <= n; i++) {
        down_values[i].clear();
        down_occurences[i].clear();
        down_ref[i] = i;
    }

    values.clear();

    int ix = 0;
    for (auto it : universes) {
        indices[it] = ++ix;
        occurences[ix] = 0;
    }

    recurse_up(1);
    recurse_down(1);

    long long ans = 0;
    int ux = universes.size();
    for (int i = 1; i <= n; i++) {
        ans = ans * (ux + 1) + up[i];
        ans %= MOD;
        ans = ans * (ux + 1) + down[i];
        ans %= MOD;
    }

    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
        cerr << it << endl;

        cin >> n;
        for (int i = 1; i <= n; i++) {
            adj[i].clear();
        }

        for (int i = 1; i <= n; i++) {
            cin >> p[i] >> s[i];
            if (p[i] >= 0) {
                adj[p[i]].push_back(i);
            }
        }

        cout << "Case #" << it << ": " << solve() << endl;
    }

    return 0;
}