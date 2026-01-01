#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <vector>
using namespace std;

map<int,int> mp;
int T,s;

int findComb(int x) {
    if (x == 1) return 1;
    if (mp.count(x)) return mp[x];
    for (int i = 1; i <= x; i++) {
        long long k = 1;
        for (int j = 1; j <= i; j++) {
            k = k * 1LL * (i + 1 - j)/j;
            if (k > x) break;
            if (k == x) return mp[x] = i;
        }
    }
}

int main() {
    freopen("checkpoint.in.1","r",stdin);
    freopen("checkpoint.out.1","w",stdout);
    scanf("%d\n", &T);
    for (int it = 1; it <= T; it++) {
        scanf("%d", &s);
        int ret = 2 * s;
        for (int i = 1; i * i <= s; i++) if (s % i == 0) ret = min(ret,findComb(i) + findComb(s/i));
        printf("Case #%d: %d\n", it, ret);
    }
}
