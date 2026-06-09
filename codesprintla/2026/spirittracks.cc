#include <bits/stdc++.h>
using namespace std;

namespace {

// --- Fast buffered I/O ---

char IN_BUF[1 << 22];
int IN_POS = 0, IN_LEN = 0;

inline int fast_getc() {
  if (IN_POS == IN_LEN) {
    IN_LEN = (int)fread(IN_BUF, 1, sizeof(IN_BUF), stdin);
    IN_POS = 0;
    if (IN_LEN == 0)
      return -1;
  }
  return (unsigned char)IN_BUF[IN_POS++];
}

inline void read_int(int& out) {
  int c = fast_getc();
  while (c != -1 && (c < '0' || c > '9'))
    c = fast_getc();
  int x = 0;
  while (c >= '0' && c <= '9') {
    x = x * 10 + (c - '0');
    c = fast_getc();
  }
  out = x;
}

char OUT_BUF[1 << 22];
int OUT_POS = 0;

inline void flush_out() {
  fwrite(OUT_BUF, 1, (size_t)OUT_POS, stdout);
  OUT_POS = 0;
}

inline void put_char(char c) {
  if (OUT_POS == (int)sizeof(OUT_BUF))
    flush_out();
  OUT_BUF[OUT_POS++] = c;
}

inline void write_int(int x) {
  if (x == 0) {
    put_char('0');
    return;
  }
  char tmp[16];
  int n = 0;
  while (x > 0) {
    tmp[n++] = (char)('0' + x % 10);
    x /= 10;
  }
  while (n--)
    put_char(tmp[n]);
}

struct Edge {
  int to;
  int pr;
};

// Single cache line per interesting node: sz + thr + 3*(to, idx_at_to) = 4+4+24 = 32 bytes.
struct alignas(32) Node {
  int sz;
  int thr;
  int to[3];
  int idx[3];
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M, Q;
  read_int(N);
  read_int(M);
  read_int(Q);

  vector<vector<Edge>> adj((size_t)N + 1);
  for (int i = 0; i < N - 1; i++) {
    int u, v, p;
    read_int(u);
    read_int(v);
    read_int(p);
    adj[(size_t)u].push_back({v, p});
    adj[(size_t)v].push_back({u, p});
  }

  vector<Node> nodes((size_t)N + 1);
  for (int u = 0; u <= N; ++u)
    nodes[(size_t)u].sz = 0;

  for (int i = 0; i < M; i++) {
    int j, s;
    read_int(j);
    read_int(s);
    nodes[(size_t)j].thr = s;
  }

  // Stage 1: build compressed entries (out_pr_at_u, dest, in_pr_at_dest), sorted by out_pr.
  struct TmpEntry {
    int out_pr;
    int to;
    int in_pr;
  };
  vector<vector<TmpEntry>> tmp((size_t)N + 1);
  for (int u = 1; u <= N; ++u) {
    if ((int)adj[(size_t)u].size() == 2)
      continue;
    tmp[(size_t)u].reserve(adj[(size_t)u].size());
    for (const Edge& first : adj[(size_t)u]) {
      int prev = u;
      int cur = first.to;
      int lastpr = first.pr;
      while ((int)adj[(size_t)cur].size() == 2) {
        for (const Edge& e : adj[(size_t)cur]) {
          if (e.to != prev) {
            prev = cur;
            cur = e.to;
            lastpr = e.pr;
            break;
          }
        }
      }
      tmp[(size_t)u].push_back({first.pr, cur, lastpr});
    }
    sort(tmp[(size_t)u].begin(), tmp[(size_t)u].end(),
         [](const TmpEntry& a, const TmpEntry& b) { return a.out_pr < b.out_pr; });
  }

  // Stage 2: resolve peer index at dest and write packed Node table.
  for (int u = 1; u <= N; ++u) {
    const int sz = (int)tmp[(size_t)u].size();
    Node& nd = nodes[(size_t)u];
    nd.sz = sz;
    for (int k = 0; k < sz; ++k) {
      const TmpEntry& e = tmp[(size_t)u][(size_t)k];
      nd.to[k] = e.to;
      int peer = -1;
      const auto& nbr = tmp[(size_t)e.to];
      for (int t = 0; t < (int)nbr.size(); ++t) {
        if (nbr[(size_t)t].out_pr == e.in_pr) {
          peer = t;
          break;
        }
      }
      nd.idx[k] = peer;
    }
  }

  // Free the temporary adjacency / entry tables before the query loop.
  vector<vector<Edge>>().swap(adj);
  vector<vector<TmpEntry>>().swap(tmp);

  // CHOICE[gt][eidx]: gt = (f > thr) ? 1 : 0; eidx = incoming entry index 0/1/2.
  //   gt=1 (f > thr) -> take greater of the other two
  //   gt=0 (f <= thr) -> take lesser of the other two
  const int CHOICE[2][3] = {
      {1, 0, 0}, // gt=0 (lesser)
      {2, 2, 1}, // gt=1 (greater)
  };

  Node* nd_ptr = nodes.data();

  for (int qi = 0; qi < Q; qi++) {
    int type;
    read_int(type);
    if (type == 1) {
      int a, b;
      read_int(a);
      read_int(b);
      nd_ptr[a].thr = b;
    } else {
      int d, f;
      read_int(d);
      read_int(f);
      const Node& sn = nd_ptr[d];
      int cur = sn.to[0];
      int eidx = sn.idx[0];
      while (nd_ptr[cur].sz != 1) {
        const Node& j = nd_ptr[cur];
        const int gt = (f > j.thr);
        const int tgt = CHOICE[gt][eidx];
        eidx = j.idx[tgt];
        cur = j.to[tgt];
        __builtin_prefetch(&nd_ptr[cur]);
      }
      write_int(cur);
      put_char('\n');
    }
  }
  flush_out();
  return 0;
}
