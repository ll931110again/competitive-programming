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

string suffix = "hogwarts.com";
string inp;

int main() {
	cin >> inp;
	int idx = -1;
	for (int i = 0; i < inp.size(); i++) if (inp[i] == '@') {
		idx = i;
		break;
	}

	if (idx != 5) {
		printf("No\n");
		return 0;
	}
	
	if (inp.substr(6) != suffix) {
		printf("No\n");
		return 0;
	}
	for (int i = 0; i < 5; i++) {
		if (inp[i] < 'a' || 'z' < inp[i]) {
			printf("No\n");
			return 0;
		}
	}
	printf("Yes\n");
	return 0;
}