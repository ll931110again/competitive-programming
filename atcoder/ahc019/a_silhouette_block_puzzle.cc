// AHC019 A - Silhouette Block Puzzle Creation
// https://atcoder.jp/contests/ahc019/tasks/ahc019_a
//
// Greedily peel off the largest polycube shape placeable in both puzzles (24
// rotations), assign a shared block id, then decompose leftovers per puzzle.

#include <algorithm>
#include <array>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_set>
#include <vector>
using namespace std;

using i64 = long long;

namespace {

struct Cell {
  int x, y, z;
  bool operator==(const Cell& o) const { return x == o.x && y == o.y && z == o.z; }
};

struct CellHash {
  size_t operator()(const Cell& c) const {
    return (size_t)c.x * 10007u + (size_t)c.y * 131u + (size_t)c.z;
  }
};

using Shape = vector<Cell>;

int D;
vector<string> f_sil[2], r_sil[2];
using Grid = array<array<array<char, 14>, 14>, 14>;

Grid rem[2], assign[2];

bool valid_sil(int p, int x, int y, int z) {
  return f_sil[p][z][x] == '1' && r_sil[p][z][y] == '1';
}

int idx(int x, int y, int z) { return x * D * D + y * D + z; }

Shape normalize(Shape s) {
  if (s.empty())
    return s;
  int mn_x = s[0].x, mn_y = s[0].y, mn_z = s[0].z;
  for (const auto& c : s) {
    mn_x = min(mn_x, c.x);
    mn_y = min(mn_y, c.y);
    mn_z = min(mn_z, c.z);
  }
  for (auto& c : s) {
    c.x -= mn_x;
    c.y -= mn_y;
    c.z -= mn_z;
  }
  sort(s.begin(), s.end(), [](const Cell& a, const Cell& b) {
    if (a.x != b.x)
      return a.x < b.x;
    if (a.y != b.y)
      return a.y < b.y;
    return a.z < b.z;
  });
  return s;
}

Shape rotate_x(const Shape& s, int max_y, int max_z) {
  Shape t;
  t.reserve(s.size());
  for (const auto& c : s)
    t.push_back({c.x, max_z - c.z, c.y});
  return t;
}

Shape rotate_y(const Shape& s, int max_x, int max_z) {
  Shape t;
  t.reserve(s.size());
  for (const auto& c : s)
    t.push_back({max_z - c.z, c.y, c.x});
  return t;
}

vector<Shape> all_rotations(Shape s) {
  s = normalize(s);
  int max_x = 0, max_y = 0, max_z = 0;
  for (const auto& c : s) {
    max_x = max(max_x, c.x);
    max_y = max(max_y, c.y);
    max_z = max(max_z, c.z);
  }
  vector<Shape> rots;
  unordered_set<string> seen;
  for (int i = 0; i < 6; ++i) {
    for (int t = 0; t < 4; ++t) {
      Shape cur = normalize(s);
      string key;
      for (const auto& c : cur)
        key += to_string(c.x) + "," + to_string(c.y) + "," + to_string(c.z) + ";";
      if (seen.insert(key).second)
        rots.push_back(cur);
      s = rotate_x(s, max_y, max_z);
      swap(max_y, max_z);
    }
    if (i & 1) {
      s = rotate_y(s, max_x, max_z);
      swap(max_x, max_z);
    } else {
      for (auto& c : s) {
        const int nt = max_x - c.x;
        c.x = c.z;
        c.z = nt;
      }
      swap(max_x, max_z);
    }
  }
  return rots;
}

bool find_placement(const Shape& rel, int p, Shape& placed) {
  for (const Shape& rot : all_rotations(rel)) {
    for (int ox = 0; ox < D; ++ox) {
      for (int oy = 0; oy < D; ++oy) {
        for (int oz = 0; oz < D; ++oz) {
          bool ok = true;
          placed.clear();
          placed.reserve(rot.size());
          for (const auto& c : rot) {
            const int x = ox + c.x, y = oy + c.y, z = oz + c.z;
            if (x >= D || y >= D || z >= D || !rem[p][x][y][z]) {
              ok = false;
              break;
            }
            placed.push_back({x, y, z});
          }
          if (ok)
            return true;
        }
      }
    }
  }
  return false;
}

bool contains(const Shape& s, const Cell& c) {
  for (const auto& v : s)
    if (v == c)
      return true;
  return false;
}

Shape grow_shared(const Cell& seed) {
  Shape shape = {seed};
  bool changed = true;
  while (changed) {
    changed = false;
    vector<Cell> cand;
    for (const auto& c : shape) {
      constexpr int dx[6] = {1, -1, 0, 0, 0, 0};
      constexpr int dy[6] = {0, 0, 1, -1, 0, 0};
      constexpr int dz[6] = {0, 0, 0, 0, 1, -1};
      for (int d = 0; d < 6; ++d) {
        const Cell n{c.x + dx[d], c.y + dy[d], c.z + dz[d]};
        if (n.x < 0 || n.x >= D || n.y < 0 || n.y >= D || n.z < 0 || n.z >= D)
          continue;
        if (!rem[0][n.x][n.y][n.z] || contains(shape, n))
          continue;
        cand.push_back(n);
      }
    }
    sort(cand.begin(), cand.end(), [](const Cell& a, const Cell& b) {
      if (a.x != b.x)
        return a.x < b.x;
      if (a.y != b.y)
        return a.y < b.y;
      return a.z < b.z;
    });
    cand.erase(unique(cand.begin(), cand.end(), [](const Cell& a, const Cell& b) {
                 return a.x == b.x && a.y == b.y && a.z == b.z;
               }),
               cand.end());
    Shape rel_shape;
    rel_shape.reserve(shape.size());
    for (const auto& c : shape)
      rel_shape.push_back({c.x - seed.x, c.y - seed.y, c.z - seed.z});
    for (const Cell& n : cand) {
      Shape trial = rel_shape;
      trial.push_back({n.x - seed.x, n.y - seed.y, n.z - seed.z});
      Shape dummy;
      if (find_placement(trial, 1, dummy)) {
        shape.push_back(n);
        rel_shape = move(trial);
        changed = true;
        break;
      }
    }
  }
  return shape;
}

void remove_cells(int p, const Shape& cells) {
  for (const auto& c : cells)
    rem[p][c.x][c.y][c.z] = 0;
}

void paint(int p, int id, const Shape& cells) {
  for (const auto& c : cells)
    assign[p][c.x][c.y][c.z] = (char)id;
}

vector<Shape> decompose_remaining(int p) {
  vector<Shape> blocks;
  constexpr int dx[6] = {1, -1, 0, 0, 0, 0};
  constexpr int dy[6] = {0, 0, 1, -1, 0, 0};
  constexpr int dz[6] = {0, 0, 0, 0, 1, -1};
  for (int x = 0; x < D; ++x) {
    for (int y = 0; y < D; ++y) {
      for (int z = 0; z < D; ++z) {
        if (!rem[p][x][y][z])
          continue;
        Shape comp;
        queue<Cell> q;
        q.push({x, y, z});
        rem[p][x][y][z] = 0;
        while (!q.empty()) {
          const Cell c = q.front();
          q.pop();
          comp.push_back(c);
          for (int d = 0; d < 6; ++d) {
            const int nx = c.x + dx[d], ny = c.y + dy[d], nz = c.z + dz[d];
            if (nx < 0 || nx >= D || ny < 0 || ny >= D || nz < 0 || nz >= D)
              continue;
            if (!rem[p][nx][ny][nz])
              continue;
            rem[p][nx][ny][nz] = 0;
            q.push({nx, ny, nz});
          }
        }
        blocks.push_back(std::move(comp));
      }
    }
  }
  return blocks;
}

void solve() {
  for (int p = 0; p < 2; ++p) {
    for (int x = 0; x < D; ++x) {
      for (int y = 0; y < D; ++y) {
        for (int z = 0; z < D; ++z) {
          rem[p][x][y][z] = valid_sil(p, x, y, z);
          assign[p][x][y][z] = 0;
        }
      }
    }
  }

  int n = 0;
  while (true) {
    Cell best_seed{-1, -1, -1};
    Shape best0, best1;
    int best_vol = 0;
    for (int x = 0; x < D; ++x) {
      for (int y = 0; y < D; ++y) {
        for (int z = 0; z < D; ++z) {
          if (!rem[0][x][y][z])
            continue;
          const Cell seed{x, y, z};
          Shape s0 = grow_shared(seed);
          Shape rel;
          rel.reserve(s0.size());
          for (const auto& c : s0)
            rel.push_back({c.x - seed.x, c.y - seed.y, c.z - seed.z});
          Shape s1;
          if (!find_placement(rel, 1, s1))
            continue;
          if ((int)s0.size() > best_vol) {
            best_vol = (int)s0.size();
            best_seed = seed;
            best0 = move(s0);
            best1 = move(s1);
          }
        }
      }
    }
    if (best_vol == 0)
      break;
    ++n;
    paint(0, n, best0);
    paint(1, n, best1);
    remove_cells(0, best0);
    remove_cells(1, best1);
  }

  for (int p = 0; p < 2; ++p) {
    for (const Shape& comp : decompose_remaining(p)) {
      ++n;
      paint(p, n, comp);
    }
  }

  cout << n << '\n';
  for (int p = 0; p < 2; ++p) {
    for (int i = 0; i < D * D * D; ++i) {
      if (i)
        cout << ' ';
      const int x = i / (D * D);
      const int y = (i / D) % D;
      const int z = i % D;
      cout << (int)assign[p][x][y][z];
    }
    cout << '\n';
  }
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> D;
  for (int p = 0; p < 2; ++p) {
    f_sil[p].resize(D);
    r_sil[p].resize(D);
    for (int z = 0; z < D; ++z)
      cin >> f_sil[p][z];
    for (int z = 0; z < D; ++z)
      cin >> r_sil[p][z];
  }

  solve();
  return 0;
}
