// AHC020 A - Broadcasting: https://atcoder.jp/contests/ahc020/tasks/ahc020_a
//
// MST for cable connectivity; greedy power assignment per resident cluster;
// hill climbing on power levels with optional station pruning via MST.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

constexpr int N = 100;
constexpr int MAXM = 300;
constexpr int MAXK = 5000;
constexpr int MAX_POWER = 5000;
constexpr double TIME_LIMIT = 1.9;

struct Pt {
  int x, y;
};

int M, K;
Pt vpos[N];
vector<pair<int, int>> ve[N];
int edge_id[N][N];
int edge_cost[MAXM];
pair<int, int> edges[MAXM];
vector<Pt> kpos;
vector<int> kclosest[MAXK];
vector<int> vclosest[N];

double now() {
  using clock = chrono::steady_clock;
  static const auto t0 = clock::now();
  return chrono::duration<double>(clock::now() - t0).count();
}

mt19937 rng(712367);

int dist_sq(const Pt& a, const Pt& b) {
  const int dx = a.x - b.x;
  const int dy = a.y - b.y;
  return dx * dx + dy * dy;
}

int required_power(int d2) {
  int p = (int)sqrt((double)d2);
  if (p * p < d2)
    ++p;
  return min(p, MAX_POWER);
}

void find_kclosest(int no) {
  for (int i = 0; i < K; ++i) {
    vector<pair<int, int>> vp;
    vp.reserve(N);
    for (int j = 0; j < N; ++j)
      vp.emplace_back(dist_sq(kpos[i], vpos[j]), j);
    sort(vp.begin(), vp.end());
    kclosest[i].clear();
    for (int j = 0; j < no; ++j) {
      kclosest[i].push_back(vp[j].second);
      vclosest[vp[j].second].push_back(i);
    }
  }
}

vector<int> mst(vector<int> need) {
  using P = pair<int, int>;
  priority_queue<P, vector<P>, greater<P>> pq;
  assert(need[0] == 1);
  need[0] = 0;
  vector<int> used(M, 0);
  for (const auto& [nb, w] : ve[0])
    pq.emplace(w, edge_id[0][nb]);
  while (!pq.empty()) {
    auto [c, e] = pq.top();
    pq.pop();
    if (need[edges[e].first] == 0 && need[edges[e].second] == 0)
      continue;
    const int v = need[edges[e].first] == 0 ? edges[e].second : edges[e].first;
    used[e] = 1;
    need[v] = 0;
    for (const auto& [nb, w] : ve[v])
      if (need[nb])
        pq.emplace(w, edge_id[v][nb]);
  }
  for (int i = 0; i < N; ++i)
    if (need[i])
      return vector<int>(M, 1);
  return used;
}

vector<int> kgreedy(const vector<int>& active, vector<int> power, bool init = false) {
  using Item = tuple<int, int, int>;
  priority_queue<Item, vector<Item>, greater<Item>> pq;
  vector<char> covered(K, 0);
  vector<int> empty_penalty(N, 1 << 30);
  for (int j = 0; j < M; ++j) {
    empty_penalty[edges[j].first] =
        min(empty_penalty[edges[j].first], edge_cost[j]);
    empty_penalty[edges[j].second] =
        min(empty_penalty[edges[j].second], edge_cost[j]);
  }
  if (init) {
    for (int j = 0; j < N; ++j)
      empty_penalty[j] = (int)(empty_penalty[j] * 0.2);
  } else {
    const double mul = pow(0.5, uniform_real_distribution<>(0, 5)(rng));
    for (int j = 0; j < N; ++j)
      empty_penalty[j] = max(1, (int)(empty_penalty[j] * mul));
  }

  for (int i = 0; i < K; ++i) {
    int best = 1 << 30;
    int bv = -1;
    for (int v : kclosest[i]) {
      if (!active[v])
        continue;
      int d = dist_sq(kpos[i], vpos[v]) - power[v] * power[v];
      if (power[v] == 0)
        d += empty_penalty[v];
      if (d < best) {
        best = d;
        bv = v;
      }
    }
    if (best <= 0)
      covered[i] = 1;
    else
      pq.emplace(best, i, bv);
  }

  while (!pq.empty()) {
    auto [d, k, v] = pq.top();
    pq.pop();
    if (covered[k])
      continue;
    covered[k] = 1;
    const int d2 = dist_sq(kpos[k], vpos[v]);
    power[v] = max(power[v], required_power(d2));
    for (int i : vclosest[v]) {
      if (covered[i])
        continue;
      const int d3 = dist_sq(kpos[i], vpos[v]);
      const int extra = d3 - power[v] * power[v] + (power[v] ? 0 : empty_penalty[v]);
      pq.emplace(extra, i, v);
    }
  }

  for (int i = 0; i < K; ++i)
    if (!covered[i])
      return vector<int>(N, MAX_POWER);
  for (int i = 0; i < N; ++i)
    if (power[i] > MAX_POWER)
      return vector<int>(N, MAX_POWER);
  return power;
}

