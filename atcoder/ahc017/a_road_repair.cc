// AHC017 A - Road Repair: https://atcoder.jp/contests/ahc017/tasks/ahc017_a
//
// Greedy edge scheduling with beam search on sampled sources, using dynamic
// bucket-Dijkstra for incremental frustration updates, then hill climbing.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
using i16 = int16_t;

namespace {

constexpr int MAX_N = 1000;
constexpr int MAX_M = 3000;
constexpr int MAX_D = 30;
constexpr int INF_DIST = 1'000'000;
constexpr int DIST_DIV = 1000;
constexpr double TIME_LIMIT = 5.85;

struct Edge {
  int u, v, w;
};

struct XorShift {
  uint32_t x = 123456789;
  uint32_t next() {
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
  }
  int next_int(int n) { return (int)(next() % (uint32_t)n); }
  int next_int(int lo, int hi) { return lo + next_int(hi - lo); }
  double next_double() { return next() * (1.0 / 4294967296.0); }
};

using Graph = vector<vector<pair<int, int>>>;

int N, M, D, K;
Graph g;
vector<pair<int, int>> coord;
vector<Edge> edges;
int fd[MAX_N][MAX_N];

int subset[MAX_N];
int subset_sz = 0;

i16 ddist[MAX_D][MAX_N][MAX_N];
i64 ddist_sum[MAX_D][MAX_N];

int vs[MAX_N];
i16 bucket[MAX_N * 4][16];
int bucket_cnt[MAX_N * 4];

unsigned short bfs_mark[MAX_N];
unsigned short bfs_id = 0;
unsigned short bfs_q[MAX_N];

XorShift rng;
chrono::steady_clock::time_point prog_start;

double elapsed() {
  return chrono::duration<double>(chrono::steady_clock::now() - prog_start)
      .count();
}

int dist2(int a, int b) {
  const int dx = coord[a].first - coord[b].first;
  const int dy = coord[a].second - coord[b].second;
  return dx * dx + dy * dy;
}

void add_edges(Graph &adj, const vector<int> &ids) {
  for (int eid : ids) {
    const auto &e = edges[eid];
    adj[e.u].emplace_back(e.v, fd[e.u][e.v]);
    adj[e.v].emplace_back(e.u, fd[e.u][e.v]);
  }
}

void remove_edges(Graph &adj, const vector<int> &ids) {
  for (int eid : ids) {
    const auto &e = edges[eid];
    auto erase_one = [&](int a, int b) {
      auto &vec = adj[a];
      for (int i = 0; i < (int)vec.size(); ++i) {
        if (vec[i].first == b) {
          vec[i] = vec.back();
          vec.pop_back();
          return;
        }
      }
    };
    erase_one(e.u, e.v);
    erase_one(e.v, e.u);
  }
}

i64 calc_all_distances(Graph &adj, const vector<int> &subset_ids, int day) {
  i64 total = 0;
  for (int sid : subset_ids) {
    const int s = subset[sid];
    fill(vs, vs + N, INF_DIST);
    fill(bucket_cnt, bucket_cnt + N * 4, 0);
    bucket[0][bucket_cnt[0]++] = s;
    vs[s] = 0;
    i64 cur_sum = 0;
    int last = 0;
    int connected = 0;
    for (int d = 0; d <= last; ++d) {
      for (int i = 0; i < bucket_cnt[d]; ++i) {
        const int v = bucket[d][i];
        if (d > vs[v])
          continue;
        if (day >= 0)
          ddist[day][sid][v] = (i16)d;
        cur_sum += d;
        ++connected;
        for (const auto &[to, w] : adj[v]) {
          const int nd = d + w;
          if (nd < vs[to]) {
            vs[to] = nd;
            bucket[nd][bucket_cnt[nd]++] = to;
            last = max(last, nd);
          }
        }
      }
    }
    cur_sum += i64(N - connected) * INF_DIST;
    if (day >= 0)
      ddist_sum[day][sid] = cur_sum;
    total += cur_sum;
    if (connected < N)
      return (i64)1e14;
  }
  return total;
}

template <bool kUpdate>
i64 calc_rem_edge(Graph &adj, const vector<int> &subset_ids, int day, int rem) {
  const int a = edges[rem].u;
  const int b = edges[rem].v;
  const int w = fd[a][b];
  remove_edges(adj, {rem});
  i64 total = 0;
  for (int sid : subset_ids) {
    if (abs((int)ddist[day][sid][a] - (int)ddist[day][sid][b]) < w) {
      total += ddist_sum[day][sid];
      continue;
    }
    const int start =
        ddist[day][sid][a] > ddist[day][sid][b] ? a : b;
    i64 cur_sum = ddist_sum[day][sid];

    if (bfs_id == 0)
      fill(bfs_mark, bfs_mark + N, 0);
    ++bfs_id;
    int qn = 1;
    bfs_q[0] = start;
    bfs_mark[start] = bfs_id;
    for (int qi = 0; qi < qn; ++qi) {
      const int v = bfs_q[qi];
      cur_sum -= ddist[day][sid][v];
      for (const auto &[to, ew] : adj[v]) {
        if (bfs_mark[to] != bfs_id &&
            ddist[day][sid][v] + ew == ddist[day][sid][to]) {
          bfs_mark[to] = bfs_id;
          bfs_q[qn++] = to;
        }
      }
    }

    fill(vs, vs + N, INF_DIST);
    int last = 0;
    int start_d = INF_DIST;
    for (int qi = 0; qi < qn; ++qi) {
      const int v = bfs_q[qi];
      int best = INF_DIST;
      for (const auto &[to, ew] : adj[v]) {
        if (bfs_mark[to] != bfs_id)
          best = min(best, (int)ddist[day][sid][to] + ew);
      }
      vs[v] = best;
      if (best < INF_DIST) {
        bucket[best][bucket_cnt[best]++] = v;
        start_d = min(start_d, best);
        last = max(last, best);
      }
    }
    if (start_d > last) {
      cur_sum += i64(qn) * INF_DIST;
      total += cur_sum;
      continue;
    }
    for (int d = start_d; d <= last; ++d) {
      for (int i = 0; i < bucket_cnt[d]; ++i) {
        const int v = bucket[d][i];
        if (d > vs[v])
          continue;
        if constexpr (kUpdate) {
          ddist_sum[day][sid] += d - ddist[day][sid][v];
          ddist[day][sid][v] = (i16)d;
        }
        cur_sum += d;
        for (const auto &[to, ew] : adj[v]) {
          if (bfs_mark[to] == bfs_id && d + ew < vs[to]) {
            vs[to] = d + ew;
            bucket[d + ew][bucket_cnt[d + ew]++] = to;
            last = max(last, d + ew);
          }
        }
      }
    }
    fill(bucket_cnt + start_d, bucket_cnt + last + 1, 0);
    total += cur_sum;
  }
  add_edges(adj, {rem});
  return total;
}

template <bool kUpdate>
i64 calc_add_edge(Graph &adj, const vector<int> &subset_ids, int day, int add) {
  const int a = edges[add].u;
  const int b = edges[add].v;
  const int w = fd[a][b];
  i64 total = 0;
  for (int sid : subset_ids) {
    if (abs((int)ddist[day][sid][a] - (int)ddist[day][sid][b]) <= w) {
      total += ddist_sum[day][sid];
      continue;
    }
    const int start =
        ddist[day][sid][a] > ddist[day][sid][b] ? a : b;
    const int start_d =
        min((int)ddist[day][sid][a], (int)ddist[day][sid][b]) + w;
    i64 cur_sum = ddist_sum[day][sid];
    fill(vs, vs + N, INF_DIST);
    fill(bucket_cnt, bucket_cnt + N * 4, 0);
    vs[start] = start_d;
    bucket[start_d][bucket_cnt[start_d]++] = start;
    int last = start_d;
    for (int d = start_d; d <= last; ++d) {
      for (int i = 0; i < bucket_cnt[d]; ++i) {
        const int v = bucket[d][i];
        if (d > vs[v])
          continue;
        cur_sum += d - ddist[day][sid][v];
        if constexpr (kUpdate) {
          ddist_sum[day][sid] += d - ddist[day][sid][v];
          ddist[day][sid][v] = (i16)d;
        }
        for (const auto &[to, ew] : adj[v]) {
          const int nd = d + ew;
          if (nd < vs[to] && nd < (int)ddist[day][sid][to]) {
            vs[to] = nd;
            bucket[nd][bucket_cnt[nd]++] = to;
            last = max(last, nd);
          }
        }
      }
    }
    total += cur_sum;
  }
  return total;
}

vector<int> pick_subset(int count, int steps) {
  vector<int> picked(count);
  vector<char> used(N, 0);
  int p = 0;
  for (int i = 0; i < N && p < count; ++i) {
    if (used[i])
      continue;
    picked[p++] = i;
    used[i] = 1;
  }
  vector<int> closest(count);
  for (int step = 0; step < steps; ++step) {
    const int a = rng.next_int(count);
    const int old = picked[a];
    const int x = rng.next_int(N);
    if (used[x])
      continue;
    double diff = 0;
    int dmin = INT_MAX;
    for (int j = 0; j < count; ++j) {
      if (j == a)
        continue;
      dmin = min(dmin, dist2(x, picked[j]));
    }
    diff += 1.0 / dmin - 1.0 / closest[a];
    for (int j = 0; j < count; ++j) {
      if (j == a)
        continue;
      if (closest[j] == dist2(old, picked[j]) || closest[j] > dist2(x, picked[j])) {
        diff -= 1.0 / closest[j];
        const int nd = dist2(x, picked[j]);
        if (nd < closest[j])
          diff += 1.0 / nd;
        else {
          int d2 = INT_MAX;
          for (int k = 0; k < count; ++k) {
            if (j == k)
              continue;
            d2 = min(d2, dist2(picked[j], picked[k]));
          }
          diff += 1.0 / d2;
        }
      }
    }
    if (diff <= 0) {
      used[old] = 0;
      used[x] = 1;
      picked[a] = x;
      for (int j = 0; j < count; ++j) {
        int d2 = INT_MAX;
        for (int k = 0; k < count; ++k) {
          if (j == k)
            continue;
          d2 = min(d2, dist2(picked[j], picked[k]));
        }
        closest[j] = d2;
      }
    }
  }
  return picked;
}

int beam_calls(const vector<pair<int, int>> &beam) {
  int per_edge = 0;
  for (int i = 0; i < (int)beam.size(); ++i) {
    per_edge += (i ? beam[i].first - beam[i - 1].first : beam[i].first) *
                (i ? beam[i - 1].second : D);
  }
  return beam.back().first + M * beam.back().first + M * per_edge;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  prog_start = chrono::steady_clock::now();

  cin >> N >> M >> D >> K;
  g.assign(N, {});
  coord.resize(N);
  edges.resize(M);
  for (int i = 0; i < M; ++i) {
    int u, v, w;
    cin >> u >> v >> w;
    --u;
    --v;
    w = (w + DIST_DIV / 2) / DIST_DIV;
    edges[i] = {u, v, w};
    g[u].emplace_back(v, w);
    g[v].emplace_back(u, w);
    fd[u][v] = fd[v][u] = w;
  }
  for (int i = 0; i < N; ++i)
    cin >> coord[i].first >> coord[i].second;

  const i64 call_budget =
      (i64)(4e9 * sqrt(1.0 * M / N) / N);
  vector<pair<int, int>> beam = {{1, 4}, {4, 4}, {7, 3}, {10, 2}, {16, 1}};
  vector<vector<pair<int, int>>> all_beams = {beam};
  for (int sz = 2; sz <= min(N, 24); ++sz) {
    for (int lv = 0; lv < (int)beam.size(); ++lv) {
      while (beam[lv].first < (lv == 0 ? 1 : beam[lv - 1].first + 1) * sz / 4 &&
             (lv + 1 == (int)beam.size() || beam[lv].first < beam[lv + 1].first) &&
             beam[lv].first < min(N, sz)) {
        ++beam[lv].first;
        all_beams.push_back(beam);
      }
    }
  }
  int beam_pos = 0;
  for (int i = 0; i < (int)all_beams.size(); ++i) {
    if (beam_calls(all_beams[i]) <= call_budget)
      beam = all_beams[i], beam_pos = i;
    else
      break;
  }

  vector<int> picked = pick_subset(beam[0].first, 5000);
  vector<char> used(N, 0);
  for (int v : picked)
    used[v] = 1;
  vector<int> rest;
  for (int i = 0; i < N; ++i)
    if (!used[i])
      rest.push_back(i);
  for (int i = 0; i < beam.back().first - beam[0].first; ++i)
    picked.push_back(rest[i]);
  subset_sz = (int)picked.size();
  copy(picked.begin(), picked.end(), subset);
  vector<int> ids(subset_sz);
  iota(ids.begin(), ids.end(), 0);

  vector<pair<i64, int>> order(M);
  auto adj = g;
  const int sort_subset = min(subset_sz, 12);
  vector<int> sort_ids(ids.begin(), ids.begin() + sort_subset);
  calc_all_distances(adj, sort_ids, 0);
  for (int i = 0; i < M; ++i)
    order[i] = {calc_rem_edge<false>(adj, sort_ids, 0, i), i};
  sort(order.rbegin(), order.rend());

  vector<int> assign(M, -1);
  vector<vector<int>> day_edges(D);
  vector<vector<i64>> day_dist(D, vector<i64>(subset_sz, 0));
  for (int sid = 0; sid < subset_sz; ++sid) {
    calc_all_distances(adj, {sid}, 0);
    day_dist[0][sid] = ddist_sum[0][sid];
    for (int d = 1; d < D; ++d) {
      day_dist[d][sid] = ddist_sum[0][sid];
      ddist_sum[d][sid] = ddist_sum[0][sid];
      memcpy(ddist[d][sid], ddist[0][sid], sizeof(ddist[0][sid]));
    }
  }

  vector<double> greedy_time(M + 1);
  vector<int> greedy_calls(M + 1);
  greedy_time[0] = elapsed();

  for (int step = 0; step < M; ++step) {
    const int e = order[step].second;
    if (step >= 25) {
      const double dt = greedy_time[step] - greedy_time[step - 25];
      const int dc = greedy_calls[step] - greedy_calls[step - 25];
      const double left = TIME_LIMIT - greedy_time[step];
      const i64 min_left = (i64)(left * dc / max(1e-9, dt) / (M - step));
      const i64 max_left =
          (i64)(left * max(1.0, left * 16 / TIME_LIMIT) * dc /
                max(1e-9, dt) / (M - step));
      while (beam_calls(beam) + (M - step) * subset_sz < min_left &&
             beam_pos + 1 < (int)all_beams.size()) {
        beam = all_beams[++beam_pos];
        for (int j = (int)beam.size() - 1; j >= 0; --j)
          beam[j].first = min(beam[j].first, subset_sz);
      }
      while (beam_calls(beam) + (M - step) * subset_sz > max_left && beam_pos > 0) {
        beam = all_beams[--beam_pos];
        for (int j = (int)beam.size() - 1; j >= 0; --j)
          beam[j].first = min(beam[j].first, subset_sz);
      }
    }

    vector<int> candidates;
    bool seen_empty = false;
    for (int d = 0; d < D; ++d) {
      if ((int)day_edges[d].size() >= K)
        continue;
      if (day_edges[d].empty()) {
        if (seen_empty)
          continue;
        seen_empty = true;
      }
      candidates.push_back(d);
    }

    const double mw = pow(1.0 * (M - 1 - step) / max(1, M - 1), 3.5);
    vector<vector<i64>> values(D, vector<i64>(beam.size(), 0));
    for (int lv = 0; lv < (int)beam.size(); ++lv) {
      const int lo = lv ? beam[lv - 1].first : 0;
      const int hi = min(beam[lv].first, subset_sz);
      vector<int> sub_ids(ids.begin() + lo, ids.begin() + hi);
      vector<pair<double, int>> scored;
      for (int d : candidates) {
        remove_edges(adj, day_edges[d]);
        values[d][lv] = calc_rem_edge<false>(adj, sub_ids, d, e) +
                        (lv ? values[d][lv - 1] : 0);
        const double bonus =
            1.0 - 0.30 * mw *
                      pow(1.0 * (K - (int)day_edges[d].size()) / K, 12.0) *
                      (day_edges[d].empty() ? 1.10 : 1.0);
        scored.emplace_back((values[d][lv] - day_dist[d][hi - 1]) * bonus, d);
        add_edges(adj, day_edges[d]);
      }
      sort(scored.begin(), scored.end());
      candidates.clear();
      for (int j = 0; j < min(beam[lv].second, (int)scored.size()); ++j)
        candidates.push_back(scored[j].second);
      if (beam[lv].first >= beam.back().first)
        break;
    }

    const int best_day = candidates[0];
    remove_edges(adj, day_edges[best_day]);
    calc_rem_edge<true>(adj, ids, best_day, e);
    add_edges(adj, day_edges[best_day]);
    day_edges[best_day].push_back(e);
    for (int sid = 0; sid < subset_sz; ++sid)
      day_dist[best_day][sid] = ddist_sum[best_day][sid];
    assign[e] = best_day;

    greedy_calls[step + 1] = beam_calls(beam) + greedy_calls[step];
    greedy_time[step + 1] = elapsed();
  }

  vector<int> hc_ids(ids.begin(), ids.begin() + min(subset_sz, 16));
  vector<i64> hc_dist(D);
  for (int d = 0; d < D; ++d)
    hc_dist[d] = day_dist[d][(int)hc_ids.size() - 1];

  while (elapsed() < TIME_LIMIT) {
    const int e = rng.next_int(M);
    const int d1 = assign[e];
    const int d2 = rng.next_int(D);
    if (d1 == d2 || (int)day_edges[d2].size() >= K)
      continue;

    remove_edges(adj, day_edges[d1]);
    const i64 v1 = calc_add_edge<false>(adj, hc_ids, d1, e);
    add_edges(adj, day_edges[d1]);

    remove_edges(adj, day_edges[d2]);
    const i64 v2 = calc_rem_edge<false>(adj, hc_ids, d2, e);
    add_edges(adj, day_edges[d2]);

    if (v1 + v2 <= hc_dist[d1] + hc_dist[d2]) {
      hc_dist[d1] = v1;
      hc_dist[d2] = v2;
      remove_edges(adj, day_edges[d1]);
      calc_add_edge<true>(adj, hc_ids, d1, e);
      add_edges(adj, day_edges[d1]);
      remove_edges(adj, day_edges[d2]);
      calc_rem_edge<true>(adj, hc_ids, d2, e);
      add_edges(adj, day_edges[d2]);
      auto &vec = day_edges[d1];
      vec.erase(find(vec.begin(), vec.end(), e));
      day_edges[d2].push_back(e);
      assign[e] = d2;
    }
  }

  for (int i = 0; i < M; ++i) {
    if (i)
      cout << ' ';
    cout << assign[i] + 1;
  }
  cout << '\n';
  return 0;
}
