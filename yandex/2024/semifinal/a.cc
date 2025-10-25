#include <algorithm>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#define maxn 100005
using namespace std;

int T, n;
int x[maxn], y[maxn];

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> x[i] >> y[i];
	}
	sort(x, x + n);
	sort(y, y + n);

	long long ret = 0;
	for (int i = 0; i < n; i++) {
		ret += abs(x[i] - (i + 1)) + abs(y[i] - (i + 1));
	}
	cout << ret << endl;
	return 0;
}