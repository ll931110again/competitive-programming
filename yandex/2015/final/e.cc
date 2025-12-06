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
#define maxn 10005
#define alphabet 26
#define mod 1000000007
using namespace std;

int T;
long long dp[maxn][alphabet + 1][2];
int fill_value[maxn];
int first_pos[alphabet], last_pos[alphabet], nxt_non_empty[maxn];

long long solve(string s) {
	int n = s.size();
	memset(fill_value, -1, sizeof fill_value);
	memset(first_pos, -1, sizeof first_pos);
	memset(last_pos, -1, sizeof last_pos);

	for (int i = 0; i < n; i++) {
		if (s[i] == '?') {
			continue;
		}
		int idx = s[i] - 'a';
		if (first_pos[idx] < 0) {
			first_pos[idx] = i;
		}
		last_pos[idx] = i;
	}

	for (int i = 0; i < alphabet; i++) if (first_pos[i] >= 0) {
		for (int j = first_pos[i]; j <= last_pos[i]; j++) {
			// The position has already been occupied by another character
			if (fill_value[j] >= 0) {
				return 0;
			}
			fill_value[j] = i;
		}
	}

	int remaining = alphabet;
	for (int i = 0; i < alphabet; i++) if (first_pos[i] >= 0) {
		remaining--;
	}

	memset(dp, 0, sizeof dp);
	dp[0][remaining][0] = 1;

	memset(nxt_non_empty, -1, sizeof nxt_non_empty);
	for (int i = n - 1; i >= 0; i--) {
		nxt_non_empty[i] = nxt_non_empty[i + 1];
		if (fill_value[i] >= 0) {
			nxt_non_empty[i] = i;
		}
	}

	for (int i = 0; i < n; i++)
		for (int j = 0; j <= alphabet; j++)
			for (int k = 0; k < 2; k++) if (dp[i][j][k]) {
				if (fill_value[i] >= 0) {
					// Only one choice here: fill in with the current character
					dp[i + 1][j][1] += dp[i][j][k];
					dp[i + 1][j][1] %= mod;
				} else {
					// Fill in with the current character
					if (k) {
						dp[i + 1][j][1] += dp[i][j][k];
						dp[i + 1][j][1] %= mod;
					}

					// Fill in with the next character
					if (nxt_non_empty[i] >= 0) {
						int _i = nxt_non_empty[i];
						dp[_i][j][1] += dp[i][j][k];
						dp[_i][j][1] %= mod;
					}
					// Fill in with a new character
					if (j > 0) {
						dp[i + 1][j - 1][1] += dp[i][j][k] * j;
						dp[i + 1][j - 1][1] %= mod;
					}
				}
			}

	long long ans = 0;
	for (int j = 0; j <= alphabet; j++) {
		ans += dp[n][j][1];
		ans %= mod;
	}

	return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

	cin >> T;
	while (T--) {
		string s;
		cin >> s;
		cout << solve(s) << endl;
	}

    return 0;
}