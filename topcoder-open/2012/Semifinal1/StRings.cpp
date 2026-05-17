#include <vector>
using namespace std;

class StRings {
  static bool partialValid(const vector<int>& S, int N) {
    vector<char> seenImg(N + 1, 0);
    for (int v = 1; v <= N; ++v) {
      if (S[v - 1] == -1)
        continue;
      int y = S[v - 1];
      if (seenImg[y])
        return false;
      seenImg[y] = 1;
    }
    vector<int> mark(N + 1, 0), firstStep(N + 1, 0);
    int token = 1;
    for (int s = 1; s <= N; ++s) {
      if (S[s - 1] == -1)
        continue;
      ++token;
      int x = s;
      int step = 0;
      while (true) {
        if (mark[x] == token) {
          int L = step - firstStep[x];
          if (L < N)
            return false;
          break;
        }
        if (S[x - 1] == -1)
          break;
        mark[x] = token;
        firstStep[x] = step;
        x = S[x - 1];
        ++step;
      }
    }
    return true;
  }

  // After setting S[pos] = cand (cand unused), check that pi has no directed
  // cycle shorter than N. Equivalently: walk u=pos+1 -> cand -> S[cand-1] -> ...
  static bool canPut(const vector<int>& S, int N, int pos, int cand) {
    int u = pos + 1;
    int cur = cand;
    int edges = 1;
    if (cur == u)
      return N == 1;
    while (true) {
      int nx = S[cur - 1];
      if (nx == -1)
        return true;
      ++edges;
      cur = nx;
      if (cur == u)
        return edges >= N;
      if (edges > N)
        return true;
    }
  }

public:
  vector<int> getSmallest(int N, vector<int> first) {
    const int L = (int)first.size();
    vector<int> S(N, -1);
    vector<char> used(N + 1, 0);
    for (int i = 0; i < L; ++i) {
      S[i] = first[i];
      used[first[i]] = 1;
    }
    if (!partialValid(S, N))
      return {};

    for (int pos = L; pos < N; ++pos) {
      bool placed = false;
      for (int cand = 1; cand <= N; ++cand) {
        if (used[cand])
          continue;
        if (!canPut(S, N, pos, cand))
          continue;
        S[pos] = cand;
        used[cand] = 1;
        placed = true;
        break;
      }
      if (!placed)
        return {};
    }

    if (N > 50)
      return vector<int>(S.end() - 50, S.end());
    return S;
  }
};
