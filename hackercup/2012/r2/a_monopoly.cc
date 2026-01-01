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
#define maxn 30005
using namespace std;

int T,n,d;
pair<int,int> order[maxn],newOrder[maxn];
int pos[maxn];
vector<int> bucket[maxn];

bool cmp(int x,int y) {
    return order[x] < order[y];
}

vector<int> refine(vector<int> mu) {
    vector<int> _mu;
    for (int i = 0; i < mu.size(); i++) {
        if (order[mu[i]].second > d) order[mu[i]].first = n + 8;
        _mu.push_back(mu[i]);
    }
    sort(_mu.begin(),_mu.end(),cmp);
    return _mu;
}

void merge(int mu,int mv) {
    if (bucket[mu].size() < bucket[mv].size()) swap(mu,mv);
    for (int i = 0; i < bucket[mv].size(); i++) {
        bucket[mu].push_back(bucket[mv][i]);
        pos[bucket[mv][i]] = mu;
    }
    bucket[mv].clear();
    sort(bucket[mu].begin(),bucket[mu].end(),cmp);
}

int main() {
    freopen("monopoly.in.2","r",stdin);
    freopen("monopoly.out.2","w",stdout);
    scanf("%d", &T);
    for (int it = 1; it <= T; it++) {
        fprintf(stderr,"%d\n", it);
        scanf("%d %d", &n, &d);
        for (int i = 1; i <= n; i++) {
            bucket[i].clear();
            bucket[i].push_back(i);
            order[i] = make_pair(1,0);
            pos[i] = i;
        }
        for (int i = 1; i < n; i++) {
            int u,v;
            scanf("%d %d", &u, &v);
            int mu = pos[u],mv = pos[v];
            int smu = order[bucket[mu][0]].first,smv = order[bucket[mv][0]].first;
            for (int j = 0; j < bucket[mu].size(); j++) {
                int x = bucket[mu][j];
                if (order[x].second >= d) newOrder[x] = make_pair(n + 8,d + 3); else {
                    newOrder[x].second = order[x].second + 1;
                    newOrder[x].first = max(1 + smv,order[x].first);
                }
            }
            for (int j = 0; j < bucket[mv].size(); j++) {
                int x = bucket[mv][j];
                if (order[x].second >= d) newOrder[x] = make_pair(n + 8,d + 3); else {
                    newOrder[x].second = order[x].second + 1;
                    newOrder[x].first = max(1 + smu,order[x].first);
                }
            }
            for (int j = 0; j < bucket[mu].size(); j++) order[bucket[mu][j]] = newOrder[bucket[mu][j]];
            for (int j = 0; j < bucket[mv].size(); j++) order[bucket[mv][j]] = newOrder[bucket[mv][j]];
            merge(mu,mv);
        }
        int ret = n;
        for (int i = 1; i <= n; i++) if (order[i].second <= d) ret = min(ret,order[i].first);
        printf("Case #%d: %d\n", it, ret);
    }
}
