// Midnight Code Cup 2026 — A. Beutsche Dahn (optimization / heuristic)
//
// Reads one test case from stdin (problem statement format). Writes schedule to stdout.
//
// Strategy (greedy):
// - Precompute all-pairs shortest hop counts (BFS from each vertex).
// - Each tick: appearances → drops → picks → moves.
// - Pick order per city: shorter hop distance to destination first (then lower passenger id).
// - Loaded train goal: minimize hops to some onboard dest; ties prefer more rides (higher P),
//   then earlier appearance time, then lower id.
// - Empty train: rational staging dist/(slack+1); trains at same city spread across ranked goals.
// - Edge conflict: one train per undirected edge per tick; lower train id moves first.
//
// Objective is contest-defined (partial in statement); tuned against judge_beutsche.py
// (minimize Σ P_i (T_arrival − T_appear)).

#include <bits/stdc++.h>
using namespace std;

namespace {

struct Passenger {
  int u, v, t_ap;
  int city = -1; // waiting city while not done / not on train
  int train = 0; // 0 if not onboard
  int rides = 0; // completed ride segments (increment on pick)
  bool done = false;
};

int V, E, Ttr, C, N;
vector<vector<int>> adj;
vector<int> train_pos;
vector<Passenger> pass;
vector<vector<int>> dist_mat; // dist_mat[a][b] = shortest path length a→b

void bfs_all_pairs() {
  dist_mat.assign(V + 1, vector<int>(V + 1, -1));
  for (int s = 1; s <= V; ++s) {
    queue<int> q;
    dist_mat[s][s] = 0;
    q.push(s);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (int v : adj[u]) {
        if (dist_mat[s][v] < 0) {
          dist_mat[s][v] = dist_mat[s][u] + 1;
          q.push(v);
        }
      }
    }
  }
}

// One step from `from` toward `goal` on a shortest path (tie: smallest neighbor id).
int step_toward(int from, int goal) {
  if (from == goal) {
    return from;
  }
  int best = -1;
  int best_w = INT_MAX;
  for (int w : adj[from]) {
    if (dist_mat[w][goal] == dist_mat[from][goal] - 1 && w < best_w) {
      best_w = w;
      best = w;
    }
  }
  assert(best != -1);
  return best;
}

struct RationalCandidate {
  int num{}; // distance hops
  int den{}; // urgency denominator (smaller slack → lower den relative weight)
  int city{};
};

bool rat_lt(const RationalCandidate& a, const RationalCandidate& b) {
  return (__int128)a.num * b.den < (__int128)b.num * a.den;
}

// Empty-train staging: minimize dist(from, target) / (slack+1), where slack is ticks until
// appearance for future passengers; slack=0 for cities that already have waiting passengers.
// Returns ranked list of distinct goal cities (best first) for diversification across trains.
vector<int> staging_goals_ranked(int from, long long tick, const vector<vector<int>>& waiting_at) {
  vector<RationalCandidate> cand;
  cand.reserve((size_t)V + (size_t)N);

  auto push_city = [&](int city, int num, int den) {
    if (num < 0 || den <= 0) {
      return;
    }
    cand.push_back({num, den, city});
  };

  for (int c = 1; c <= V; ++c) {
    if (waiting_at[c].empty()) {
      continue;
    }
    int d = dist_mat[from][c];
    push_city(c, d, 1);
  }
  for (int i = 1; i <= N; ++i) {
    if (pass[i].done || pass[i].train != 0) {
      continue;
    }
    if (pass[i].city >= 0) {
      continue;
    }
    int c = pass[i].u;
    int d = dist_mat[from][c];
    int slack = pass[i].t_ap - (int)tick;
    if (slack < 0) {
      slack = 0;
    }
    push_city(c, d, slack + 1);
  }

  sort(cand.begin(), cand.end(), [](const RationalCandidate& a, const RationalCandidate& b) {
    if (rat_lt(a, b)) {
      return true;
    }
    if (rat_lt(b, a)) {
      return false;
    }
    return a.city < b.city;
  });

  vector<int> out;
  out.reserve(cand.size());
  int last = -1;
  for (const RationalCandidate& r : cand) {
    if (r.city == last) {
      continue;
    }
    last = r.city;
    out.push_back(r.city);
  }
  return out;
}