i64 edge_weight_sum(const vector<int>& used) {
  i64 s = 0;
  for (int i = 0; i < M; ++i)
    if (used[i])
      s += edge_cost[i];
  return s;
}

i64 power_cost_sum(const vector<int>& power) {
  i64 s = 0;
  for (int p : power)
    s += 1LL * p * p;
  return s;
}

vector<int> refine_mst(vector<int> need, i64& wcost, vector<int> used) {
  for (int rep = 0; rep < 5; ++rep) {
    bool update = false;
    for (int i = 1; i < N; ++i) {
      if (need[i] == 0)
        continue;
      need[i] = 0;
      const vector<int> cand = mst(need);
      const i64 cost = edge_weight_sum(cand);
      if (cost <= wcost) {
        update |= cost < wcost;
        used = cand;
        wcost = cost;
      } else {
        need[i] = 1;
      }
    }
    for (int i = 1; i < N; ++i) {
      if (need[i] == 1)
        continue;
      need[i] = 1;
      const vector<int> cand = mst(need);
      const i64 cost = edge_weight_sum(cand);
      if (cost <= wcost) {
        update |= cost < wcost;
        used = cand;
        wcost = cost;
      } else {
        need[i] = 0;
      }
    }
    if (!update)
      break;
  }
  return used;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n >> M >> K;
  assert(n == N);

  for (int i = 0; i < N; ++i)
    cin >> vpos[i].x >> vpos[i].y;

  memset(edge_id, -1, sizeof(edge_id));
  for (int i = 0; i < M; ++i) {
    int a, b, w;
    cin >> a >> b >> w;
    --a;
    --b;
    ve[a].emplace_back(b, w);
    ve[b].emplace_back(a, w);
    edge_id[a][b] = edge_id[b][a] = i;
    edges[i] = {a, b};
    edge_cost[i] = w;
  }

  kpos.resize(K);
  for (int i = 0; i < K; ++i)
    cin >> kpos[i].x >> kpos[i].y;

  find_kclosest(10);

  const vector<int> all_active(N, 1);
  vector<int> power = kgreedy(all_active, vector<int>(N, 0), true);
  i64 ncost = power_cost_sum(power);
  vector<int> need(N, 1);
  vector<int> eused = mst(need);
  i64 wcost = edge_weight_sum(eused);

  int steps = 0;
  while (now() < TIME_LIMIT) {
    ++steps;
    int a;
    do {
      a = uniform_int_distribution<>(0, N - 1)(rng);
    } while (power[a] == 0);
    const int val =
        uniform_int_distribution<>(0, 1)(rng) == 0
            ? 0
            : uniform_int_distribution<>(0, power[a])(rng);

    vector<int> npower = power;
    npower[a] = val;
    npower = kgreedy(all_active, npower);
    const i64 nncost = power_cost_sum(npower);

    need.assign(N, 1);
    vector<int> neused = mst(need);
    i64 nwcost = edge_weight_sum(neused);
    neused = refine_mst(need, nwcost, neused);

    if (nncost + nwcost <= ncost + wcost) {
      power = move(npower);
      eused = move(neused);
      ncost = nncost;
      wcost = nwcost;
    }
  }

  for (int i = 0; i < N; ++i) {
    if (i)
      cout << ' ';
    cout << power[i];
  }
  cout << '\n';
  for (int i = 0; i < M; ++i) {
    if (i)
      cout << ' ';
    cout << eused[i];
  }
  cout << '\n';
  return 0;
}
