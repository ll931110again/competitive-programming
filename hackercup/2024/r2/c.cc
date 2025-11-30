#include <algorithm>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#define maxn 805
using namespace std;

int T;
int m, n;
long long K;

int a[maxn][maxn];
unordered_map<int, vector<pair<int,int> > > buckets;

int tx[maxn][maxn];

void update(int x, int y, int delta) {
	for (int dx = x; dx <= m; dx += (dx & -dx)) {
		for (int dy = y; dy <= n; dy += (dy & -dy)) {
			tx[dx][dy] += delta;
		}
	}
}

long long get_single(int x, int y) {
	long long ans = 0;
	for (int dx = x; dx > 0; dx -= (dx & -dx)) {
		for (int dy = y; dy > 0; dy -= (dy & -dy)) {
			ans += tx[dx][dy];
		}
	}
	return ans;
}

long long get(int x, int y, int mid) {
	int lx = max(1, x - mid);
	int ly = max(1, y - mid);
	int rx = min(m, x + mid);
	int ry = min(n, y + mid);

	long long ans = get_single(rx, ry) - get_single(lx - 1, ry) - get_single(rx, ly - 1) + get_single(lx - 1, ly - 1);
	return (rx - lx + 1) * (ry - ly + 1) - ans;
}

long long count_valid_configuration(int mid) {
	long long ret = 0;
	for (auto bucket : buckets) {
		for (auto item : bucket.second) {
			update(item.first, item.second, 1);
		}
		
		for (auto item : bucket.second) {
			auto value = get(item.first, item.second, mid);
			ret += value;
		}
		
		for (auto item : bucket.second) {
			update(item.first, item.second, -1);
		}
	}

	return ret;
}

int solve() {
	buckets.clear();

	cin >> m >> n >> K;
	for (int i = 1; i <= m; i++) {
		for (int j = 1; j <= n; j++) {
			cin >> a[i][j];
			buckets[a[i][j]].push_back({i, j});
		}
	}

	int low = 1, high = max(m, n), ans = high;
	while (low <= high) {
		int mid = (low + high) / 2;
		if (count_valid_configuration(mid) >= K) {
			ans = mid;
			high = mid - 1;
		} else {
			low = mid + 1;
		}
	}

	return ans;
}

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	
	cin >> T;
	for (int it = 1; it <= T; it++) {
		cerr << it << endl;
		cout << "Case #" << it << ": " << solve() << endl;
	}
	return 0;
}