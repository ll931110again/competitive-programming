#include <algorithm>
#include <cstdint>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Wall {
    int x1, y, x2;
};

static int findBlockIndex(const vector<int>& coords, int v) {
    // returns i such that coords[i] <= v < coords[i+1]
    int lo = 0, hi = (int)coords.size() - 2;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (coords[mid] <= v && v < coords[mid + 1]) return mid;
        if (v < coords[mid]) hi = mid - 1;
        else lo = mid + 1;
    }
    return -1;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n, w;
    int caseNum = 1;
    while (cin >> m >> n >> w) {
        if (m == 0 && n == 0 && w == 0) break;

        vector<Wall> walls;
        walls.reserve(w);
        long long blockedCells = 0;

        vector<int> xs, ys;
        xs.push_back(0);
        xs.push_back(n);
        ys.push_back(0);
        ys.push_back(m);

        for (int i = 0; i < w; i++) {
            int x1, y1, x2, y2;
            cin >> x1 >> y1 >> x2 >> y2;
            Wall ww;
            ww.x1 = x1;
            ww.x2 = x2;
            ww.y = y1;
            walls.push_back(ww);
            blockedCells += (long long)(x2 - x1 + 1);

            xs.push_back(x1);
            xs.push_back(x2 + 1);
            ys.push_back(y1);
            ys.push_back(y1 + 1);
        }

        sort(xs.begin(), xs.end());
        xs.erase(unique(xs.begin(), xs.end()), xs.end());
        sort(ys.begin(), ys.end());
        ys.erase(unique(ys.begin(), ys.end()), ys.end());

        int X = (int)xs.size() - 1; // blocks
        int Y = (int)ys.size() - 1;

        vector<uint8_t> blocked((size_t)X * (size_t)Y, 0);
        auto id = [&](int ix, int iy) -> int { return iy * X + ix; };

        for (const auto& ww : walls) {
            int iy = findBlockIndex(ys, ww.y);
            int ix1 = findBlockIndex(xs, ww.x1);
            int ix2 = findBlockIndex(xs, ww.x2);
            for (int ix = ix1; ix <= ix2; ix++) blocked[id(ix, iy)] = 1;
        }

        int sx = findBlockIndex(xs, n - 1);
        int sy = findBlockIndex(ys, m - 1);

        vector<uint8_t> vis((size_t)X * (size_t)Y, 0);
        queue<pair<int, int>> q;
        if (sx >= 0 && sy >= 0 && !blocked[id(sx, sy)]) {
            vis[id(sx, sy)] = 1;
            q.push({sx, sy});
        }

        while (!q.empty()) {
            int ix = q.front().first;
            int iy = q.front().second;
            q.pop();

            // move west
            if (ix - 1 >= 0) {
                int nx = ix - 1, ny = iy;
                int nid = id(nx, ny);
                if (!blocked[nid] && !vis[nid]) {
                    vis[nid] = 1;
                    q.push({nx, ny});
                }
            }
            // move south
            if (iy - 1 >= 0) {
                int nx = ix, ny = iy - 1;
                int nid = id(nx, ny);
                if (!blocked[nid] && !vis[nid]) {
                    vis[nid] = 1;
                    q.push({nx, ny});
                }
            }
        }

        long long reachable = 0;
        for (int iy = 0; iy < Y; iy++) {
            long long h = (long long)ys[iy + 1] - ys[iy];
            for (int ix = 0; ix < X; ix++) {
                if (!vis[id(ix, iy)]) continue;
                long long wdt = (long long)xs[ix + 1] - xs[ix];
                reachable += wdt * h;
            }
        }

        long long totalCells = 1LL * m * n;
        long long emptyCells = totalCells - blockedCells;
        long long stuck = emptyCells - reachable;

        cout << "Case " << caseNum++ << ": " << stuck << "\n";
    }
    return 0;
}

