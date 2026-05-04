#include <algorithm>
#include <cstring>
#include <cmath>
#include <cstdio>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

string s;

int main() {
	ios_base::sync_with_stdio(false);
	cin.tie(NULL);

	cin >> s;
	cout << ((s == "00:00") ? "okay" : "poor") << endl;
	return 0;
}