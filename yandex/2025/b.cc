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
#define maxn 1005
using namespace std;

int T, m, n;
string s1[maxn], s2[maxn];
bool match[maxn][maxn];

int height[maxn];
int ls[maxn], rs[maxn];

int solve() {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			match[i][j] = (s1[i][j] == s2[i][j]);
		}
	}

	int ans = 0;
	memset(height, 0, sizeof height);
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			if (match[i][j]) {
				height[j]++;
			} else {
				height[j] = 0;
			}
		}
		stack<int> s;
		for (int j = 0; j < n; j++) {
			while (!s.empty() && height[s.top()] >= height[j]) {
				s.pop();
			}
			ls[j] = (s.empty()) ? 0 : (1 + s.top());
			s.push(j);
		}

		while (!s.empty()) {
			s.pop();
		}

		for (int j = n - 1; j >= 0; j--) {
			while (!s.empty() && height[s.top()] >= height[j]) {
				s.pop();
			}
			rs[j] = (s.empty()) ? (n - 1) : (s.top() - 1);
			s.push(j);
		}

		for (int j = 0; j < n; j++) {
			ans = max(ans, height[j] * (rs[j] - ls[j] + 1));
		}
	}

	return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
	while (T--) {
		cin >> m >> n;
		for (int i = 0; i < m; i++) {
			cin >> s1[i];
		}
		for (int i = 0; i < m; i++) {
			cin >> s2[i];
		}
		cout << solve() << endl;
	}

    return 0;
}