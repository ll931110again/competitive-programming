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
using namespace std;

vector< vector<int> > a, breakpoint, pos;
vector<int> idx_chain, idx_value;
vector<long long> sum_chain;
int m, n;

int main() {
	scanf("%d %d", &m, &n);
	a.resize(m);
	breakpoint.resize(m);
	pos.resize(m);

	for (int i = 0; i < m; i++) {
		a[i].resize(n);
		breakpoint[i].resize(n);
		pos[i].resize(n);
		
		for (int j = 0; j < n; j++) {
			scanf("%d", &a[i][j]);
		}
	}

	for (int j = n - 1; j >= 0; j--) {
		for (int i = 0; i < m; i++) {
			pos[i][j] = (j == n - 1 || a[i][j] > a[i][j + 1]) ? j : pos[i][j + 1];
		}
	}
	for (int j = 0; j < n; j++) {
		for (int i = 0; i + 1 < m; i++) {
			breakpoint[i][j] = j - 1;
			int low = j, high = pos[i][j];
			while (low <= high) {
				int mid = (low + high) >> 1;
				if (a[i][mid] <= a[i + 1][j]) {
					breakpoint[i][j] = mid;
					low = mid + 1;
				} else {
					high = mid - 1;
				}
			}
		}
	}

	long long ret = 0;
	idx_value.resize(m + 1);
	idx_value[0] = -2;

	idx_chain.resize(m + 1);
	idx_chain[0] = -1;

	sum_chain.resize(m + 1);
	sum_chain[0] = 0;

	for (int j = 0; j < n; j++) {
		int idx_pos = 1;
		for (int i = 0; i < m; i++) {
			int low = 0, high = idx_pos - 1, low_pos = i - 1, ans = idx_pos - 1;
			// query pos[i][j]
			while (low <= high) {
				int mid = (low + high) >> 1;
				if (idx_value[mid] >= pos[i][j]) {
					ans = mid - 1;
					low_pos = idx_chain[mid - 1];
					high = mid - 1;
				} else {
					low = mid + 1;
				}
			}
			long long ssum = sum_chain[ans] + 1LL * (i - low_pos) * pos[i][j];
			// cout << "E: " << sum_chain[ans] << ' ' << 1LL * (i - low_pos) * pos[i][j] << endl;
			ssum -= 1LL * (i + 1) * (j - 1);
			ret += ssum;
			// cout << "R: " << ret << ' ' << ans << ' ' << low_pos << ' ' << pos[i][j] << ' ' << breakpoint[i][j] << endl;

			// insert breakpoint[i][j]
			if (i == m - 1) continue;
			while (idx_pos > 0 && idx_value[idx_pos - 1] >= breakpoint[i][j]) {
				idx_pos--;
			}
			idx_value[idx_pos] = breakpoint[i][j];
			idx_chain[idx_pos] = i;
			sum_chain[idx_pos] = sum_chain[idx_pos - 1] + 
				1LL * (idx_chain[idx_pos] - idx_chain[idx_pos - 1]) * idx_value[idx_pos];
			// cout << idx_pos << ' ' << sum_chain[idx_pos] << endl;
			idx_pos++;
		}
	}

	printf("%lld\n", ret);
	return 0;
}