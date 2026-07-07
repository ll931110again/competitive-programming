// AHC005 A - Patrolling: https://atcoder.jp/contests/ahc005/tasks/ahc005_a
//
// Enumerate straight road corridors between graph nodes, order them with greedy
// + 2-opt SA, then output shortest paths between chosen endpoints (full coverage).

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int DR[4] = {-1, 1, 0, 0};
constexpr int DC[4] = {0, 0, -1, 1};
constexpr char MV[4] = {'U', 'D', 'L', 'R'};
constexpr i64 INF = (1LL << 60);

int N, SI, SJ;
vector<string> G;
int ROADS;

struct Corridor {
  int na, nb;
  int ar, ac, br, bc;
};

vector<pair<int, int>> nodes;
vector<vector<int>> node_id;
vector<Corridor> corridors;
vector<vector<i64>> dist;
vector<vector<vector<pair<int, int>>>> par_from;

bool road(int r, int c) { return G[r][c] != '#'; }

int degree(int r, int c) {
  int d = 0;
  for (int k = 0; k < 4; ++k) {
    const int nr = r + DR[k], nc = c + DC[k];
    if (nr >= 0 && nr < N && nc >= 0 && nc < N && road(nr, nc))
      ++d;
  }
  return d;
}

bool is_node(int r, int c) {
  if (!road(r, c))
    return false;
  if (r == SI && c == SJ)
    return true;
  const int d = degree(r, c);
  if (d != 2)
    return true;
  vector<pair<int, int>> nb;
  for (int k = 0; k < 4; ++k) {
    const int nr = r + DR[k], nc = c + DC[k];
    if (nr >= 0 && nr < N && nc >= 0 && nc < N && road(nr, nc))
      nb.emplace_back(nr, nc);
  }
  if ((int)nb.size() != 2)
    return true;
  return !(nb[0].first + nb[1].first == 2 * r && nb[0].second + nb[1].second == 2 * c);
}

void collect_nodes() {
  node_id.assign(N, vector<int>(N, -1));
  nodes.clear();
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (!is_node(i, j))
        continue;
      node_id[i][j] = (int)nodes.size();
      nodes.emplace_back(i, j);
    }
  }
}

void collect_corridors() {
  corridors.clear();
  set<pair<int, int>> seen;
  for (const auto [r, c] : nodes) {
    const int u = node_id[r][c];
    for (int d = 0; d < 4; ++d) {
      int nr = r + DR[d], nc = c + DC[d];
      if (nr < 0 || nr >= N || nc < 0 || nc >= N || !road(nr, nc))
        continue;
      while (!is_node(nr, nc)) {
        nr += DR[d];
        nc += DC[d];
      }
      const int v = node_id[nr][nc];
      if (u == v)
        continue;
      const int a = min(u, v), b = max(u, v);
      if (seen.count({a, b}))
        continue;
      seen.insert({a, b});
      Corridor cor;
      cor.na = u;
      cor.nb = v;
      cor.ar = nodes[u].first;
      cor.ac = nodes[u].second;
      cor.br = nodes[v].first;
      cor.bc = nodes[v].second;
      corridors.push_back(cor);
    }
  }
}

void build_all_pairs() {
  const int V = (int)nodes.size();
  dist.assign(V, vector<i64>(V, INF));
  par_from.assign(V, vector<vector<pair<int, int>>>(N, vector<pair<int, int>>(N, {-1, -1})));
  for (int s = 0; s < V; ++s) {
    const auto [sr, sc] = nodes[s];
    vector<vector<i64>> d(N, vector<i64>(N, INF));
    auto& par = par_from[s];
    d[sr][sc] = 0;
    using P = pair<i64, pair<int, int>>;
    priority_queue<P, vector<P>, greater<P>> pq;
    pq.emplace(0, pair<int, int>{sr, sc});
    while (!pq.empty()) {
      const auto [cost, pos] = pq.top();
      pq.pop();
      const auto [r, c] = pos;
      if (cost != d[r][c])
        continue;
      for (int k = 0; k < 4; ++k) {
        const int nr = r + DR[k], nc = c + DC[k];
        if (nr < 0 || nr >= N || nc < 0 || nc >= N || !road(nr, nc))
          continue;
        const i64 nd = cost + (G[nr][nc] - '0');
        if (nd < d[nr][nc]) {
          d[nr][nc] = nd;
          par[nr][nc] = {r, c};
          pq.emplace(nd, pair<int, int>{nr, nc});
        }
      }
    }
    for (int t = 0; t < V; ++t) {
      const auto [tr, tc] = nodes[t];
      dist[s][t] = d[tr][tc];
    }
  }
}

string path_between_cells(int src_node, pair<int, int> src, pair<int, int> dst) {
  if (src == dst)
    return {};
  const auto [tr, tc] = dst;
  const auto& par = par_from[src_node];
  vector<pair<int, int>> chain;
  for (auto cur = pair<int, int>{tr, tc}; cur != src; cur = par[cur.first][cur.second]) {
    if (cur.first < 0)
      return {};
    chain.push_back(cur);
  }
  chain.push_back(src);
  reverse(chain.begin(), chain.end());
  string out;
  for (int i = 0; i + 1 < (int)chain.size(); ++i) {
    const auto [r0, c0] = chain[i];
    const auto [r1, c1] = chain[i + 1];
    if (r1 == r0 - 1)
      out.push_back('U');
    else if (r1 == r0 + 1)
      out.push_back('D');
    else if (c1 == c0 - 1)
      out.push_back('L');
    else
      out.push_back('R');
  }
  return out;
}

