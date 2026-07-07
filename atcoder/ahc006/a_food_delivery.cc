// AHC006 A - Food Delivery: https://atcoder.jp/contests/ahc006/tasks/ahc006_a
//
// Pick 50 of 1000 pickup-delivery pairs; route starts/ends at (400, 400).
// Greedy / SA order selection + event permutation under time budget.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
using Clock = chrono::steady_clock;
namespace {

constexpr int kN = 1000;
constexpr int kM = 50;
constexpr int kOffice = 400;
constexpr int kSeed = 42;

struct Order {
  int ax, ay, cx, cy;
};

struct Point {
  int x, y;
  int order_idx;
  bool is_pickup;
};

int manhattan(int x1, int y1, int x2, int y2) {
  return abs(x1 - x2) + abs(y1 - y2);
}

int manhattan(const Point& a, const Point& b) {
  return manhattan(a.x, a.y, b.x, b.y);
}

i64 route_time(const vector<pair<int, int>>& path) {
  i64 t = 0;
  for (size_t i = 1; i < path.size(); ++i)
    t += manhattan(path[i - 1].first, path[i - 1].second, path[i].first,
                   path[i].second);
  return t;
}

i64 standalone_cost(const Order& o) {
  return manhattan(kOffice, kOffice, o.ax, o.ay) +
         manhattan(o.ax, o.ay, o.cx, o.cy) +
         manhattan(o.cx, o.cy, kOffice, kOffice);
}

vector<pair<int, int>> build_route(const vector<int>& chosen,
                                   const vector<Order>& orders) {
  const int m = (int)chosen.size();
  vector<char> picked(m, 0), delivered(m, 0);
  vector<pair<int, int>> path;
  path.emplace_back(kOffice, kOffice);
  int x = kOffice, y = kOffice;

  for (int step = 0; step < 2 * m; ++step) {
    int best_dist = INT_MAX;
    int best_kind = -1;
    int best_i = -1;
    for (int i = 0; i < m; ++i) {
      const Order& o = orders[chosen[i]];
      if (!picked[i]) {
        const int d = manhattan(x, y, o.ax, o.ay);
        if (d < best_dist) {
          best_dist = d;
          best_kind = 0;
          best_i = i;
        }
      } else if (!delivered[i]) {
        const int d = manhattan(x, y, o.cx, o.cy);
        if (d < best_dist) {
          best_dist = d;
          best_kind = 1;
          best_i = i;
        }
      }
    }
    if (best_kind == 0) {
      picked[best_i] = 1;
      x = orders[chosen[best_i]].ax;
      y = orders[chosen[best_i]].ay;
    } else {
      delivered[best_i] = 1;
      x = orders[chosen[best_i]].cx;
      y = orders[chosen[best_i]].cy;
    }
    path.emplace_back(x, y);
  }
  path.emplace_back(kOffice, kOffice);
  return path;
}

vector<Point> build_events(const vector<int>& chosen,
                           const vector<Order>& orders) {
  vector<Point> events;
  events.reserve(2 * chosen.size());
  for (int j = 0; j < (int)chosen.size(); ++j) {
    const Order& o = orders[chosen[j]];
    events.push_back({o.ax, o.ay, j, true});
    events.push_back({o.cx, o.cy, j, false});
  }
  return events;
}

bool precedence_ok(const vector<Point>& events, const vector<int>& perm) {
  vector<int> first_pick(perm.size() / 2, -1), first_del(perm.size() / 2, -1);
  for (int i = 0; i < (int)perm.size(); ++i) {
    const Point& p = events[perm[i]];
    if (p.is_pickup)
      first_pick[p.order_idx] = i;
    else
      first_del[p.order_idx] = i;
  }
  for (int j = 0; j < (int)first_pick.size(); ++j) {
    if (first_pick[j] < 0 || first_del[j] < 0 || first_pick[j] >= first_del[j])
      return false;
  }
  return true;
}

vector<pair<int, int>> path_from_perm(const vector<Point>& events,
                                      const vector<int>& perm) {
  vector<pair<int, int>> path;
  path.reserve(perm.size() + 2);
  path.emplace_back(kOffice, kOffice);
  for (int idx : perm)
    path.emplace_back(events[idx].x, events[idx].y);
  path.emplace_back(kOffice, kOffice);
  return path;
}

void improve_permutation(const vector<Point>& events, vector<int>& perm,
                         mt19937& rng) {
  const int n = (int)perm.size();
  vector<pair<int, int>> path = path_from_perm(events, perm);
  i64 best = route_time(path);

  for (int iter = 0; iter < 8000; ++iter) {
    const int kind = uniform_int_distribution<int>(0, 2)(rng);
    vector<int> next = perm;
    if (kind == 0) {
      const int i = uniform_int_distribution<int>(0, n - 1)(rng);
      const int j = uniform_int_distribution<int>(0, n - 1)(rng);
      if (i != j)
        swap(next[i], next[j]);
    } else if (kind == 1) {
      const int i = uniform_int_distribution<int>(0, n - 2)(rng);
      const int j = uniform_int_distribution<int>(i + 1, n - 1)(rng);
      reverse(next.begin() + i, next.begin() + j + 1);
    } else {
      const int i = uniform_int_distribution<int>(0, n - 1)(rng);
      const int j = uniform_int_distribution<int>(0, n - 1)(rng);
      if (i != j) {
        const int v = next[i];
        next.erase(next.begin() + i);
        next.insert(next.begin() + j, v);
      }
    }
    if (!precedence_ok(events, next))
      continue;
    path = path_from_perm(events, next);
    const i64 t = route_time(path);
    if (t < best) {
      best = t;
      perm = std::move(next);
    }
  }
}

vector<int> initial_perm(const vector<Point>& events) {
  const int n = (int)events.size();
  vector<int> perm(n);
  iota(perm.begin(), perm.end(), 0);
  vector<char> picked(events.size() / 2, 0);
  vector<int> order;
  order.reserve(n);
  int x = kOffice, y = kOffice;
  for (int step = 0; step < n; ++step) {
    int best = -1, best_d = INT_MAX;
    for (int i = 0; i < n; ++i) {
      if (find(order.begin(), order.end(), i) != order.end())
        continue;
      const Point& p = events[i];
      if (p.is_pickup || picked[p.order_idx]) {
        const int d = manhattan(x, y, p.x, p.y);
        if (d < best_d) {
          best_d = d;
          best = i;
        }
      }
    }
    order.push_back(best);
    const Point& p = events[best];
    if (p.is_pickup)
      picked[p.order_idx] = 1;
    x = p.x;
    y = p.y;
  }
  return order;
}

vector<int> select_orders(const vector<Order>& orders, mt19937& rng) {
  vector<int> idx(kN);
  iota(idx.begin(), idx.end(), 0);
  sort(idx.begin(), idx.end(), [&](int a, int b) {
    const i64 ca = standalone_cost(orders[a]);
    const i64 cb = standalone_cost(orders[b]);
    if (ca != cb)
      return ca < cb;
    return a < b;
  });

  vector<int> chosen;
  chosen.reserve(kM);
  for (int i = 0; i < kN && (int)chosen.size() < kM; ++i)
    chosen.push_back(idx[i]);

  vector<Point> events = build_events(chosen, orders);
  vector<int> perm = initial_perm(events);
  improve_permutation(events, perm, rng);
  i64 best_t = route_time(path_from_perm(events, perm));
  vector<int> best = chosen;

  for (int trial = 0; trial < 80; ++trial) {
    vector<int> cand;
    cand.reserve(kM);
    vector<char> used(kN, 0);
    while ((int)cand.size() < kM) {
      const int id = idx[uniform_int_distribution<int>(0, kN - 1)(rng)];
      if (used[id])
        continue;
      used[id] = 1;
      cand.push_back(id);
    }
    events = build_events(cand, orders);
    perm = initial_perm(events);
    improve_permutation(events, perm, rng);
    const i64 t = route_time(path_from_perm(events, perm));
    if (t < best_t) {
      best_t = t;
      best = cand;
    }
  }
  return best;
}

void sa_refine(vector<int>& chosen, const vector<Order>& orders, mt19937& rng,
               Clock::time_point deadline) {
  vector<char> in_set(kN, 0);
  for (int id : chosen)
    in_set[id] = 1;
  vector<int> pool;
  for (int i = 0; i < kN; ++i)
    if (!in_set[i])
      pool.push_back(i);

  vector<Point> events = build_events(chosen, orders);
  vector<int> perm = initial_perm(events);
  improve_permutation(events, perm, rng);
  i64 cur = route_time(path_from_perm(events, perm));
  double temp = 5000.0;

  while (Clock::now() < deadline) {
    vector<int> next = chosen;
    const int out = uniform_int_distribution<int>(0, kM - 1)(rng);
    const int in_id = pool[uniform_int_distribution<int>(0, (int)pool.size() - 1)(rng)];
    const int old_id = next[out];
    next[out] = in_id;
    events = build_events(next, orders);
    perm = initial_perm(events);
    improve_permutation(events, perm, rng);
    const i64 nxt = route_time(path_from_perm(events, perm));
    const i64 delta = nxt - cur;
    if (delta < 0 || bernoulli_distribution(exp(-double(delta) / temp))(rng)) {
      chosen = next;
      cur = nxt;
      in_set[old_id] = 0;
      in_set[in_id] = 1;
      pool.clear();
      for (int i = 0; i < kN; ++i)
        if (!in_set[i])
          pool.push_back(i);
    }
    temp *= 0.99997;
  }
}

}  // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  vector<Order> orders(kN);
  for (int i = 0; i < kN; ++i)
    cin >> orders[i].ax >> orders[i].ay >> orders[i].cx >> orders[i].cy;

  mt19937 rng(kSeed);
  const auto deadline = Clock::now() + chrono::milliseconds(1900);
  vector<int> chosen = select_orders(orders, rng);
  sa_refine(chosen, orders, rng, deadline);

  vector<Point> events = build_events(chosen, orders);
  vector<int> perm = initial_perm(events);
  improve_permutation(events, perm, rng);
  vector<pair<int, int>> path = path_from_perm(events, perm);

  cout << kM;
  for (int id : chosen)
    cout << ' ' << (id + 1);
  cout << '\n';
  cout << path.size();
  for (const auto& [x, y] : path)
    cout << ' ' << x << ' ' << y;
  cout << '\n';
  return 0;
}
