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
 
constexpr unsigned MOD = 1'000'000'007;
using Mint = ModInt<MOD>;

Mint binom(long long x, long long y) {
	if (x < y) {
		return 0;
	}
	Mint ans = 1;
	for (int i = 1; i <= y; i++) {
		ans *= (x - i + 1);
		ans /= i;
	}
	return ans;
}

int T;
long long n, a, b;

vector< pair<long long, int> > mp;
vector<int> divisors;

Mint ans;

vector<int> complement_divisors(vector<int> x) {
	vector<int> y = divisors;
	for (int i = 0; i < divisors.size(); i++) {
		y[i] = mp[i].second - x[i];
	}
	return y;
}

Mint steps(long long n, vector<int> divisors) {
	Mint value = 1;
	for (auto x : divisors) {
		value *= binom(n + x - 1, x);
	}
	return value;
}

void rec(int idx, long long prod) {
	if (prod > a) {
		return;
	}

	if (idx >= divisors.size()) {
		ans += steps(n, divisors) * steps(n, complement_divisors(divisors));
		return;
	}

	for (int i = 0; i <= mp[idx].second; i++) {
		divisors[idx] = i;
		rec(idx + 1, prod);
		prod *= mp[idx].first;
	}
}

Mint solve(long long n, long long a, long long b) {
	mp.clear();
	long long tmp = b;

	for (long long i = 2; i * i <= b; i++) if (tmp % i == 0) {
		int cnt = 0;
		while (tmp % i == 0) {
			cnt++;
			tmp /= i;
		}
		mp.push_back({i, cnt});
	}

	if (tmp > 1) {
		mp.push_back({tmp, 1});
	}

	ans = 0;
	divisors.resize(mp.size());
	rec(0, 1);
	return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
        cerr << it << endl;
		cin >> n >> a >> b;
		cout << "Case #" << it << ": ";
		cout << solve(n, a, b) << endl;
    }

    return 0;
}