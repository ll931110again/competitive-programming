#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif
 
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <random>
#include <stack>
#include <map>
#include <set>
#include <queue>
#include <vector>
#include <unordered_set>
#define maxn 40
#define maxm 800
using namespace std;
 
pair<int,int> edges[maxm];
int cnt[maxn];
int T, n, m;
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
 
    std::random_device rd;
    std::mt19937 g(rd());
 
    cin >> T;
    while (T--) {
        cin >> n >> m;\
        for (int i = 0; i < m; i++) {
            cin >> edges[i].first >> edges[i].second;
        }
 
        int ret = 0;
        for (int iter = 0; iter < 3e5; iter++) {
            shuffle(edges, edges + m, g);
            int value = 0;
 
            memset(cnt, 0, sizeof cnt);
            for(int i = 0; i < m; i++) {
                int u = edges[i].first, v = edges[i].second;
                if (cnt[u] >= 2 || cnt[v] >= 2) {
                    continue;
                }
                value++;
                cnt[u]++;
                cnt[v]++;
            }
 
            ret = max(ret, value);
        }
        cout << ret << endl;
    }
 
    return 0;
}
