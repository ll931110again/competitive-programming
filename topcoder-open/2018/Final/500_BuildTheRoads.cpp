#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#define maxn 202
using namespace std;

double d[maxn][maxn];
int n;

class BuildTheRoads {
public:
	double minimalCost(vector <int> x, vector <int> y) {
		n = x.size();
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				d[i][j] = sqrt((x[i] - x[j]) * (x[i] - x[j]) + (y[i] - y[j]) * (y[i] - y[j]));
			}
		}
		
		double ret = 1e9;
		for (int a = 0; a < n; a++) {
			for (int b = 0; b < n; b++) {
				ret = min(ret, solve(a, b));
			}
		}
		return ret;
	}
	
	double solve(int s, int t) {
		pair<double, double> p[maxn];
		int k = 0;
		
		for (int i = 0; i < n; i++) if (i != s && i != t) {
			p[k++] = make_pair(d[s][i], d[t][i]);
		}
		
		sort(p, p + k);
		
		double ans = 1e9;
		double maxy = 0;
		for (int i = k; i >= 0; i--) {
			if (i < k) {
				maxy = max(maxy, p[i].second);
			}
			double maxx = 0;
			if (i > 0) {
				maxx = p[i - 1].first;
			}
			double tmp = max(2.0 * max(maxx, maxy), maxx + maxy + d[s][t]);
			ans = min(ans, tmp);
		}
		return ans;
	}
};


<%:testing-code%>
//Powered by KawigiEdit 2.1.4 (beta) modified by pivanof!