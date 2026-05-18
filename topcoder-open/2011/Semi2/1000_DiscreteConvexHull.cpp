#include <bits/stdc++.h>
using namespace std;

class DiscreteConvexHull {
public:
  long long minArea(vector<string> cells) {
    vector<pair<long long, long long>> pts;
    for (const string& row : cells) {
      string s = row;
      for (int i = 0; i < (int)s.size();) {
        while (i < (int)s.size() && s[i] == ' ')
          i++;
        if (i >= (int)s.size())
          break;
        int j = i;
        while (j < (int)s.size() && s[j] != ' ')
          j++;
        string tok = s.substr(i, j - i);
        int comma = (int)tok.find(',');
        long long x = stoll(tok.substr(0, comma));
        long long y = stoll(tok.substr(comma + 1));
        pts.push_back({x, y});
        i = j;
      }
    }

    if (pts.empty())
      return 0;
    if (pts.size() == 1)
      return 1;

    vector<long long> xs, ys;
    for (auto p : pts) {
      xs.push_back(p.first);
      ys.push_back(p.second);
    }
    sort(xs.begin(), xs.end());
    sort(ys.begin(), ys.end());
    xs.erase(unique(xs.begin(), xs.end()), xs.end());
    ys.erase(unique(ys.begin(), ys.end()), ys.end());

    int nx = (int)xs.size();
    int ny = (int)ys.size();
    vector<vector<char>> in(nx, vector<char>(ny, 0));
    auto getX = [&](long long v) {
      return (int)(lower_bound(xs.begin(), xs.end(), v) - xs.begin());
    };
    auto getY = [&](long long v) {
      return (int)(lower_bound(ys.begin(), ys.end(), v) - ys.begin());
    };
    for (auto p : pts)
      in[getX(p.first)][getY(p.second)] = 1;

    bool changed = true;
    while (changed) {
      changed = false;
      for (int i = 0; i < nx; i++) {
        int l = ny, r = -1;
        for (int j = 0; j < ny; j++)
          if (in[i][j]) {
            l = min(l, j);
            r = max(r, j);
          }
        if (r >= 0) {
          for (int j = l; j <= r; j++)
            if (!in[i][j]) {
              in[i][j] = 1;
              changed = true;
            }
        }
      }
      for (int j = 0; j < ny; j++) {
        int l = nx, r = -1;
        for (int i = 0; i < nx; i++)
          if (in[i][j]) {
            l = min(l, i);
            r = max(r, i);
          }
        if (r >= 0) {
          for (int i = l; i <= r; i++)
            if (!in[i][j]) {
              in[i][j] = 1;
              changed = true;
            }
        }
      }
    }

    long long area = 0;
    for (int i = 0; i + 1 < nx; i++) {
      long long dx = xs[i + 1] - xs[i];
      for (int j = 0; j + 1 < ny; j++) {
        if (!in[i][j])
          continue;
        long long dy = ys[j + 1] - ys[j];
        area += dx * dy;
      }
    }
    return area;
  }
};
