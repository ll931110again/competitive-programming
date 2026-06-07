#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>
#include <vector>

static std::vector<int> bits;
static int bit_pos;

static void encode_bit(int b) {
  bits.push_back(b);
}

static int decode_bit() {
  return bits[bit_pos++];
}

static int got_hops[1005][36];
static int hop_calls;

void hops(int h, int c, int d) {
  got_hops[h][c] = d;
  ++hop_calls;
}

#include "saveit.cc"

static void bfs_dist(int n, int h, const std::vector<int> g[], int ans[1005][36]) {
  for (int hub = 0; hub < h; ++hub) {
    std::fill(ans[hub], ans[hub] + n, -1);
    std::queue<int> q;
    ans[hub][hub] = 0;
    q.push(hub);
    while (!q.empty()) {
      const int u = q.front();
      q.pop();
      for (int v : g[u]) {
        if (ans[hub][v] == -1) {
          ans[hub][v] = ans[hub][u] + 1;
          q.push(v);
        }
      }
    }
  }
}

static bool read_saveit(const char* path, int& n, int& h, int& p, int a[], int b[]) {
  FILE* f = fopen(path, "r");
  if (!f) {
    return false;
  }
  if (fscanf(f, "%d %d %d", &n, &p, &h) != 3) {
    fclose(f);
    return false;
  }
  for (int i = 0; i < p; ++i) {
    if (fscanf(f, "%d %d", &a[i], &b[i]) != 2) {
      fclose(f);
      return false;
    }
  }
  fclose(f);
  return true;
}

int main(int argc, char** argv) {
  const char* path = (argc > 1) ? argv[1] : "saveit/appeal/Subtask1-data/grader.in.0a";
  static int a[1000005], b[1000005];
  int n, h, p;
  if (!read_saveit(path, n, h, p, a, b)) {
    return 1;
  }

  std::vector<int> g[1005];
  for (int i = 0; i < p; ++i) {
    g[a[i]].push_back(b[i]);
    g[b[i]].push_back(a[i]);
  }

  bits.clear();
  encode(n, h, p, a, b);
  const int encoded_bits = (int)bits.size();

  memset(got_hops, 0, sizeof got_hops);
  hop_calls = 0;
  bit_pos = 0;
  decode(n, h);

  int expect[1005][36];
  bfs_dist(n, h, g, expect);

  for (int i = 0; i < h; ++i) {
    for (int c = 0; c < n; ++c) {
      if (got_hops[i][c] != expect[i][c]) {
        printf("FAIL %s: hops(%d,%d) got %d want %d\n", path, i, c, got_hops[i][c], expect[i][c]);
        return 1;
      }
    }
  }

  printf("OK %s n=%d h=%d bits=%d hops_calls=%d\n", path, n, h, encoded_bits, hop_calls);
  return 0;
}
