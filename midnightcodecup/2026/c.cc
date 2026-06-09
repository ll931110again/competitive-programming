// Midnight Code Cup 2026 — C. Run, Fix, Repeat (interactive)
//
// Generator structure: border = wall; interior even×even = room (always empty); other interior =
// corridor (open or wall).
//
// Exploration vs exploit split (contest T = 10000): use at most 5000 queries to map corridors
// (row-major unknown interiors, then pad with room probes inside the same cap). Remaining
// queries (~5000) go to localization. For smaller T, cap is min(5000, T).
//
// Localization: intersect candidate passable cells with distance shells (BFS on known map);
// landmarks on rooms, then randomized splitting queries.

#include <bits/stdc++.h>
using namespace std;

namespace {

int N;
int queries_left;
int T_total;

// -1 unknown corridor, 0 free, 1 blocked
int8_t wall[130][130];

inline bool is_border(int r, int c) {
  return r == 1 || r == N || c == 1 || c == N;
}

inline bool is_room(int r, int c) {
  return r % 2 == 0 && c % 2 == 0 && r >= 2 && r <= N - 1 && c >= 2 && c <= N - 1;
}

int ask(int r, int c) {
  cout << r << ' ' << c << endl;
  --queries_left;
  int resp;
  if (!(cin >> resp)) {
    exit(0);
  }
  if (resp == -1) {
    wall[r][c] = 1;
  } else {
    wall[r][c] = 0;
  }
  return resp;
}

vector<vector<int>> bfs_dist_map(int sr, int sc) {
  vector<vector<int>> d(N + 1, vector<int>(N + 1, -1));
  if (sr < 1 || sr > N || sc < 1 || sc > N || wall[sr][sc] != 0) {
    return d;
  }
  const int dr[4] = {-1, 1, 0, 0};
  const int dc[4] = {0, 0, -1, 1};
  queue<pair<int, int>> q;
  d[sr][sc] = 0;
  q.push({sr, sc});
  while (!q.empty()) {
    auto [r, c] = q.front();
    q.pop();
    int nd = d[r][c] + 1;
    for (int k = 0; k < 4; ++k) {
      int nr = r + dr[k];
      int nc = c + dc[k];
      if (nr < 1 || nr > N || nc < 1 || nc > N) {
        continue;
      }
      if (wall[nr][nc] != 0) {
        continue;
      }
      if (d[nr][nc] >= 0) {
        continue;
      }
      d[nr][nc] = nd;
      q.push({nr, nc});
    }
  }
  return d;
}

vector<pair<int, int>> all_passable() {
  vector<pair<int, int>> out;
  out.reserve(8000);
  for (int r = 1; r <= N; ++r) {
    for (int c = 1; c <= N; ++c) {
      if (wall[r][c] == 0) {
        out.push_back({r, c});
      }
    }
  }
  return out;
}

vector<pair<int, int>> filter_by_distance(const vector<pair<int, int>>& cand, int qr, int qc,
                                          int dist_need) {
  auto dm = bfs_dist_map(qr, qc);
  vector<pair<int, int>> out;
  out.reserve(cand.size());
  for (auto [r, c] : cand) {
    if (dm[r][c] == dist_need) {
      out.push_back({r, c});
    }
  }
  return out;
}

pair<int, int> pick_splitting_query(const vector<pair<int, int>>& cand, mt19937& rng) {
  const int n = (int)cand.size();
  if (n == 1) {
    return cand[0];
  }
  const int trials = min(96, max(16, n));
  int best_mx = INT_MAX;
  pair<int, int> best_q = cand[(unsigned)(rng() % (unsigned)n)];

  for (int t = 0; t < trials; ++t) {
    auto qv = cand[(unsigned)(rng() % (unsigned)n)];
    int qr = qv.first;
    int qc = qv.second;
    auto dm = bfs_dist_map(qr, qc);
    unordered_map<int, int> cnt;
    int mx = 0;
    for (auto [r, c] : cand) {
      int dd = dm[r][c];
      if (dd < 0) {
        continue;
      }
      mx = max(mx, ++cnt[dd]);
    }
    if (mx < best_mx) {
      best_mx = mx;
      best_q = {qr, qc};
    }
  }
  return best_q;
}

vector<pair<int, int>> build_landmarks() {
  vector<pair<int, int>> lm;
  lm.reserve(13);
  lm.push_back({2, 2});
  lm.push_back({2, N - 1});
  lm.push_back({N - 1, 2});
  lm.push_back({N - 1, N - 1});
  int m = N / 2;
  if (m % 2 != 0) {
    --m;
  }
  lm.push_back({m, m});
  lm.push_back({2, m});
  lm.push_back({N - 1, m});
  lm.push_back({m, 2});
  lm.push_back({m, N - 1});
  for (int k = 12; k <= N - 1; k += 12) {
    int e = k;
    if (e % 2 != 0) {
      ++e;
    }
    if (e > N - 1) {
      break;
    }
    lm.push_back({e, e});
  }
  return lm;
}

// At most min(5000, T_total) exploration asks: corridors first, then pad with room probes.
void exploration_phase() {
  const long long explore_cap = min(5000, max(0, T_total));
  long long done = 0;

  for (int r = 2; r <= N - 1 && done < explore_cap && queries_left > 0; ++r) {
    for (int c = 2; c <= N - 1 && done < explore_cap && queries_left > 0; ++c) {
      if (is_room(r, c)) {
        continue;
      }
      if (wall[r][c] != -1) {
        continue;
      }
      ask(r, c);
      ++done;
    }
  }

  for (int r = 2; r <= N - 1 && done < explore_cap && queries_left > 0; ++r) {
    for (int c = 2; c <= N - 1 && done < explore_cap && queries_left > 0; ++c) {
      if (!is_room(r, c)) {
        continue;
      }
      ask(r, c);
      ++done;
    }
  }
}

void localize_one(mt19937& rng) {
  const vector<pair<int, int>> landmarks = build_landmarks();
  vector<pair<int, int>> cand = all_passable();
  size_t li = 0;

  while (queries_left > 0) {
    if (cand.empty()) {
      cand = all_passable();
      li = 0;
    }
    if (cand.empty()) {
      return;
    }

    if (cand.size() == 1) {
      int resp = ask(cand[0].first, cand[0].second);
      if (resp == 0) {
        return;
      }
      cand = all_passable();
      li = 0;
      continue;
    }

    int qr = 0;
    int qc = 0;
    if (li < landmarks.size()) {
      qr = landmarks[li].first;
      qc = landmarks[li].second;
      ++li;
    } else {
      auto sp = pick_splitting_query(cand, rng);
      qr = sp.first;
      qc = sp.second;
    }

    int resp = ask(qr, qc);
    if (resp == 0) {
      return;
    }
    if (resp < 0) {
      return;
    }
    cand = filter_by_distance(cand, qr, qc, resp);
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(&cout);
  cout << unitbuf;

  if (!(cin >> N >> queries_left)) {
    return 0;
  }
  T_total = queries_left;

  memset(wall, -1, sizeof(wall));
  for (int r = 1; r <= N; ++r) {
    for (int c = 1; c <= N; ++c) {
      if (is_border(r, c)) {
        wall[r][c] = 1;
      } else if (is_room(r, c)) {
        wall[r][c] = 0;
      }
    }
  }

  uint64_t seed = chrono::steady_clock::now().time_since_epoch().count();
  mt19937 rng((unsigned)seed);

  exploration_phase();

  while (queries_left > 0) {
    localize_one(rng);
  }
  return 0;
}
