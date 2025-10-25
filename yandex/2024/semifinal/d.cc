#include <algorithm>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
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

Modular<1000000007> solve(long long R) {
	if (R < 2) {
		return 0;
	}

	// special case: two digits
	auto ans = Modular(R - 1) * Modular(R) / 2 * 2;

	// three digits
	long long n = sqrt(R);
	auto threes = Modular(R + 1) * (n - 1);
	// subtract 2^2 + .. + n^2
	threes -= Modular(n) * Modular(n + 1) * Modular(n * 2 + 1) / 6 - 1;

	ans += threes;

	// four digits
	long long low = 1, high = 1e6;
	while (low <= high) {
		long long mid = (low + high) / 2;
		if (mid * mid * mid <= R) {
			n = mid;
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}

	auto fours = Modular(R + 1) * (n - 1);
	// subtract 2^3 + ... + n^3
	auto sums = Modular(n) * Modular(n + 1) / 2;
	sums = sums * sums;
	fours -= (sums - 1);

	ans += fours;

	// five digits
	low = 1;
	high = 32000;
	while (low <= high) {
		long long mid = (low + high) / 2;
		if (mid * mid * mid * mid <= R) {
			n = mid;
			low = mid + 1;
		} else {
			high = mid - 1;
		}
	}

	auto fives = Modular(R + 1) * (n - 1);
	// subtract 2^4 + .. + n^4
	sums = Modular(n) * Modular(n + 1) * Modular(n * 2 + 1) * Modular(n * n * 3 + n * 3 - 1) / 30 - 1;
	fives -= sums;
	ans += fives;

	for (int x = 2; ; x++) {
		bool found = false;
		long long value = 1, d = 0;

		while (true) {
			if (R / x < value) {
				break;
			}

			d++;
			value *= x;

			if (d >= 5) {
				found = true;
				ans += (R - value + 1);
			}
		}

		if (!found) {
			break;
		}
	}

	return ans;
}

int T;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> T;
	while (T--) {
		long long L, R;
		cin >> L >> R;
		cout << solve(R) - solve(L - 1)  << endl;
	}
	return 0;
}