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
#define maxn 255
using namespace std;

vector< pair<double, double> > leaves;
pair<double, double> a[maxn];
vector<int> adj[maxn], weight;
int n;

class BalancingTrees {
public:
	double minCost(vector <int> p, vector <int> w) {
		leaves.clear();
		n = p.size() + 1;
		for (int i = 0; i < n; i++) {
			adj[i].clear();
		}
		for (int i = 0; i < p.size(); i++) {
			adj[p[i]].push_back(i + 1);
		}
		weight = w;
		
		a[0] = make_pair(1, 0);
		dfs(0);
		
		for (int i = 0; i < n; i++) if (!adj[i].size()) {
			leaves.push_back(make_pair(a[i].first, a[i].second - weight[i]));
		}
		
		double low = -1e12, high = 1e12;
		for (int iter = 0; iter < 250; iter++) {
			double mlow = (low + low + high)/3, mhigh = (low + high + high)/3;
			if (calc(mlow) < calc(mhigh)) {
				high = mhigh;
			} else {
				low = mlow;
			}
		}
		return calc(low);
	}
	
	double calc(double x) {
		double ans = 0;
		for (auto p : leaves) {
			ans += abs(p.first * x + p.second);
		}
		return ans;
	}
	
	void dfs(int u) {
		for (auto v : adj[u]) {
			a[v] = make_pair(a[u].first / adj[u].size(), (a[u].second - weight[u]) / adj[u].size());
			dfs(v);
		}
	}
};


<%:testing-code%>
//Powered by KawigiEdit 2.1.4 (beta) modified by pivanof!