#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#define maxn 5002
using namespace std;
 
vector<int> adj[maxn];
int n,child[maxn];
bool found[maxn];
bool seek[maxn];
vector<int> store;
 
void DFS(int u,int pre) {
    child[u] = 1;
    for (int i = 0; i < adj[u].size(); i++) {
        int v = adj[u][i];
        if (v == pre) continue;
        DFS(v,u);
        child[u] += child[v];
    }   
    vector<int> contain;
    for (int i = 0; i < adj[u].size(); i++) {
        int v = adj[u][i];
        if (v == pre) continue;
        contain.push_back(child[v]);        
    }
    if (u != 1) contain.push_back(n - child[u]);
    memset(seek,false,sizeof(seek));
    seek[0] = true;
    for (int i = 0; i < contain.size(); i++)
        for (int j = n - 1; j >= contain[i]; j--) seek[j] |= seek[j - contain[i]];
    for (int i = 0; i < n; i++) found[i] |= seek[i];
}
 
int main() {
    scanf("%d", &n);
    for (int i = 1; i < n; i++) {
        int u,v;
        scanf("%d %d", &u, &v);
        adj[u].push_back(v);
        adj[v].push_back(u);
    }
    DFS(1,-1);
    for (int i = 1; i <= n - 2; i++) if (found[i]) store.push_back(i);
    printf("%d\n", store.size());
    for (int i = 0; i < store.size(); i++) printf("%d %d\n", store[i], n - 1 - store[i]);
}
