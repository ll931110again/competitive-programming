#include <bits/stdc++.h>
using namespace std;

class RobotSimulation {
  int dx[4], dy[4];

  int dir(char c) {
    if (c == 'U')
      return 0;
    if (c == 'D')
      return 1;
    if (c == 'L')
      return 2;
    return 3;
  }

  long long runPeriod(long long& x, long long& y, const string& program,
                      set<pair<long long, long long>>& vis) {
    long long added = 0;
    for (char c : program) {
      int d = dir(c);
      x += dx[d];
      y += dy[d];
      if (!vis.count({x, y})) {
        vis.insert({x, y});
        added++;
      }
    }
    return added;
  }

public:
  long long cellsVisited(string program, int times) {
    dx[0] = 0;
    dy[0] = 1;
    dx[1] = 0;
    dy[1] = -1;
    dx[2] = -1;
    dy[2] = 0;
    dx[3] = 1;
    dy[3] = 0;

    int L = program.size();
    bool allSame = true;
    for (int i = 1; i < L; i++)
      if (program[i] != program[0])
        allSame = false;
    if (allSame)
      return 1LL * times * L + 1;

    map<tuple<long long, long long, int>, int> seen;
    set<pair<long long, long long>> vis;
    long long x = 0, y = 0;
    long long total = 1;
    vis.insert({0, 0});

    for (int t = 0; t < times;) {
      auto key = make_tuple(x, y, 0);
      if (seen.count(key)) {
        int t0 = seen[key];
        int cycle = t - t0;
        int rem = times - t;
        int full = rem / cycle;
        int extra = rem % cycle;

        long long before = total;
        for (int i = t0 + 1; i <= t; i++)
          (void)i;
        long long x2 = x, y2 = y;
        set<pair<long long, long long>> vis2 = vis;
        // rewind: re-simulate from t0 state is hard; instead simulate one cycle from current
        // at detection point (x,y) matches state at t0
        long long tx = x, ty = y;
        long long perAdd = runPeriod(tx, ty, program, vis2);
        long long perNew = vis2.size() - vis.size();
        total += perNew * full;
        for (int e = 0; e < extra; e++)
          total += runPeriod(x, y, program, vis);
        return total;
      }
      seen[key] = t;
      total += runPeriod(x, y, program, vis);
      t++;
    }
    return total;
  }
};
