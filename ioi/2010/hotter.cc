#include "grader.h"

static int t[50];

static int imin(int a, int b) {
  return a < b ? a : b;
}
static int imax(int a, int b) {
  return a > b ? a : b;
}

static int fix(int end, int x) {
  if (end == 1) {
    return x;
  }
  return end - x + 1;
}

static int midgame(int p, int a, int b) {
  if (a == b) {
    return a;
  }
  if (a > b) {
    const int tmp = a;
    a = b;
    b = tmp;
  }

  int sz = 3;
  for (; b - a + 1 > sz; sz = 2 * sz + 1) {
  }

  int mid = -999;
  if (p < b - sz + 1) {
    mid = b - sz / 2;
  } else if (p > a + sz - 1) {
    mid = a + sz / 2;
  } else if (p <= a) {
    mid = p + sz / 2;
  } else if (p >= b) {
    mid = p - sz / 2;
  }

  const int q = mid + (mid - p);
  const int g = Guess(q);
  if (g == 0) {
    return mid;
  }
  if ((q > mid && g > 0) || (q < mid && g < 0)) {
    return midgame(q, imin(mid + 1, b), b);
  }
  return midgame(q, a, imax(mid - 1, a));
}

static int endgame(int end, int n) {
  int z = 0;
  for (; t[z] < n; ++z) {
  }

  if (n == 2) {
    const int g = Guess(fix(end, 1));
    if (g > 0) {
      return fix(end, 1);
    }
    return fix(end, 2);
  }
  if (n == 3) {
    const int g = Guess(fix(end, 1));
    if (g > 0) {
      return fix(end, 1);
    }
    if (g == 0) {
      return fix(end, 2);
    }
    return fix(end, 3);
  }
  if (n == 4 || n == 5) {
    int g = Guess(fix(end, 3));
    if (g < 0) {
      return fix(end, n);
    }
    if (g == 0) {
      return fix(end, 4);
    }
    g = Guess(fix(end, 1));
    if (g > 0) {
      return fix(end, 1);
    }
    if (g == 0) {
      return fix(end, 2);
    }
    return fix(end, 3);
  }
  if (n == 6) {
    int g = Guess(fix(end, 1));
    if (g > 0) {
      g = Guess(fix(end, 3));
      if (g > 0) {
        return fix(end, 3);
      }
      if (g == 0) {
        return fix(end, 2);
      }
      return fix(end, 1);
    }
    g = Guess(fix(end, 9));
    if (g > 0) {
      return fix(end, 6);
    }
    if (g == 0) {
      return fix(end, 5);
    }
    return fix(end, 4);
  }
  if (n == 7) {
    int g = Guess(fix(end, 1));
    if (g == 0) {
      return fix(end, 4);
    }
    if (g > 0) {
      g = Guess(fix(end, 3));
      if (g > 0) {
        return fix(end, 3);
      }
      if (g == 0) {
        return fix(end, 2);
      }
      return fix(end, 1);
    }
    g = Guess(fix(end, 11));
    if (g > 0) {
      return fix(end, 7);
    }
    if (g == 0) {
      return fix(end, 6);
    }
    return fix(end, 5);
  }

  int g = Guess(fix(end, t[z - 2] - 2));
  if (g == 0) {
    return fix(end, (t[z - 2] - 2 + n) / 2);
  }
  if (g < 0) {
    return midgame(fix(end, t[z - 2] - 2), fix(end, (t[z - 2] - 2 + n) / 2 + 1), fix(end, n));
  }

  g = Guess(fix(end, t[z - 2]));
  if (g < 0) {
    return endgame(end, t[z - 2]);
  }
  if (g == 0) {
    return fix(end, t[z - 2] - 1);
  }
  return midgame(fix(end, t[z - 2]), fix(end, t[z - 2]), fix(end, (t[z - 2] - 2 + n - 1) / 2));
}

int HC(int N) {
  if (!t[0]) {
    t[0] = 1;
    t[1] = 3;
    t[2] = 7;
    for (int i = 3; i < 30; ++i) {
      t[i] = t[i - 2] + (1 << i);
    }
  }

  if (N == 1) {
    return 1;
  }
  if (N == 2) {
    Guess(1);
    const int r = Guess(2);
    return r > 0 ? 2 : 1;
  }
  if (N == 3) {
    Guess(1);
    const int r = Guess(3);
    if (r > 0) {
      return 3;
    }
    if (r < 0) {
      return 1;
    }
    return 2;
  }

  const int mid = (N + 2) / 2;
  Guess(mid - 2);
  const int r = Guess(mid);
  if (r == 0) {
    return mid - 1;
  }
  if (r < 0) {
    return endgame(1, mid);
  }
  return endgame(N, N - mid + 1);
}
