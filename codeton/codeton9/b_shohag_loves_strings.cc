// Codeforces 2039 (CodeTON Round 9 (Div. 1 + Div. 2, Rated, Prizes!)) — B. Shohag Loves Strings
// Submission: https://codeforces.com/contest/2039/submission/295467520

#include <algorithm>
#include <iostream>
using namespace std;

int T;

string solve(string s) {
	int n = s.size();
	for (int i = 0; i + 2 <= n; i++) {
		if (s[i] == s[i + 1]) {
			return s.substr(i, 2);
		}
	}

	for (int i = 0; i + 3 <= n; i++) {
		if (s[i] != s[i + 1] && s[i] != s[i + 2] && s[i + 1] != s[i + 2]) {
			return s.substr(i, 3);
		}
	}

	return "-1";
}

int main() {
	cin >> T;
	while (T--) {
		string s;
		cin >> s;
		cout << solve(s) << endl;
	}
	return 0;
}