string path_between(int a, int b) {
  return path_between_cells(a, nodes[a], nodes[b]);
}

struct TourStep {
  int cid;
  int end;  // 0 -> na, 1 -> nb
};

i64 tour_cost(const vector<TourStep>& tour, int start_node) {
  if (corridors.empty())
    return 0;
  i64 cost = 0;
  int cur = start_node;
  for (const auto& st : tour) {
    const auto& cor = corridors[st.cid];
    const int tgt = st.end == 0 ? cor.na : cor.nb;
    cost += dist[cur][tgt];
    cur = tgt;
  }
  cost += dist[cur][start_node];
  return cost;
}

vector<TourStep> greedy_tour(int start_node, mt19937* rng = nullptr) {
  vector<char> used(corridors.size(), 0);
  vector<TourStep> tour;
  tour.reserve(corridors.size());
  int cur = start_node;

  for (int k = 0; k < (int)corridors.size(); ++k) {
    vector<tuple<i64, int, int>> cand;
    for (int i = 0; i < (int)corridors.size(); ++i) {
      if (used[i])
        continue;
      const auto& cor = corridors[i];
      const i64 d0 = dist[cur][cor.na];
      const i64 d1 = dist[cur][cor.nb];
      cand.emplace_back(d0, i, 0);
      cand.emplace_back(d1, i, 1);
    }
    sort(cand.begin(), cand.end());
    const i64 best_d = get<0>(cand[0]);
    vector<tuple<i64, int, int>> ties;
    for (const auto& x : cand) {
      if (get<0>(x) == best_d)
        ties.push_back(x);
      else
        break;
    }
    const auto pick =
        rng ? ties[uniform_int_distribution<int>(0, (int)ties.size() - 1)(*rng)] : ties[0];
    const int best_c = get<1>(pick);
    const int best_e = get<2>(pick);
    used[best_c] = 1;
    tour.push_back({best_c, best_e});
    cur = best_e == 0 ? corridors[best_c].na : corridors[best_c].nb;
  }
  return tour;
}

void improve_two_opt(vector<TourStep>& tour, int start_node,
                     chrono::steady_clock::time_point deadline) {
  const int m = (int)tour.size();
  if (m < 2)
    return;
  bool improved = true;
  while (improved && chrono::steady_clock::now() < deadline) {
    improved = false;
    for (int i = 0; i < m - 1 && !improved; ++i) {
      for (int j = i + 1; j < m; ++j) {
        vector<TourStep> cand = tour;
        reverse(cand.begin() + i, cand.begin() + j + 1);
        for (int k = i; k <= j; ++k)
          cand[k].end ^= 1;
        const i64 old_c = tour_cost(tour, start_node);
        const i64 new_c = tour_cost(cand, start_node);
        if (new_c < old_c) {
          tour = std::move(cand);
          improved = true;
          break;
        }
      }
    }
  }
}

void sa_two_opt(vector<TourStep>& tour, int start_node,
                chrono::steady_clock::time_point deadline) {
  if (tour.empty())
    return;
  mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());
  i64 cur = tour_cost(tour, start_node);
  const int m = (int)tour.size();
  double temp = 5000.0;
  while (chrono::steady_clock::now() < deadline) {
    if (m < 2)
      break;
    const int i = uniform_int_distribution<int>(0, m - 2)(rng);
    const int j = uniform_int_distribution<int>(i + 1, m - 1)(rng);
    vector<TourStep> cand = tour;
    reverse(cand.begin() + i, cand.begin() + j + 1);
    for (int k = i; k <= j; ++k)
      cand[k].end ^= 1;
    const i64 nxt = tour_cost(cand, start_node);
    const i64 delta = nxt - cur;
    if (delta < 0 || bernoulli_distribution(exp(-delta / temp))(rng)) {
      tour = std::move(cand);
      cur = nxt;
    }
    temp *= 0.999995;
    if (temp < 1e-3)
      temp = 1e-3;
  }
}

string build_output(const vector<TourStep>& tour, int start_node) {
  string out;
  int cur = start_node;
  for (const auto& st : tour) {
    const auto& cor = corridors[st.cid];
    const int tgt = st.end == 0 ? cor.na : cor.nb;
    out += path_between(cur, tgt);
    cur = tgt;
  }
  out += path_between(cur, start_node);
  return out;
}

}  // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> N >> SI >> SJ;
  G.resize(N);
  ROADS = 0;
  for (int i = 0; i < N; ++i) {
    cin >> G[i];
    for (int j = 0; j < N; ++j)
      if (road(i, j))
        ++ROADS;
  }

  collect_nodes();
  collect_corridors();
  build_all_pairs();

  const int start_node = node_id[SI][SJ];
  mt19937 rng((uint32_t)chrono::steady_clock::now().time_since_epoch().count());

  const auto deadline =
      chrono::steady_clock::now() + chrono::milliseconds(2700);
  const auto init_deadline =
      chrono::steady_clock::now() + chrono::milliseconds(400);

  vector<TourStep> tour = greedy_tour(start_node);
  i64 best_cost = tour_cost(tour, start_node);
  while (chrono::steady_clock::now() < init_deadline) {
    vector<TourStep> cand = greedy_tour(start_node, &rng);
    const i64 cost = tour_cost(cand, start_node);
    if (cost < best_cost) {
      best_cost = cost;
      tour = std::move(cand);
    }
  }

  improve_two_opt(tour, start_node,
                  chrono::steady_clock::now() + chrono::milliseconds(540));
  sa_two_opt(tour, start_node, deadline);

  cout << build_output(tour, start_node) << '\n';
  return 0;
}
