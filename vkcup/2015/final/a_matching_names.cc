/*
 * Solution for problem A: Matching Names.
 *
 * Insert all the strings into a prefix trie. Mark them with red and blue colors
 * (there could be multiple red and blue values in a terminal node).
 * 
 * Now process on the tree. At a given node, we'd want to greedily match the red and blue
 * color node at maximum depth as possible. So in a given node, we will:
 *  - First, try to find and match all possible matchings at the subtree level.
 *  - Then, match all the residuals at the node level.
 * 
 * To implement this, for a node, we keep track of the red indices and blue indices which
 * have not found their matchings yet. Then we aggregate the info from that node and from its
 * children together, doing small to large merging and eliminating pairs of opposite colors. 
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
#include <vector>
#define maxn 100005
#define maxk 800005
using namespace std;

int n;
string a[maxn], b[maxn];

struct node {
    vector<int> a_indices;
    vector<int> b_indices;
    vector<int> nxt;
    int depth;
};

node new_node(int depth) {
    node x;
    x.a_indices.clear();
    x.b_indices.clear();
    x.nxt.resize(26);
    for (int i = 0; i < 26; i++) {
        x.nxt[i] = -1;
    }
    x.depth = depth;

    return x;
}

vector<node> nodes;
int refs[maxk];
int matchings[maxn], total_value = 0;
int node_count = 1;

void add(string s, int idx, int color) {
    int root = 0;
    for (auto ch : s) {
        int pos = ch - 'a';
        if (nodes[root].nxt[pos] < 0) {
            nodes.push_back(new_node(nodes[root].depth + 1));
            nodes[root].nxt[pos] = node_count++;
        }
        root = nodes[root].nxt[pos];
    }
    if (!color) {
        nodes[root].a_indices.push_back(idx);
    } else {
        nodes[root].b_indices.push_back(idx);
    }
}

void merge(int iu, int iv) {
    int u = refs[iu];
    int v = refs[iv];

    int sz_u = nodes[u].a_indices.size() + nodes[u].b_indices.size();
    int sz_v = nodes[v].a_indices.size() + nodes[v].b_indices.size();

    // Do small to large merging
    if (sz_u > sz_v) {
        refs[iv] = u;
        swap(u, v);
    } else {
        refs[iu] = v;
    }

    for (auto x : nodes[u].a_indices) {
        nodes[v].a_indices.push_back(x);
    }
    for (auto x : nodes[u].b_indices) {
        nodes[v].b_indices.push_back(x);
    }

    nodes[u].a_indices.clear();
    nodes[u].b_indices.clear();
}

void check_matchings(int node, int depth) {
    while (!nodes[node].a_indices.empty() && !nodes[node].b_indices.empty()) {
        int a_value = nodes[node].a_indices.back();
        int b_value = nodes[node].b_indices.back();
        total_value += depth;
        matchings[a_value] = b_value;

        nodes[node].a_indices.pop_back();
        nodes[node].b_indices.pop_back();
    }
}

void build_answer(int node) {
    for (int i = 0; i < 26; i++) if (nodes[node].nxt[i] >= 0) {
        build_answer(nodes[node].nxt[i]);
    }

    check_matchings(refs[node], nodes[node].depth);
    for (int i = 0; i < 26; i++) if (nodes[node].nxt[i] >= 0) {
        // merge i and the children
        merge(node, nodes[node].nxt[i]);
        check_matchings(refs[node], nodes[node].depth);
    }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> n;
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  for (int i = 0; i < n; i++) {
    cin >> b[i];
  }

  nodes.push_back(new_node(0));
  for (int i = 0; i < n; i++) {
    add(a[i], i, 0);
    add(b[i], i, 1);
  }
  for (int i = 0; i < node_count; i++) {
    refs[i] = i;
  }

  build_answer(0);
  cout << total_value << endl;
  for (int i = 0; i < n; i++) {
    cout << i + 1 << ' ' << matchings[i] + 1 << endl;
  }

  return 0;
}
