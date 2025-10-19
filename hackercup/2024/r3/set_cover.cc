#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#define maxn 2505
using namespace std;

int T, n, K;
vector<string> grid;

int solve_top_left(vector<string>& grid) {
    int minX = n, maxX = -1, minY = n, maxY = -1;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) if (grid[i][j] == '1') {
            if (minX < 0 || minX > i) {
                minX = i;
            }
            maxX = max(maxX, i);
            if (minY < 0 || minY > j) {
                minY = j;
            }
            maxY = max(maxY, j);
            grid[i][j] = '?';
        }
    }

    int ans = 0;
    if (maxX >= 0) {
        ans = (maxX - minX + 1) * (maxY - minY + 1);
    }

    if (K == 1) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) if (grid[i][j] == '?') {
                int ax = min(minX, i);
                int bx = max(maxX, i);
                int ay = min(minY, j);
                int by = max(maxY, j);
                ans = max(ans, (bx - ax + 1) * (by - ay + 1));
            }
        }
    }

    if (K == 2) {
        vector< pair<int,int> > top_lefts, bottom_rights;
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) if (grid[i][j] == '?') {
                top_lefts.push_back({i, j});
                break;
            }

            for (int i = n - 1; i >= 0; i--) if (grid[i][j] == '?') {
                bottom_rights.push_back({i, j});
                break;
            }
        }

        for (auto u : top_lefts) {
            for (auto v : bottom_rights) {
                int ax = min(minX, u.first);
                ax = min(ax, v.first);

                int bx = max(maxX, u.first);
                bx = max(bx, v.first);

                int ay = min(minY, u.second);
                ay = min(ay, v.second);

                int by = max(maxY, u.second);
                by = max(by, v.second);

                ans = max(ans, (bx - ax + 1) * (by - ay + 1));
            }
        }
    }

    if (K == 3) {
        vector< pair<int,int> > top_lefts;
        for (int j = 0; j < n; j++) {
            for (int i = 0; i < n; i++) if (grid[i][j] == '?') {
                top_lefts.push_back({i, j});
                break;
            }
        }

        pair<int,int> rightmost = {-1, -1}, bottommost = {-1, -1};
        for (int j = n - 1; j >= 0; j--) {
            if (rightmost.first >= 0) {
                break;
            }

            for (int i = 0; i < n; i++) if (grid[i][j] == '?') {
                rightmost = {i, j};
                break;
            }
        }

        for (int i = n - 1; i >= 0; i--) {
            if (bottommost.first >= 0) {
                break;
            }

            for (int j = 0; j < n; j++) if (grid[i][j] == '?') {
                bottommost = {i, j};
                break;
            }
        }

        int ax = min(minX, rightmost.first);
        ax = min(ax, bottommost.first);

        int bx = max(maxX, rightmost.first);
        bx = max(bx, bottommost.first);

        int ay = min(minY, rightmost.second);
        ay = min(ay, bottommost.second);

        int by = max(maxY, rightmost.second);
        by = max(by, bottommost.second);

        for (auto u : top_lefts) {
            int cx = min(ax, u.first);
            int dx = max(bx, u.first);
            int cy = min(ay, u.second);
            int dy = max(by, u.second);
            ans = max(ans, (dx - cx + 1) * (dy - cy + 1));          
        }
    }

    if (K >= 4) {
        int ax = minX, bx = maxX, ay = minY, by = maxY;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) if (grid[i][j] == '?') {
                ax = min(ax, i);
                bx = max(bx, i);
                ay = min(ay, j);
                by = max(by, j);
            }
        }
        ans = max(ans, (bx - ax + 1) * (by - ay + 1));
    }

    return ans;
}

vector<string> rotate(vector<string> grid) {
    vector<string> ans = grid;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            ans[j][n - 1 - i] = grid[i][j];
        }
    }
    return ans;
}

int solve() {
    int ans = 0;
    for (int i = 0; i < 4; i++) {
        auto tmp_grid = grid;
        ans = max(ans, solve_top_left(tmp_grid));
        grid = rotate(grid);
    }
    return ans;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    
    cin >> T;
    for (int it = 1; it <= T; it++) {
        cerr << it << endl;

        cin >> n >> K;
        grid.clear();
        grid.resize(n);
        for (int i = 0; i < n; i++) {
            cin >> grid[i];
        }

        cout << "Case #" << it << ": " << solve() << endl;
    }

    return 0;
}