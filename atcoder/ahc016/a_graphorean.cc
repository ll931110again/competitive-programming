// AHC016 A - Graphorean: https://atcoder.jp/contests/ahc016/tasks/ahc016_a
//
// Enumerate non-isomorphic graphs on N=5/6 vertices, greedily pick M with large
// pairwise edit distance, then classify each noisy query by best permutation match.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

struct Graph {
  int n = 0;
  vector<vector<uint8_t>> adj;

  explicit Graph(int n_) : n(n_), adj(n_, vector<uint8_t>(n_, 0)) {}

  static Graph from_upper_string(int n, const string& s) {
    Graph g(n);
    int p = 0;
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        g.adj[i][j] = g.adj[j][i] = (s[p++] == '1');
      }
    }
    return g;
  }

  string to_upper_string(const vector<int>& perm) const {
    string s;
    s.reserve(n * (n - 1) / 2);
    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        s.push_back(adj[perm[i]][perm[j]] ? '1' : '0');
      }
    }
    return s;
  }

  string to_upper_string() const {
    vector<int> id(n);
    iota(id.begin(), id.end(), 0);
    return to_upper_string(id);
  }
};

int edge_count(const Graph& g) {
  int m = 0;
  for (int i = 0; i < g.n; ++i) {
    for (int j = i + 1; j < g.n; ++j) {
      m += g.adj[i][j];
    }
  }
  return m;
}

int agreement(const Graph& h, const Graph& g, const vector<int>& perm) {
  int same = 0;
  for (int i = 0; i < h.n; ++i) {
    for (int j = i + 1; j < h.n; ++j) {
      if (h.adj[i][j] == g.adj[perm[i]][perm[j]])
        ++same;
    }
  }
  return same;
}

int best_agreement(const Graph& h, const Graph& g) {
  vector<int> perm(h.n);
  iota(perm.begin(), perm.end(), 0);
  int best = -1;
  do {
    best = max(best, agreement(h, g, perm));
  } while (next_permutation(perm.begin(), perm.end()));
  return best;
}

string canonical_key(const Graph& g) {
  vector<int> perm(g.n);
  iota(perm.begin(), perm.end(), 0);
  string best;
  do {
    const string s = g.to_upper_string(perm);
    if (best.empty() || s < best)
      best = s;
  } while (next_permutation(perm.begin(), perm.end()));
  return best;
}

Graph graph_from_mask(int n, int mask) {
  Graph g(n);
  int bit = 0;
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (mask & (1 << bit))
        g.adj[i][j] = g.adj[j][i] = 1;
      ++bit;
    }
  }
  return g;
}

vector<Graph> enumerate_non_isomorphic(int n) {
  const int e = n * (n - 1) / 2;
  map<string, Graph> uniq;
  for (int mask = 0; mask < (1 << e); ++mask) {
    Graph g = graph_from_mask(n, mask);
    uniq.emplace(canonical_key(g), move(g));
  }
  vector<Graph> out;
  out.reserve(uniq.size());
  for (auto& [_, g] : uniq)
    out.push_back(move(g));
  return out;
}

int choose_n(int m) {
  if (m <= 34)
    return 5;
  return 6;
}

vector<Graph> select_graphs(const vector<Graph>& pool, int m) {
  const int total = pool[0].n * (pool[0].n - 1) / 2;
  vector<int> order(pool.size());
  iota(order.begin(), order.end(), 0);
  sort(order.begin(), order.end(), [&](int a, int b) {
    return edge_count(pool[a]) < edge_count(pool[b]);
  });

  vector<int> picked;
  picked.push_back(order[0]);
  while ((int)picked.size() < m) {
    int best_i = -1;
    int best_d = -1;
    for (int i : order) {
      if (find(picked.begin(), picked.end(), i) != picked.end())
        continue;
      int min_d = total;
      for (int j : picked) {
        min_d = min(min_d, total - best_agreement(pool[i], pool[j]));
      }
      if (min_d > best_d) {
        best_d = min_d;
        best_i = i;
      }
    }
    if (best_i < 0)
      break;
    picked.push_back(best_i);
  }

  while ((int)picked.size() < m) {
    for (int i : order) {
      if (find(picked.begin(), picked.end(), i) != picked.end())
        continue;
      picked.push_back(i);
      if ((int)picked.size() == m)
        break;
    }
  }

  vector<Graph> out;
  out.reserve(m);
  for (int i : picked)
    out.push_back(pool[i]);
  return out;
}

int predict(const Graph& h, const vector<Graph>& gs) {
  int best_k = 0;
  int best_score = -1;
  for (int k = 0; k < static_cast<int>(gs.size()); ++k) {
    const int score = best_agreement(h, gs[k]);
    if (score > best_score) {
      best_score = score;
      best_k = k;
    }
  }
  return best_k;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int m;
  double eps;
  cin >> m >> eps;

  const int n = choose_n(m);
  const vector<Graph> pool = enumerate_non_isomorphic(n);
  const vector<Graph> gs = select_graphs(pool, m);

  cout << n << '\n';
  for (const Graph& g : gs)
    cout << g.to_upper_string() << '\n';
  cout.flush();

  for (int q = 0; q < 100; ++q) {
    string h_str;
    cin >> h_str;
    const Graph h = Graph::from_upper_string(n, h_str);
    cout << predict(h, gs) << '\n';
    cout.flush();
  }
  return 0;
}
