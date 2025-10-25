#include <algorithm>
#include <iostream>
#include <vector>
#define maxn 1000005
using namespace std;

vector<int> adj[maxn];
long long ss[maxn];
int weight[maxn];

long long minPopulation = 1e18;
int bestLocation = -1;

void DFS(int u, int par) {
    ss[u] = weight[u];
    for (auto v : adj[u]) if (v != par) {
        DFS(v, u);
        ss[u] += ss[v];
    }
}

void findBestCity(int u, int par) {
    long long value = ss[0] - ss[u];
    for (auto v : adj[u]) if (v != par) {
        value = max(value, ss[v]);
        findBestCity(v, u);
    }

    if (minPopulation > value) {
        minPopulation = value;
        bestLocation = u;
    }
}

int LocateCentre(int N, int P[], int S[], int D[]) {
    for (int i = 0; i < N - 1; i++) {
        adj[S[i]].push_back(D[i]);
        adj[D[i]].push_back(S[i]);
    }

    for (int i = 0; i < N; i++) {
        weight[i] = P[i];
    }

    DFS(0, -1);
    findBestCity(0, -1);
    return bestLocation;
}

int main() {
    int n = 5;
    int p[] = {10, 10, 10, 10, 10};
    int S[] = {0, 1, 2, 3};
    int D[] = {1, 2, 3, 4};
    cout << LocateCentre(n, p, S, D) << endl;
    return 0;
}