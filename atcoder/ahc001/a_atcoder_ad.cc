// AHC001 A - AtCoder Ad: https://atcoder.jp/contests/ahc001/tasks/ahc001_a
//
// Simulated annealing on rectangle borders (kmyk): start from 1×1 cells at each
// company point, then expand/shrink edges while resolving overlaps. Time-budgeted
// SA beats area-balanced BSP on typical inputs.

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;

namespace {

constexpr int W = 10000;
constexpr int LEFT = 0;
constexpr int UP = 1;
constexpr int RIGHT = 2;

struct Company {
  int x, y, r;
};

struct Rect {
  int a, b, c, d;

  i64 area() const { return i64(c - a) * (d - b); }
};

double per_company_score(i64 area, i64 target) {
  if (area <= 0)
    return 0.0;
  const double ratio = min<double>(area, target) / max<double>(area, target);
  return 2.0 * ratio - ratio * ratio;
}

struct BorderAnnealer {
  const vector<Company>& co;
  const int n;
  vector<Rect> rect;
  vector<vector<int>> owner;
  double pre_score = 0.0;
  mt19937 rng;

  explicit BorderAnnealer(const vector<Company>& companies, uint32_t seed)
      : co(companies), n((int)companies.size()), rect(n), owner(W, vector<int>(W, -1)),
        rng(seed) {
    for (int i = 0; i < n; ++i) {
      rect[i] = {co[i].x, co[i].y, co[i].x + 1, co[i].y + 1};
      owner[co[i].y][co[i].x] = i;
      pre_score += per_company_score(1, co[i].r);
    }
  }

  double company_score(int i) const {
    const i64 area = rect[i].area();
    return per_company_score(area, co[i].r);
  }

  void clear_from_grid(int id, const Rect& r) {
    for (int y = r.b; y < r.d; ++y)
      for (int x = r.a; x < r.c; ++x)
        if (owner[y][x] == id)
          owner[y][x] = -1;
  }

  void apply_to_grid(int id) {
    for (int y = rect[id].b; y < rect[id].d; ++y)
      for (int x = rect[id].a; x < rect[id].c; ++x)
        owner[y][x] = id;
  }

  set<int> overlapping_companies(int i, int dir, int amount) const {
    set<int> overlap;
    int ly = rect[i].b, ry = rect[i].d;
    int lx = rect[i].a, rx = rect[i].c;
    if (dir == LEFT) {
      lx = rect[i].a - amount;
      rx = rect[i].a;
    } else if (dir == UP) {
      ly = rect[i].b - amount;
      ry = rect[i].b;
    } else if (dir == RIGHT) {
      lx = rect[i].c;
      rx = rect[i].c + amount;
    } else {
      ly = rect[i].d;
      ry = rect[i].d + amount;
    }

    for (int y = ly; y < ry; ++y) {
      for (int x = lx; x < rx; ++x) {
        const int j = owner[y][x];
        if (j >= 0 && j != i)
          overlap.insert(j);
      }
    }
    return overlap;
  }

  bool expansion_blocked(int i, int dir, int amount,
                         const set<int>& overlap) const {
    if (amount <= 0 || overlap.empty())
      return false;

    int ly = rect[i].b, ry = rect[i].d;
    int lx = rect[i].a, rx = rect[i].c;
    if (dir == LEFT)
      lx -= amount;
    else if (dir == UP)
      ly -= amount;
    else if (dir == RIGHT)
      rx += amount;
    else
      ry += amount;

    for (int j : overlap) {
      if (ly <= co[j].y && co[j].y < ry && lx <= co[j].x && co[j].x < rx)
        return true;
    }
    return false;
  }

