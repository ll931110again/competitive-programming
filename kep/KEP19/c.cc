#include <algorithm>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
#define maxn 55
using namespace std;

int n;
int scores[maxn];
int test_cnt[maxn];
bool ok[maxn];

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> n;
	for (int i = 0; i < n; i++) {
		cin >> scores[i];
	}

	for (int i = 0; i < n; i++) {
		cin >> test_cnt[i];
	}

	int total_score = 0;
	for (int i = 0; i < n; i++) {
		ok[i] = true;
		for (int j = 0; j < test_cnt[i]; j++) {
			int x;
			cin >> x;
			ok[i] &= x;
		}
		if (ok[i]) {
			total_score += scores[i];
		}
	}

	cout << total_score << endl;
	return 0;
}