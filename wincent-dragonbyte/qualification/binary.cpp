#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstring>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#include <queue>
using namespace std;

int T;
long long n;
long long MAX_N = 1e18 + 1;

long long evaluate(vector<int> polynomial, long long mid) {
    long long ans = 0;
    for (auto coef : polynomial) {
        if (MAX_N / mid < ans) {
            return MAX_N;
        }
        ans = ans * mid + coef;
    }
    return ans;
}

long long solve_polynomial(long long n, vector<int> polynomial) {
    long long low = 1, high = n, ans = -1;
    while (low <= high) {
        long long mid = (low + high) / 2;
        long long tmp = evaluate(polynomial, mid);
        if (tmp == n) {
            return mid;
        }
        if (tmp > n) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return -1;
}

bool is_valid(long long n, long long base) {
    while (n) {
        if (n % base > 1) {
            return false;
        }
        n /= base;
    }
    return true;
}

long long solve(long long n) {
    if (n == 1) {
        return 3;
    }
    if (n == 2) {
        return -1;
    }
    if (n == 3) {
        return 3;
    }

    long long ans = n - 1;
    vector< vector<int> >  polynomials = { {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}, {1, 0, 0, 0}, {1, 0, 0, 1}, {1, 0, 1, 0}, {1, 0, 1, 1}, {1, 1, 0, 0}, {1, 1, 0, 1}, {1, 1, 1, 0}, {1, 1, 1, 1} };
    for (const auto polynomial : polynomials) {
        auto value = solve_polynomial(n, polynomial);
        if (value > 2 && value < ans) {
            ans = value;
        }
    }

    for (long long base = 3; base * base * base * base <= n; base++) {
        if (is_valid(n, base)) {
            ans = min(ans, base);
            break;
        }
    }

    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);

    cin >> T;
    while (T--) {
        cin >> n;
        cout << solve(n) << endl;
    }
	return 0;
}
