#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>
#include <cstring>
#define maxn 500005
using namespace std;

int n, m;
pair<int,int> a[maxn], b[maxn];
int nsmall[maxn], nbig[maxn];
int tx[4 * maxn];

void add(int i, int low, int high, int pos) {
	if (low == high) {
		tx[i]++;
		return;
	}
	int mid = (low + high) >> 1;
	if (pos <= mid) {
		add(2 * i, low, mid, pos);
	} else {
		add(2 * i + 1, mid + 1, high, pos);
	}
	tx[i] = tx[2 * i] + tx[2 * i + 1];
}

int query(int i, int low, int high, int u, int v) {
	if (v < low || high < u) {
		return 0;
	}
	if (u <= low && high <= v) {
		return tx[i];
	}
	int mid = (low + high) >> 1;
	return query(2 * i, low, mid, u, v) + query(2 * i + 1, mid + 1, high, u, v);
}

int main() {
	scanf("%d %d", &n, &m);
	for (int i = 1; i <= n; i++) {
		scanf("%d", &a[i].first);
		a[i].second = i;

		if (i <= m) {
			b[i] = a[i];
		}
	}
	if (m == 1) {
		printf("0\n");
		return 0;
	}

	sort(a + 1, a + n + 1);
	sort(b + 1, b + m + 1);
	int low = 1, high = 2;
	while (low <= n) {
		high = low + 1;
		while (high <= n && a[low].first == a[high].first) {
			high++;
		}
		for (int i = low; i < high; i++) {
			int lpos = a[i].second + 1;
			int rpos = lpos + m - 2;
			nsmall[a[i].second] = query(1, 1, n, lpos, rpos);
		}
		for (int i = low; i < high; i++) {
			add(1, 1, n, a[i].second);
		}
		low = high;
	}

	memset(tx, 0, sizeof tx);
	high = n;
	while (high > 0) {
		low = high - 1;
		while (low > 0 && a[low].first == a[high].first) {
			low--;
		}
		for (int i = low + 1; i <= high; i++) {
			int rpos = a[i].second - 1;
			int lpos = rpos - (m - 2);
			nbig[a[i].second] = query(1, 1, n, lpos, rpos);
		}

		for (int i = low + 1; i <= high; i++) {
			add(1, 1, n, a[i].second);
		}

		high = low;
	}

	/*for (int i = 1; i <= n; i++) {
		cout << nsmall[i] << ' ' << nbig[i] << endl;
	}*/

	long long ret = 0, inv_count = 0;
	memset(tx, 0, sizeof tx);
	for (int i = 1; i <= m; i++) {
		inv_count += query(1, 1, n, b[i].second + 1, m);
		add(1, 1, n, b[i].second);		
	}
	ret = inv_count;
	for (int i = m + 1; i <= n; i++) {
		inv_count += (nbig[i] - nsmall[i - m]);
		ret += inv_count;
	}
	printf("%lld\n", ret);
	return 0;
}