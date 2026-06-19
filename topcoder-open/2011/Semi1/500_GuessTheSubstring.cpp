#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
class GuessTheSubstring {
  struct SAM {
    struct State {
      int next[26];
      int link;
      int len;
      i64 occ;
      State() : link(-1), len(0), occ(0) {
        memset(next, -1, sizeof(next));
      }
    };
    vector<State> st;
    int last;

    SAM(int n = 0) {
      st.reserve(2 * n + 5);
      st.push_back(State());
      last = 0;
    }

    void extend(int c) {
      int cur = (int)st.size();
      st.push_back(State());
      st[cur].len = st[last].len + 1;
      int p = last;
      while (p != -1 && st[p].next[c] == -1) {
        st[p].next[c] = cur;
        p = st[p].link;
      }
      if (p == -1) {
        st[cur].link = 0;
      } else {
        int q = st[p].next[c];
        if (st[p].len + 1 == st[q].len) {
          st[cur].link = q;
        } else {
          int clone = (int)st.size();
          st.push_back(st[q]);
          st[clone].len = st[p].len + 1;
          while (p != -1 && st[p].next[c] == q) {
            st[p].next[c] = clone;
            p = st[p].link;
          }
          st[q].link = st[cur].link = clone;
        }
      }
      last = cur;
    }

    void buildOcc() {
      vector<int> order(st.size());
      iota(order.begin(), order.end(), 0);
      sort(order.begin(), order.end(), [&](int a, int b) { return st[a].len > st[b].len; });
      for (int v : order) {
        if (st[v].link != -1)
          st[st[v].link].occ += st[v].occ;
      }
    }
  };

  double huffman(vector<double> w) {
    if (w.empty())
      return 0.0;
    if (w.size() == 1)
      return 0.0;
    priority_queue<double, vector<double>, greater<double>> pq;
    for (double x : w)
      pq.push(x);
    double cost = 0.0;
    while (pq.size() > 1) {
      double a = pq.top();
      pq.pop();
      double b = pq.top();
      pq.pop();
      double s = a + b;
      cost += s;
      pq.push(s);
    }
    return cost;
  }

public:
  double solve(vector<string> pieces) {
    string s;
    for (const string& p : pieces)
      s += p;
    int n = (int)s.size();
    SAM sam(n);
    for (int i = 0; i < n; i++) {
      sam.extend(s[i] - 'a');
      sam.st[sam.last].occ++;
    }
    sam.buildOcc();

    vector<double> weights;
    for (int v = 1; v < (int)sam.st.size(); v++) {
      int lo = sam.st[sam.st[v].link].len + 1;
      int hi = sam.st[v].len;
      double w = (double)sam.st[v].occ / (double)(1LL * n * (n + 1) / 2);
      for (int len = lo; len <= hi; len++)
        weights.push_back(w);
    }
    return huffman(weights);
  }
};
