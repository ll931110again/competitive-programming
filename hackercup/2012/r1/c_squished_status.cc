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

int T,m;
char s[1005];
string encode;
long long f[1005];
long long mod = 4207849484LL;

int convert(string s) {
    stringstream w;
    w << s;
    int x;
    w >> x;
    return x;
}

long long rec(int pos) {
    if (pos < 0) return 1;
    if (f[pos] >= 0) return f[pos];
    f[pos] = 0;

    string store = "";
    for (int i = pos; i > pos - 3 && i >= 0; i--) {
        store = s[i] + store;
        if (s[i] == '0') continue;
        if (convert(store) <= m) f[pos] = (f[pos] + rec(i - 1)) % mod;
    }
    return f[pos];
}

int main() {
    freopen("encode.in.1","r",stdin);
    freopen("encode.out.1","w",stdout);
    scanf("%d\n", &T);
    for (int it = 1; it <= T; it++) {
        scanf("%d\n", &m);
        gets(s);
        encode = "";
        for (int i = 0; i < strlen(s); i++) if ('0' <= s[i] && s[i] <= '9') encode += s[i];
        memset(f,-1,sizeof(f));
        fprintf(stderr,"%d\n", m);
        printf("Case #%d: %lld\n", it, rec(encode.size() - 1));
    }
}
