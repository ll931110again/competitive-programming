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
using namespace std;

int T, x, y;

bool solve() {
	// Cannot have empty board
	if (x + y == 0) {
		return false;
	}

	// The board must be a square
	int k = sqrt(x + y);
	if (k * k != x + y) {
		return false;
	}

	// If k is odd, then every combination works
	if (k % 2) {
		return true;
	}

	// If k is even, then we only have a palindrome if each row has
	// either even number of white stones and even number of black stones.
	// Summing over all rows, this requires x and y are both even.
	if ((x % 2) && (y % 2)) {
		return false;
	}
	return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
	while (T--) {
		cin >> x >> y;
		cout << (solve() ? "YES" : "NO") << endl;
	}

    return 0;
}