int pick_staging_goal(const vector<int>& ranked, int rank) {
  if (ranked.empty()) {
    return -1;
  }
  if (rank >= (int)ranked.size()) {
    return ranked.back();
  }
  return ranked[(size_t)rank];
}

struct Tick {
  vector<string> cmds;
  vector<pair<int, int>> moves; // train, next_city
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> V >> E;
  adj.assign(V + 1, {});
  for (int i = 0; i < E; ++i) {
    int u, v;
    cin >> u >> v;
    adj[u].push_back(v);
    adj[v].push_back(u);
  }
  cin >> Ttr;
  train_pos.resize(Ttr + 1);
  for (int j = 1; j <= Ttr; ++j) {
    cin >> train_pos[j];
  }
  cin >> C;
  cin >> N;
  pass.resize(N + 1);
  for (int i = 1; i <= N; ++i) {
    cin >> pass[i].u >> pass[i].v >> pass[i].t_ap;
  }

  bfs_all_pairs();

  vector<vector<int>> waiting_at(V + 1);
  vector<vector<int>> onboard(Ttr + 1); // passenger ids per train

  vector<Tick> plan;
  long long total_moves = 0;
  const long long MAX_TICKS = 1000000LL;
  const long long MAX_MOVES = 2000000LL;

  for (long long tick = 1;; ++tick) {
    if (tick > MAX_TICKS) {
      cerr << "error: exceeded tick limit\n";
      return 1;
    }

    // --- appearances ---
    for (int i = 1; i <= N; ++i) {
      if (!pass[i].done && pass[i].city < 0 && pass[i].t_ap == (int)tick) {
        pass[i].city = pass[i].u;
        waiting_at[pass[i].u].push_back(i);
      }
    }

    // Pick order: shorter remaining trip first, then lower passenger id.
    for (int city = 1; city <= V; ++city) {
      if (waiting_at[city].empty()) {
        continue;
      }
      sort(waiting_at[city].begin(), waiting_at[city].end(), [&](int a, int b) {
        int da = dist_mat[city][pass[a].v];
        int db = dist_mat[city][pass[b].v];
        if (da != db) {
          return da < db;
        }
        return a < b;
      });
    }

    Tick cur;

    // --- drops first (anyone sitting on train at destination) ---
    for (int j = 1; j <= Ttr; ++j) {
      vector<int> stay;
      for (int pid : onboard[j]) {
        if (train_pos[j] == pass[pid].v) {
          cur.cmds.push_back("drop " + to_string(j) + " " + to_string(pid));
          pass[pid].done = true;
          pass[pid].train = 0;
        } else {
          stay.push_back(pid);
        }
      }
      onboard[j].swap(stay);
    }

    // --- picks: FIFO by appearance order already in waiting_at if we always push_back by tick
    // order --- Process cities 1..V; at each city, assign waiting passengers to trains there (by
    // train id).
    for (int city = 1; city <= V; ++city) {
      if (waiting_at[city].empty()) {
        continue;
      }
      vector<int> trains_here;
      for (int j = 1; j <= Ttr; ++j) {
        if (train_pos[j] == city) {
          trains_here.push_back(j);
        }
      }
      sort(trains_here.begin(), trains_here.end());
      size_t wi = 0;
      for (int j : trains_here) {
        while (wi < waiting_at[city].size() && (int)onboard[j].size() < C) {
          int pid = waiting_at[city][wi++];
          if (pass[pid].done) {
            continue;
          }
          cur.cmds.push_back("pick " + to_string(j) + " " + to_string(pid));
          pass[pid].rides++;
          pass[pid].city = -1;
          pass[pid].train = j;
          onboard[j].push_back(pid);
        }
      }
      vector<int> rest(waiting_at[city].begin() + (ptrdiff_t)wi, waiting_at[city].end());
      waiting_at[city].swap(rest);
    }

    vector<int> ranked_cache;
    int staged_from = -1;

    // --- build desired moves ---
    vector<pair<int, int>> want; // train -> next_city
    want.reserve(Ttr);

    for (int j = 1; j <= Ttr; ++j) {
      int goal = -1;
      if (!onboard[j].empty()) {
        // Head toward passenger minimizing (hops, then higher rides / earlier appear / id).
        int pos = train_pos[j];
        int best_pid = onboard[j][0];
        int best_d = dist_mat[pos][pass[best_pid].v];
        int best_r = pass[best_pid].rides;
        int best_t = pass[best_pid].t_ap;
        for (int pid : onboard[j]) {
          int d = dist_mat[pos][pass[pid].v];
          int r = pass[pid].rides;
          int ti = pass[pid].t_ap;
          if (d < best_d || (d == best_d && r > best_r) ||
              (d == best_d && r == best_r && ti < best_t) ||
              (d == best_d && r == best_r && ti == best_t && pid < best_pid)) {
            best_d = d;
            best_r = r;
            best_t = ti;
            best_pid = pid;
          }
        }
        goal = pass[best_pid].v;
      } else {
        int pos = train_pos[j];
        if (pos != staged_from) {
          staged_from = pos;
          ranked_cache = staging_goals_ranked(pos, tick, waiting_at);
        }
        int rank = 0;
        for (int jj = 1; jj < j; ++jj) {
          if (train_pos[jj] == pos) {
            rank++;
          }
        }
        goal = pick_staging_goal(ranked_cache, rank);
      }
      if (goal < 0 || train_pos[j] == goal) {
        continue;
      }
      int nxt = step_toward(train_pos[j], goal);
      want.push_back({j, nxt});
    }

    sort(want.begin(), want.end(),
         [](const pair<int, int>& a, const pair<int, int>& b) { return a.first < b.first; });

    vector<vector<char>> seen_edge(V + 1, vector<char>(V + 1, 0));

    for (auto& pr : want) {
      int j = pr.first;
      int nxt = pr.second;
      int u = train_pos[j];
      int a = min(u, nxt), b = max(u, nxt);
      if (seen_edge[a][b]) {
        continue;
      }
      seen_edge[a][b] = 1;
      cur.moves.push_back({j, nxt});
      train_pos[j] = nxt;
    }

    total_moves += (long long)cur.moves.size();
    if (total_moves > MAX_MOVES) {
      cerr << "error: exceeded move budget\n";
      return 1;
    }

    bool idle = cur.cmds.empty() && cur.moves.empty();

    int undelivered = 0;
    bool future_appearance = false;
    for (int i = 1; i <= N; ++i) {
      if (!pass[i].done) {
        undelivered++;
      }
      if (!pass[i].done && pass[i].t_ap > (int)tick) {
        future_appearance = true;
      }
    }

    if (undelivered == 0) {
      plan.push_back(move(cur));
      break;
    }

    // No-op ticks advance time until the next appearance time (passengers not yet on platform).
    if (idle && future_appearance) {
      plan.push_back(move(cur));
      continue;
    }

    if (idle) {
      cerr << "error: deadlock tick=" << tick << " undelivered=" << undelivered << "\n";
      return 1;
    }

    plan.push_back(move(cur));
  }

  // --- output ---
  cout << plan.size() << "\n";
  for (const Tick& tk : plan) {
    cout << tk.cmds.size() << "\n";
    for (const string& s : tk.cmds) {
      cout << s << "\n";
    }
    cout << tk.moves.size() << "\n";
    for (auto& mv : tk.moves) {
      cout << mv.first << " " << mv.second << "\n";
    }
  }

  return 0;
}
