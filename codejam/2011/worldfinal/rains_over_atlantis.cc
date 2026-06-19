#include <bits/stdc++.h>
using namespace std;
using i64 = long long;
namespace {

const int DX[4] = {1, -1, 0, 0};
const int DY[4] = {0, 0, 1, -1};

vector<i64> water_levels(int H, int W, const vector<i64>& h) {
  const int N = H * W;
  const i64 INF = numeric_limits<i64>::max() / 4;

  vector<i64> wl(N, INF);
  using Node = pair<i64, int>; // (wl, idx)
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
      i64 cand = max(h[u], cur);
      if (cand < wl[u]) {
        wl[u] = cand;
        pq.push({cand, u});
      }
    }
  }

  return wl;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int H, W;
    i64 M;
    cin >> H >> W >> M;
    const int N = H * W;
    vector<i64> h(N);
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

    i64 days = 0;
    while (!all_zero()) {
      vector<i64> wl = water_levels(H, W, h);

      auto min_adj_wl = [&](int r, int c) -> i64 {
        int v = r * W + c;
        i64 best = (r == 0 || r == H - 1 || c == 0 || c == W - 1) ? 0 : (i64)4e18;
        for (int dir = 0; dir < 4; dir++) {
          int nr = r + DX[dir], nc = c + DY[dir];
          if (nr < 0 || nr >= H || nc < 0 || nc >= W)
            continue;
          best = min(best, wl[nr * W + nc]);
        }
        return best;
      };

      bool can_batch = true;
      i64 batch_t = (i64)4e18;
      i64 max_h = 0;

      for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
          int v = r * W + c;
          max_h = max(max_h, h[v]);
          i64 down = min_adj_wl(r, c);
          i64 drop = wl[v] - down;
          if (wl[v] == h[v]) {
            if (h[v] > 0 && drop < M)
              can_batch = false;
          } else {
            // submerged: wl decreases by M per day in a batch, height stays fixed.
            i64 water = wl[v] - h[v];
            i64 t = (water + M - 1) / M; // first day when wl' <= h
            batch_t = min(batch_t, t);
          }
        }
      }

      if (can_batch) {
        if (batch_t == (i64)4e18) {
          // No submerged cells; once every exposed cell can erode at rate M,
          // the whole map erodes to <=0 in ceil(max_h / M) days.
          batch_t = (max_h + M - 1) / M;
        }

        // Apply batch.
        for (int i = 0; i < N; i++) {
          if (wl[i] == h[i] && h[i] > 0) {
            __int128 nh = (__int128)h[i] - (__int128)batch_t * (__int128)M;
            h[i] = (nh > 0) ? (i64)nh : 0;
          }
        }
        days += batch_t;
        continue;
      }

      // Single-day step.
      vector<i64> nh = h;
      for (int r = 0; r < H; r++) {
        for (int c = 0; c < W; c++) {
          int v = r * W + c;
          if (h[v] == 0 && wl[v] == 0)
            continue;
          i64 down = min_adj_wl(r, c);
          i64 drop = wl[v] - down;
          i64 dec = min(drop, M);
          i64 val = h[v] - dec;
          nh[v] = max<i64>(0, val);
        }
      }
      h.swap(nh);
      days++;
    }

    cout << "Case #" << tc << ": " << days << "\n";
  }
  return 0;
}
