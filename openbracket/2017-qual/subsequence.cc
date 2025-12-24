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

string inp;
int K, pos = 0;
string ret;

int main() {
	cin >> inp >> K;
	for (int ch = 'z'; ch >= 'a'; ch--) {
		int ncount = 0, last_pos = -1;
		for (int i = pos; i < inp.size(); i++) if (inp[i] == ch) {
			ncount++;
			last_pos = i;
		}
		if (ncount >= K) {
			for (int i = 0; i < ncount; i++) {
				ret += ch;
			}
			pos = last_pos + 1;
		}
	}
	cout << ret << endl;
	return 0;
}