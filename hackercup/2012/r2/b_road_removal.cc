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
#define maxn 10005
#define maxm 50005
using namespace std;

int T,n,m,k;
int ma[maxm],mb[maxm];
int pre[maxn],rank[maxn],ret;
bool import[maxn];

int root(int x) {
    if (x != pre[x]) pre[x] = root(pre[x]);
    return pre[x];
}

void link(int u,int v) {
    u = root(u);
    v = root(v);
    if (u == v) return;
    if (rank[u] > rank[v]) {
        import[u] |= import[v];
        pre[v] = u;
    }
    else {
        import[v] |= import[u];
        pre[u] = v;
    }
    if (rank[u] == rank[v]) rank[v]++;
}

void checkLink(int u,int v) {
    u = root(u);
    v = root(v);
    if (u == v && import[u]) {
        ret++;
        return;
    }
    link(u,v);
}

int main() {
    freopen("road.in.2","r",stdin);
    freopen("road.out.2","w",stdout);
    scanf("%d", &T);
    for (int it = 1; it <= T; it++) {
        scanf("%d %d %d", &n, &m, &k);
        for (int i = 0; i < m; i++) scanf("%d %d", &ma[i], &mb[i]);
        for (int i = 0; i < n; i++) pre[i] = i,rank[i] = 0;
        for (int i = 0; i < n; i++) import[i] = (i < k);
        for (int i = 0; i < m; i++) if (ma[i] >= k && mb[i] >= k) link(ma[i],mb[i]);
        ret = 0;
        for (int i = 0; i < m; i++) if (ma[i] < k || mb[i] < k) checkLink(ma[i],mb[i]);
        printf("Case #%d: %d\n", it, ret);
    }
}
