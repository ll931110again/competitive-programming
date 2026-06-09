// IOI 2010 Day 1 — Hotter Colder (full score, O(log(3N)) guesses)
//
// Each Guess(G) compares the secret J to M = (prev + G) / 2. Maintain an interval
// [lo, hi] of candidates and recurse:
//   • "middle game" — interval away from both 1 and N;
//   • "wall game"   — interval touching one boundary (w.l.o.g. [1 .. P]).
//
// Reference: IOI 2010 task review / Gordon Cormack solution outline.

#include "grader.h"
#include <bits/stdc++.h>
using namespace std;

extern int Guess(int g);

enum Result : int { COLDER = -1, SAME = 0, HOTTER = 1 };

namespace {

// Local coordinate x in [1 .. len] against a wall at 1 (left) or N (right).
int to_global(int wall_end, int local) {
  return wall_end == 1 ? local : wall_end - local + 1;
}

// Smallest odd 2^(k+1) - 1 covering `width` consecutive integers.
int odd_cover_width(int width) {
  int sz = 3;
  while (sz < width) {
    sz = 2 * sz + 1;
  }
  return sz;
}

// Candidates J in [lo, hi]; previous guess was `prev` (global coordinate).
int solve_middle(int prev, int lo, int hi) {
  if (lo == hi) {
    return lo;
  }
  if (lo > hi) {
    const int t = lo;
    lo = hi;
    hi = t;
  }

  const int span = odd_cover_width(hi - lo + 1);
  int pivot;

  if (prev < hi - span + 1) {
    pivot = hi - span / 2;
  } else if (prev > lo + span - 1) {
    pivot = lo + span / 2;
  } else if (prev <= lo) {
    pivot = prev + span / 2;
  } else {
    pivot = prev - span / 2;
  }

  const int g = 2 * pivot - prev;
  const Result r = static_cast<Result>(Guess(g));

  if (r == SAME) {
    return pivot;
  }

  const bool secret_above = (g > pivot && r == HOTTER) || (g < pivot && r == COLDER);
  if (secret_above) {
    return solve_middle(g, pivot + 1, hi);
  }
  return solve_middle(g, lo, pivot - 1);
}

// wall_len[k] = largest P such that a wall interval of length P is solvable
// using k further guesses after the last guess sat on the far endpoint.
int wall_len[32];
bool wall_table_built = false;

void build_wall_table() {
  if (wall_table_built) {
    return;
  }
  wall_table_built = true;
  wall_len[0] = 1;
  wall_len[1] = 3;
  wall_len[2] = 7;
  for (int i = 3; i < 30; ++i) {
    wall_len[i] = wall_len[i - 2] + (1 << i);
  }
}

int wall_level(int n) {
  int z = 0;
  while (wall_len[z] < n) {
    ++z;
  }
  return z;
}

Result ask(int g) {
  return static_cast<Result>(Guess(g));
}

// Secret in local range [1 .. n] against wall at global `wall_end` (1 or N).
int solve_wall(int wall_end, int n) {
  const auto G = [&](int local) { return to_global(wall_end, local); };

  if (n == 2) {
    if (ask(G(1)) == HOTTER) {
      return G(1);
    }
    return G(2);
  }

  if (n == 3) {
    const Result r = ask(G(1));
    if (r == HOTTER) {
      return G(1);
    }
    if (r == SAME) {
      return G(2);
    }
    return G(3);
  }

  if (n == 4 || n == 5) {
    Result r = ask(G(3));
    if (r == COLDER) {
      return G(n);
    }
    if (r == SAME) {
      return G(4);
    }
    r = ask(G(1));
    if (r == HOTTER) {
      return G(1);
    }
    if (r == SAME) {
      return G(2);
    }
    return G(3);
  }

  if (n == 6) {
    Result r = ask(G(1));
    if (r == HOTTER) {
      r = ask(G(3));
      if (r == HOTTER) {
        return G(3);
      }
      if (r == SAME) {
        return G(2);
      }
      return G(1);
    }
    r = ask(G(9));
    if (r == HOTTER) {
      return G(6);
    }
    if (r == SAME) {
      return G(5);
    }
    return G(4);
  }

  if (n == 7) {
    Result r = ask(G(1));
    if (r == SAME) {
      return G(4);
    }
    if (r == HOTTER) {
      r = ask(G(3));
      if (r == HOTTER) {
        return G(3);
      }
      if (r == SAME) {
        return G(2);
      }
      return G(1);
    }
    r = ask(G(11));
    if (r == HOTTER) {
      return G(7);
    }
    if (r == SAME) {
      return G(6);
    }
    return G(5);
  }

  const int z = wall_level(n);
  const int small = wall_len[z - 2];
  const int probe = small - 2;
  const int mid = (probe + n) / 2;

  Result r = ask(G(probe));
  if (r == SAME) {
    return G(mid);
  }
  if (r == COLDER) {
    return solve_middle(G(probe), G(mid + 1), G(n));
  }

  r = ask(G(small));
  if (r == COLDER) {
    return solve_wall(wall_end, small);
  }
  if (r == SAME) {
    return G(small - 1);
  }
  return solve_middle(G(small), G(small), G((probe + n - 1) / 2));
}

} // namespace

int HC(int N) {
  build_wall_table();

  if (N == 1) {
    return 1;
  }

  if (N == 2) {
    Guess(1);
    return ask(2) == HOTTER ? 2 : 1;
  }

  if (N == 3) {
    Guess(1);
    const Result r = ask(3);
    if (r == HOTTER) {
      return 3;
    }
    if (r == COLDER) {
      return 1;
    }
    return 2;
  }

  const int anchor = (N + 2) / 2;
  Guess(anchor - 2);
  const Result r = ask(anchor);

  if (r == SAME) {
    return anchor - 1;
  }
  if (r == COLDER) {
    return solve_wall(1, anchor);
  }
  return solve_wall(N, N - anchor + 1);
}
