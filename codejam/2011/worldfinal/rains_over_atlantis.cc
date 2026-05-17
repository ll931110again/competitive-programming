#include <algorithm>
#include <iostream>
#include <limits>
#include <queue>
#include <tuple>
#include <vector>

using namespace std;

using int64 = long long;

static const int DX[4] = {1, -1, 0, 0};
static const int DY[4] = {0, 0, 1, -1};

static vector<int64> water_levels(int H, int W, const vector<int64>& h) {
  const int N = H * W;
  const int64 INF = numeric_limits<int64>::max() / 4;

  vector<int64> wl(N, INF);
  using Node = pair<int64, int>; // (wl, idx)
  priority_queue<Node, vector<Node>, greater<Node>> pq;

  auto id = [&](int r, int c) { return r * W + c; };

  // Boundary cells can drain directly to the outside (which has level 0),
  // so their water level is just their own height.
  for (int r = 0; r < H; r++) {
    for (int c = 0; c < W; c++) {
      if (r == 0 || r == H - 1 || c == 0 || c == W - 1) {
        int v = id(r, c);
        wl[v] = h[v];
        pq.push({wl[v], v});
      }
    }
  }

  while (!pq.empty()) {
    auto [cur, v] = pq.top();
    pq.pop();
    if (cur != wl[v])
      continue;
    int r = v / W, c = v % W;
    for (int dir = 0; dir < 4; dir++) {
      int nr = r + DX[dir], nc = c + DY[dir];
      if (nr < 0 || nr >= H || nc < 0 || nc >= W)
        continue;
      int u = id(nr, nc);
      int64 cand = max(h[u], cur);
      if (cand < wl[u]) {
        wl[u] = cand;
        pq.push({cand, u});
      }
    }
  }

  return wl;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int H, W;
    int64 M;
    cin >> H >> W >> M;
    const int N = H * W;
    vector<int64> h(N);
    for (int r = 0; r < H; r++) {
      for (int c = 0; c < W; c++) {
        cin >> h[r * W + c];
      }
    }

    auto all_zero = [&]() {
      for (int i = 0; i < N; i++)
        if (h[i] > 0)
          return false;
      return true;
    };

    int64 days = 0;
    while (!all_zero()) {
      vector<int64> wl = water_levels(H, W, h);

      auto min_adj_wl = [&](int r, int c) -> int64 {
        int v = r * W + c;
        int64 best = (r == 0 || r == H - 1 || c == 0 || c == W - 1) ? 0 : (int64)4e18;
        for (int dir = 0; dir < 4; dir++) {
          int nr = r + DX[dir], nc = c + DY[dir];
          if (nr < 0 || nr >= H || nc < 0 || nc >= W)
            continue;
          best = min(best, wl[nr * W + nc]);
        }
        return best;
      };

      bool can_batch = true;
      int64 batch_t = (int64)4e18;
      int64 max_h = 0;

      for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
          int v = r * W + c;
          max_h = max(max_h, h[v]);
          int64 down = min_adj_wl(r, c);
          int64 drop = wl[v] - down;
          if (wl[v] == h[v]) {
            if (h[v] > 0 && drop < M)
              can_batch = false;
          } else {
            // submerged: wl decreases by M per day in a batch, height stays fixed.
            int64 water = wl[v] - h[v];
            int64 t = (water + M - 1) / M; // first day when wl' <= h
            batch_t = min(batch_t, t);
          }
        }
      }

      if (can_batch) {
        if (batch_t == (int64)4e18) {
          // No submerged cells; once every exposed cell can erode at rate M,
          // the whole map erodes to <=0 in ceil(max_h / M) days.
          batch_t = (max_h + M - 1) / M;
        }

        // Apply batch.
        for (int i = 0; i < N; i++) {
          if (wl[i] == h[i] && h[i] > 0) {
            __int128 nh = (__int128)h[i] - (__int128)batch_t * (__int128)M;
            h[i] = (nh > 0) ? (int64)nh : 0;
          }
        }
        days += batch_t;
        continue;
      }

      // Single-day step.
      vector<int64> nh = h;
      for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
          int v = r * W + c;
          if (h[v] == 0 && wl[v] == 0)
            continue;
          int64 down = min_adj_wl(r, c);
          int64 drop = wl[v] - down;
          int64 dec = min(drop, M);
          int64 val = h[v] - dec;
          nh[v] = max<int64>(0, val);
        }
      }
      h.swap(nh);
      days++;
    }

    cout << "Case #" << tc << ": " << days << "\n";
  }
  return 0;
}