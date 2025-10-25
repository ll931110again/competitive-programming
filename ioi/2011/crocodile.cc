#include <algorithm>
#include <vector>
#include <iostream>
#include <functional>
#include <queue>
#define maxn 100005
using namespace std;
using QueueItem = pair<int,int>;
 
 
vector<QueueItem> adj[maxn];
QueueItem dist[maxn];
int ans[maxn];
bool vis[maxn];
int inf = 1e9 + 2;
 
int travel_plan(int n, int m, int R[][2], int L[], int K, int P[]) {
    for (int i = 0; i < m; i++) {
        int u = R[i][0], v = R[i][1], c = L[i];
        adj[u].push_back(make_pair(v, c));
        adj[v].push_back(make_pair(u, c));
    }
 
    for (int i = 0; i < n; i++) {
        dist[i] = {inf, inf};
    }
    for (int i = 0; i < K; i++) {
        dist[P[i]] = {0, 0};
        ans[i] = 0;
    }
 
    priority_queue< QueueItem, vector<QueueItem>, std::greater<QueueItem> > pq;
    for (int i = 0; i < K; i++) {
        pq.push({0, P[i]});
    }
 
    while (!pq.empty()) {
        auto u = pq.top().second;
        pq.pop();
 
        if (vis[u]) {
            continue;
        }
        vis[u] = true;
        ans[u] = dist[u].second;
 
        for (auto it : adj[u]) {
            int v = it.first, c = it.second;
            int new_cost = ans[u] + c;
            if (new_cost < dist[v].first) {
                dist[v] = {new_cost, dist[v].first};
                pq.push({dist[v].second, v});
            } else if (new_cost < dist[v].second) {
                dist[v] = {dist[v].first, new_cost};
                pq.push({dist[v].second, v});
            }
        }
    }
 
    return ans[0];
} 
