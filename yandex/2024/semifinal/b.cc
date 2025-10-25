#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#define maxn 100005
using namespace std;

template <int MOD=1'000'000'007>
struct Modular {
  int value;
  static const int MOD_value = MOD;

  Modular(long long v = 0) { value = v % MOD; if (value < 0) value += MOD;}
  Modular(long long a, long long b) : value(0){ *this += a; *this /= b;}

  Modular& operator+=(Modular const& b) {value += b.value; if (value >= MOD) value -= MOD; return *this;}
  Modular& operator-=(Modular const& b) {value -= b.value; if (value < 0) value += MOD;return *this;}
  Modular& operator*=(Modular const& b) {value = (long long)value * b.value % MOD;return *this;}

  friend Modular mexp(Modular a, long long e) {
    Modular res = 1; while (e) { if (e&1) res *= a; a *= a; e >>= 1; }
    return res;
  }
  friend Modular inverse(Modular a) { return mexp(a, MOD - 2); }

  Modular& operator/=(Modular const& b) { return *this *= inverse(b); }
  friend Modular operator+(Modular a, Modular const b) { return a += b; }
  friend Modular operator-(Modular a, Modular const b) { return a -= b; }
  friend Modular operator-(Modular const a) { return 0 - a; }
  friend Modular operator*(Modular a, Modular const b) { return a *= b; }
  friend Modular operator/(Modular a, Modular const b) { return a /= b; }
  friend std::ostream& operator<<(std::ostream& os, Modular const& a) {return os << a.value;}
  friend bool operator==(Modular const& a, Modular const& b) {return a.value == b.value;}
  friend bool operator!=(Modular const& a, Modular const& b) {return a.value != b.value;}
};

int n, k;
Modular<1000000007> fact[maxn], inv[maxn];

int main() {
	cin >> n >> k;
	fact[0] = 1;
	for (int i = 1; i < maxn; i++) {
		fact[i] = fact[i - 1] * i;
	}

	inv[maxn - 1] = inverse(fact[maxn - 1]);
	for (int i = maxn - 2; i >= 0; i--) {
		inv[i] = inv[i + 1] * (i + 1);
	}

	auto binom = [&](int x, int y) {
		auto top = fact[x];
		auto bot = inv[y] * inv[x - y];
		return top * bot;
	};

	auto ans = Modular(k);
	
	auto ss = Modular(1), ps = mexp(Modular(2), k);
	for (int i = 1; i <= k; i++) {
		ans -= mexp(ss / ps, n - 1);
		ss += binom(k, i);
	}

	cout << ans << endl;

	return 0;
}