#include "grader.h"
#include <bits/stdc++.h>
using namespace std;

namespace {

vector<int> adj[1005];
int dist[1005][36];
int par[1005];
int decoded_delta[1005];
int decoded_dist[1005];

int ternary_buf[3];
int ternary_len;

void encode_integer(int x) {
  for (int b = 0; b < 10; ++b) {
    encode_bit(x & 1);
    x >>= 1;
  }
}

int decode_integer() {
  int value = 0;
  for (int b = 0; b < 10; ++b) {
    if (decode_bit()) {
      value |= 1 << b;
    }
  }
  return value;
}

void flush_ternary() {
  if (ternary_len == 0) {
    return;
  }
  while (ternary_len < 3) {
    ternary_buf[ternary_len++] = 0;
  }
  int val = ternary_buf[0] * 9 + ternary_buf[1] * 3 + ternary_buf[2];
  for (int b = 0; b < 5; ++b) {
    encode_bit(val & 1);
    val >>= 1;
  }
  ternary_len = 0;
}

void encode_delta(int delta) {
  ternary_buf[ternary_len++] = delta + 1;
  if (ternary_len == 3) {
    flush_ternary();
  }
}

int decode_buf[3];
int decode_pos;

int decode_delta() {
  if (decode_pos == 0) {
    int val = 0;
    for (int b = 0; b < 5; ++b) {
      if (decode_bit()) {
        val |= 1 << b;
      }
    }
    decode_buf[0] = val / 9;
    val %= 9;
    decode_buf[1] = val / 3;
    decode_buf[2] = val % 3;
    decode_pos = 3;
  }
  return decode_buf[3 - decode_pos--] - 1;
}

void fill_distance(int u, int p) {
  for (int v : adj[u]) {
    if (v == p) {
      continue;
    }
    decoded_dist[v] = decoded_dist[u] + decoded_delta[v];
    fill_distance(v, u);
  }
}

void encode(int n, int h, int p, int a[], int b[]) {
  for (int i = 0; i < n; ++i) {
    adj[i].clear();
  }
  memset(par, -1, sizeof par);
  par[0] = 0;
  ternary_len = 0;

  for (int i = 0; i < p; ++i) {
    adj[a[i]].push_back(b[i]);
    adj[b[i]].push_back(a[i]);
  }

  queue<int> q;
  q.push(0);
  while (!q.empty()) {
    const int u = q.front();
    q.pop();
    for (int v : adj[u]) {
      if (par[v] == -1) {
        par[v] = u;
        q.push(v);
      }
    }
  }

  memset(dist, -1, sizeof dist);
  for (int i = 0; i < h; ++i) {
    dist[i][i] = 0;
    q.push(i);
    while (!q.empty()) {
      const int u = q.front();
      q.pop();
      for (int v : adj[u]) {
        if (dist[v][i] == -1) {
          dist[v][i] = dist[u][i] + 1;
          q.push(v);
        }
      }
    }
  }

  for (int i = 1; i < n; ++i) {
    encode_integer(par[i]);
  }

  for (int i = 0; i < h; ++i) {
    for (int x = 1; x < n; ++x) {
      encode_delta(dist[x][i] - dist[par[x]][i]);
    }
    flush_ternary();
  }
}

void decode(int n, int h) {
  for (int i = 0; i < n; ++i) {
    adj[i].clear();
  }
  decode_pos = 0;

  for (int i = 1; i < n; ++i) {
    par[i] = decode_integer();
    adj[par[i]].push_back(i);
  }

  int dist0[1005];
  queue<int> q;
  memset(dist0, -1, sizeof dist0);
  dist0[0] = 0;
  q.push(0);
  while (!q.empty()) {
    const int u = q.front();
    q.pop();
    for (int v : adj[u]) {
      if (dist0[v] == -1) {
        dist0[v] = dist0[u] + 1;
        q.push(v);
      }
    }
  }

  for (int i = 0; i < h; ++i) {
    decoded_dist[0] = dist0[i];
    for (int x = 1; x < n; ++x) {
      decoded_delta[x] = decode_delta();
    }
    decode_pos = 0;

    fill_distance(0, -1);

    for (int x = 0; x < n; ++x) {
      hops(i, x, decoded_dist[x]);
    }
  }
}

} // namespace