  vector<Rect> run(chrono::steady_clock::time_point deadline) {
    const auto start = chrono::steady_clock::now();
    vector<Rect> best = rect;
    double best_score = pre_score;

    uniform_int_distribution<int> pick_company(0, n - 1);
    uniform_int_distribution<int> pick_dir(0, 3);

    for (int iter = 0;; ++iter) {
      if (iter % 32 == 0) {
        const auto now = chrono::steady_clock::now();
        if (now >= deadline)
          break;
      }

      const int i = pick_company(rng);
      const int dir = pick_dir(rng);

      int amount_min = -100;
      int amount_max = 300;
      if (dir == LEFT) {
        amount_max = min(amount_max, rect[i].a);
        amount_min = max(amount_min, -(co[i].x - rect[i].a));
      } else if (dir == UP) {
        amount_max = min(amount_max, rect[i].b);
        amount_min = max(amount_min, -(co[i].y - rect[i].b));
      } else if (dir == RIGHT) {
        amount_max = min(amount_max, W - rect[i].c);
        amount_min = max(amount_min, -(rect[i].c - co[i].x) + 1);
      } else {
        amount_max = min(amount_max, W - rect[i].d);
        amount_min = max(amount_min, -(rect[i].d - co[i].y) + 1);
      }
      if (amount_min > amount_max)
        continue;

      int amount = 0;
      for (int tries = 0; tries < 8 && amount == 0; ++tries)
        amount = uniform_int_distribution<int>(amount_min, amount_max)(rng);
      if (amount == 0)
        continue;

      const set<int> overlap = overlapping_companies(i, dir, amount);
      if (expansion_blocked(i, dir, amount, overlap))
        continue;

      const Rect old_i = rect[i];
      vector<Rect> trial = rect;
      double delta = -per_company_score(trial[i].area(), co[i].r);
      map<int, Rect> saved;
      for (int j : overlap) {
        delta -= per_company_score(trial[j].area(), co[j].r);
        saved[j] = trial[j];
        Rect& moved = trial[j];
        const Rect& anchor = trial[i];
        if (dir == LEFT) {
          if (co[j].x < anchor.a - amount)
            moved.c = anchor.a - amount;
          else if (co[j].y < anchor.b)
            moved.d = anchor.b;
          else if (anchor.c <= co[j].x)
            moved.a = anchor.c;
          else if (anchor.d <= co[j].y)
            moved.b = anchor.d;
        } else if (dir == UP) {
          if (co[j].x < anchor.a)
            moved.c = anchor.a;
          else if (co[j].y < anchor.b - amount)
            moved.d = anchor.b - amount;
          else if (anchor.c <= co[j].x)
            moved.a = anchor.c;
          else if (anchor.d <= co[j].y)
            moved.b = anchor.d;
        } else if (dir == RIGHT) {
          if (co[j].x < anchor.a)
            moved.c = anchor.a;
          else if (co[j].y < anchor.b)
            moved.d = anchor.b;
          else if (anchor.c + amount <= co[j].x)
            moved.a = anchor.c + amount;
          else if (anchor.d <= co[j].y)
            moved.b = anchor.d;
        } else {
          if (co[j].x < anchor.a)
            moved.c = anchor.a;
          else if (co[j].y < anchor.b)
            moved.d = anchor.b;
          else if (anchor.c <= co[j].x)
            moved.a = anchor.c;
          else if (anchor.d + amount <= co[j].y)
            moved.b = anchor.d + amount;
        }
        delta += per_company_score(moved.area(), co[j].r);
      }

      Rect next_i = trial[i];
      if (dir == LEFT)
        next_i.a -= amount;
      else if (dir == UP)
        next_i.b -= amount;
      else if (dir == RIGHT)
        next_i.c += amount;
      else
        next_i.d += amount;
      delta += per_company_score(next_i.area(), co[i].r);

      const auto now = chrono::steady_clock::now();
      const double temperature =
          max(0.0, chrono::duration<double>(deadline - now).count() /
                       chrono::duration<double>(deadline - start).count());

      const bool accept =
          delta >= 0.0 ||
          bernoulli_distribution(min(1.0, exp(100.0 * delta) * temperature))(rng);

      if (accept) {
        rect[i] = next_i;
        for (int j : overlap)
          rect[j] = trial[j];
        pre_score += delta;
        if (pre_score > best_score) {
          best_score = pre_score;
          best = rect;
        }
        clear_from_grid(i, old_i);
        for (const auto& [j, old_j] : saved)
          clear_from_grid(j, old_j);
        apply_to_grid(i);
        for (int j : overlap)
          apply_to_grid(j);
      }
    }
    return best;
  }
};

vector<Rect> solve(const vector<Company>& co) {
  const auto deadline =
      chrono::steady_clock::now() + chrono::milliseconds(4750);
  BorderAnnealer annealer(co, 20210306);
  return annealer.run(deadline);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  cin >> n;
  vector<Company> co(n);
  for (int i = 0; i < n; ++i)
    cin >> co[i].x >> co[i].y >> co[i].r;

  const vector<Rect> out = solve(co);
  for (const auto& r : out)
    cout << r.a << ' ' << r.b << ' ' << r.c << ' ' << r.d << '\n';
  return 0;
}
