// Codeforces 1870 (CodeTON Round 6 (Div. 1 + Div. 2, Rated, Prizes!)) — D. Prefix Purchase
// Submission: https://codeforces.com/contest/1870/submission/334302925

#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <iostream>
#include <cstring>
#include <queue>
#include <stack>
#include <vector>
#include <unordered_set>
#define maxn 200005
using namespace std;
 
int T, n, k;
int a[maxn];
int amount[maxn];
int budget;
 
void solve() {
	for (int i = n - 2; i >= 0; i--) {
		a[i] = min(a[i], a[i + 1]);
	}
 
	for (int i = 0; i < n; i++) {
		amount[i] = 0;
	}
 
	amount[0] = budget / a[0];
	budget -= amount[0] * a[0];
    
    stack<int> s;
    s.push(0);

	for (int i = 1; i < n; i++) {
        while (!s.empty()) {
            int prev = s.top();

            int can_move = 0;
            if (a[prev] == a[i]) {
                can_move = amount[prev];
            } else {
                int delta = a[i] - a[prev];
                can_move = min(budget / delta, amount[prev]);
            }

            if (a[prev] < a[i] && can_move == 0) {
                break;
            }

            budget -= can_move * (a[i] - a[prev]);
            amount[i] += can_move;
            amount[prev] -= can_move;
            s.pop();
        }
        s.push(i);
	}
 
	for (int i = n - 2; i >= 0; i--) {
		amount[i] += amount[i + 1];
	}
	for (int i = 0; i < n; i++) {
		cout << amount[i] << ' ';
	}
	cout << endl;
}
 
int main() {
	std::ios::sync_with_stdio(false);
	std::cin.tie(nullptr);
 
	cin >> T;
	while (T--) {
		cin >> n;
		for (int i = 0; i < n; i++) {
			cin >> a[i];
		}
		cin >> budget;
		solve();
	}
	return 0;
}
