#include <algorithm>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

struct Castle {
  int a; // minimum to attack
  int m; // expected deaths
  int g; // must leave to defend
};

struct Task {
  long long req;  // soldiers needed before starting this subtree
  long long cost; // soldiers lost after conquering this subtree
};

static long long minInitialForTasks(vector<Task> &tasks) {
  // Given tasks where each requires current >= req and then reduces by cost,
  // the minimal starting value is minimized by sorting by (req - cost)
  // descending.
  sort(tasks.begin(), tasks.end(), [](const Task &x, const Task &y) {
    long long dx = x.req - x.cost;
    long long dy = y.req - y.cost;
    if (dx != dy)
      return dx > dy;
    return x.req > y.req;
  });
  long long need = 0;
  long long spent = 0;
  for (const auto &t : tasks) {
    need = max(need, t.req + spent);
    spent += t.cost;
  }
  return need;
}

struct DpRes {
  long long need; // minimal soldiers needed upon arriving (before attacking
                  // this castle)
  long long loss; // total soldiers removed in this subtree (deaths+garrisons)
};

static DpRes dfsSolve(int u, int parent, const vector<Castle> &castles,
                      const vector<vector<int>> &adj) {
  vector<Task> tasks;
  long long lossSum = 0;

  for (int v : adj[u]) {
    if (v == parent)
      continue;
    DpRes child = dfsSolve(v, u, castles, adj);
    tasks.push_back(Task{child.need, child.loss});
    lossSum += child.loss;
  }

  long long selfCost = (long long)castles[u].m + (long long)castles[u].g;
  long long needAfterCapture = minInitialForTasks(tasks);
  long long needBeforeAttack =
      max<long long>(castles[u].a, selfCost + needAfterCapture);
  long long totalLoss = selfCost + lossSum;
  return DpRes{needBeforeAttack, totalLoss};
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n;
  int caseNum = 1;
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

    long long best = (1LL << 62);
    for (int root = 0; root < n; root++) {
      DpRes res = dfsSolve(root, -1, castles, adj);
      best = min(best, res.need);
    }

    cout << "Case " << caseNum++ << ": " << best << "\n";
  }
  return 0;
}
