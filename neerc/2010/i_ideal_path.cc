/*
 * Solution for problem I: Ideal path
 *
 * We compute the shortest path from 1 to each node u, and the shortest path from each node u to n.
 *
 * Then we compute the lexicographically minimal path stepwise. In each step, we find out the minimum
 * color that could result to a valid path. An edge u -> v of color c is valid if v belongs to a shortest path
 * from 1 to n. To check that, we simply check d_from[v] + d_to[v] == d_from[n].
 */

#ifdef ONLINE_JUDGE
  #include <bits/stdc++.h>
#endif
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
#include <unordered_set>
#include <vector>
#define maxn 100005
using namespace std;

int n, m;
vector< pair<int,int> > adj[maxn];

int d_from[maxn], d_to[maxn];

void BFS() {
    for (int i = 1; i <= n; i++) {
        d_from[i] = d_to[i] = -1;
    }
    d_from[1] = 0;
    d_to[n] = 0;

    queue<int> q;
    q.push(1);

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (auto it : adj[u]) {
            int v = it.first;
            if (d_from[v] < 0) {
                d_from[v] = 1 + d_from[u];
                q.push(v);
            }
        }
    }

    q.push(n);
    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (auto it : adj[u]) {
            int v = it.first;
            if (d_to[v] < 0) {
                d_to[v] = 1 + d_to[u];
                q.push(v);
            }
        }
    }
}

vector<int> compute_path() {
    vector<int> path;
    unordered_set<int> frontiers;
    frontiers.insert(1);

    int target_dist = d_from[n];
    while (true) {
        unordered_set<int> next_frontiers;
        int min_color = -1;

        for (auto u : frontiers) {
            for (auto it : adj[u]) {
                int v = it.first, color = it.second;
                if (d_from[v] == 1 + d_from[u] && d_from[v] + d_to[v] == target_dist) {
                    if (min_color < 0 || min_color > color) {
                        next_frontiers.clear();
                        next_frontiers.insert(v);
                        min_color = color;
                    } else if (min_color == color) {
                        next_frontiers.insert(v);
                    }
                }
            }
        }

        path.push_back(min_color);
        if (next_frontiers.find(n) != next_frontiers.end()) {
            break;
        }

        frontiers = next_frontiers;
    }

    return path;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("ideal.in", "r", stdin);
  freopen("ideal.out", "w", stdout);

  cin >> n >> m;
  for (int i = 0; i < m; i++) {
    int u, v, c;
    cin >> u >> v >> c;
    adj[u].push_back({v, c});
    adj[v].push_back({u, c});
  }

  BFS();

  auto path = compute_path();
  cout << path.size() << endl;
  for (auto x : path) {
    cout << x << ' ';
  }
  cout << endl;

  return 0;
}
