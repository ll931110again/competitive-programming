#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

struct Castle {
  int a; // minimum to attack
  int m; // expected deaths
  int g; // must leave to defend
};

struct Task {
  i64 req;  // soldiers needed before starting this subtree
  i64 cost; // soldiers lost after conquering this subtree
};

i64 min_initial_for_tasks(vector<Task>& tasks) {
  // Given tasks where each requires current >= req and then reduces by cost,
  // the minimal starting value is minimized by sorting by (req - cost)
  // descending.
  sort(tasks.begin(), tasks.end(), [](const Task& x, const Task& y) {
    i64 dx = x.req - x.cost;
    i64 dy = y.req - y.cost;
    if (dx != dy)
      return dx > dy;
    return x.req > y.req;
  });
  i64 need = 0;
  i64 spent = 0;
  for (const auto& t : tasks) {
    need = max(need, t.req + spent);
    spent += t.cost;
  }
  return need;
}

struct DpRes {
  i64 need; // minimal soldiers needed upon arriving (before attacking
            // this castle)
  i64 loss; // total soldiers removed in this subtree (deaths+garrisons)
};

DpRes dfs_solve(int u, int parent, const vector<Castle>& castles, const vector<vector<int>>& adj) {
  vector<Task> tasks;
  i64 loss_sum = 0;

  for (int v : adj[u]) {
    if (v == parent)
      continue;
    DpRes child = dfs_solve(v, u, castles, adj);
    tasks.push_back(Task{child.need, child.loss});
    loss_sum += child.loss;
  }

  i64 self_cost = (i64)castles[u].m + (i64)castles[u].g;
  i64 need_after_capture = min_initial_for_tasks(tasks);
  i64 need_before_attack = max<i64>(castles[u].a, self_cost + need_after_capture);
  i64 total_loss = self_cost + loss_sum;
  return DpRes{need_before_attack, total_loss};
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  int case_num = 1;
  while (cin >> n) {
    if (n == 0)
      break;
    vector<Castle> castles(n);
    for (int i = 0; i < n; i++) {
      cin >> castles[i].a >> castles[i].m >> castles[i].g;
    }

    vector<vector<int>> adj(n);
    for (int i = 0; i < n - 1; i++) {
      int x, y;
      cin >> x >> y;
      --x;
      --y;
      adj[x].push_back(y);
      adj[y].push_back(x);
    }

    i64 best = (1LL << 62);
    for (int root = 0; root < n; root++) {
      DpRes res = dfs_solve(root, -1, castles, adj);
      best = min(best, res.need);
    }

    cout << "Case " << case_num++ << ": " << best << "\n";
  }
  return 0;
}
