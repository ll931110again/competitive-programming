#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 10000005
using namespace std;

struct Fraction {
	int numerator;
	int denominator;
};

int n;
Fraction fs[maxn];

int min_divisor[maxn];
int denominator_idx[maxn];

int chosen_fractions_idx[maxn];
int cnt = 0;

bool cmpDenominator(Fraction x, Fraction y) {
	return x.denominator < y.denominator;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

	for (int i = 2; i < maxn; i++) if (!min_divisor[i]) {
		for (int j = i; j < maxn; j += i) if (!min_divisor[j]) {
			min_divisor[j] = i;
		}
	}

	cin >> n;
	n--;
	for (int i = 0; i < n; i++) {
		string s;
		cin >> s;

		int pos = -1;
		for (int j = 0; j < s.size(); j++) if (s[j] == '/') {
			pos = j;
			break;
		}

		int numerator = stoi(s.substr(0, pos));
		int denominator = stoi(s.substr(pos + 1, (int) s.size() - pos - 1));
		fs[i] = {.numerator = numerator, .denominator = denominator};
	}
	
	sort(fs, fs + n, cmpDenominator);
	memset(denominator_idx, -1, sizeof denominator_idx);
	int pos = 0;

	// First, choose the fraction fs[0].numerator/fs[0].denominator, where fs[0].denominator = 2
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
		// This is guaranteed to terminate since we will choose all the prime numbers less than equal to N
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