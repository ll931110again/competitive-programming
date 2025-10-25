#include <cstring>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include "grader.h"
#define maxn 1005
#define maxh 36
using namespace std;

vector<int> adj[maxn];
int dist[maxn][maxh];
int par[maxn];

int decoded_delta[maxn];
int decoded_dist[maxn];

void encode_integer(int X) {
    for (int mask = 0; mask < 10; mask++) if (X & (1 << mask)) {
        encode_bit(1);
    } else {
        encode_bit(0);
    }
}

int decode_integer() {
    int value = 0;
    for (int mask = 0; mask < 10; mask++) {
        int bit = decode_bit();
        if (bit) {
            value |= (1 << mask);
        }
    }
    return value;
}

void encode(int N, int H, int P, int A[], int B[]) {
    memset(dist, -1, sizeof dist);
    for (int i = 0; i < P; i++) {
        adj[A[i]].push_back(B[i]);
        adj[B[i]].push_back(A[i]);
    }

    for (int i = 0; i < H; i++) {
        queue<int> q;
        q.push(i);

        dist[i][i] = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (auto v : adj[u]) {
                if (dist[v][i] == -1 || dist[v][i] > 1 + dist[u][i]) {
                    dist[v][i] = 1 + dist[u][i];
                    q.push(v);
                }

                if (i == 0 && par[v] == -1) {
                    par[v] = u;
                }
            }
        }
    }

    /*
        82000 bits
        First, send the parent-child relationship
        Then, for each h, first send dist[h][0]. For each further i, send dist[h][i] - dist[h][par[i]]
    */

    for (int i = 1; i < N; i++) {
        encode_integer(par[i]);
    }

    for (int i = 0; i < H; i++) {
        encode_integer(dist[0][i]);
        for (int x = 1; x < N; x++) {
            int p = par[x], delta = dist[x][i] - dist[p][i];
            if (delta == 0) {
                encode_bit(0);
                encode_bit(0);
            } else if (delta == 1) {
                encode_bit(0);
                encode_bit(1);
            } else if (delta == -1) {
                encode_bit(1);
                encode_bit(0);
            }
        }
    }
}

void fill_distance(int u, int p) {
    for (auto v : adj[u]) if (v != p) {
        decoded_dist[v] = decoded_dist[u] + decoded_delta[v];
        fill_distance(v, u);
    }
}

void decode(int N, int H) {
    for (int i = 0; i < N; i++) {
        adj[i].clear();
    }

    // Construct the parent-child relationship
    for (int i = 1; i < N; i++) {
        par[i] = decode_integer();
        adj[par[i]].push_back(i);
    }

    for (int i = 0; i < H; i++) {
        decoded_dist[0] = decode_integer();
        for (int x = 1; x < N; x++) {
            int x0 = decode_bit(), x1 = decode_bit();
            if (x0 == 0 && x1 == 0) {
                decoded_delta[x] = 0;
            } else if (x0 == 0 && x1 == 1) {
                decoded_delta[x] = 1;
            } else {
                decoded_delta[x] = -1;
            }
        }

        fill_distance(0, -1);

        for (int x = 0; x < N; x++) {
            hops(i, x, decoded_dist[x]);
        }
    }
}
