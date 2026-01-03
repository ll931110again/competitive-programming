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

using namespace std;


class SlideCardsLeft {
public:
	vector <int> getPosition(int N, long long steps) {
		vector<int> ans(N);
		for (int i = 0; i < N; i++) {
			ans[i] = i;
		}
		
		for (int b = 60; b >= 0; b--) {
			if (steps & (1LL << b)) {
				if (b + 1 >= N) {
					return vector<int>();
				}
				swap(ans[b + 1], ans[0]);
			}
		}
		
		return ans;
	}
};



//Powered by KawigiEdit 2.1.4 (beta) modified by pivanof!