#include <bits/stdc++.h>
using namespace std;

namespace {

int secret;
int prev;
int guesses;

int hotter_colder(int j, int p, int g) {
  if (abs(g - j) < abs(p - j)) {
    return 1;
  }
  if (abs(g - j) > abs(p - j)) {
    return -1;
  }
  return 0;
}

int Guess(int g) {
  ++guesses;
  if (prev == 0) {
    prev = g;
    return 0;
  }
  const int r = hotter_colder(secret, prev, g);
  prev = g;
  return r;
}

#include "hotter.cc"

int max_guesses(int n, int st) {
  if (st == 1) {
    return 500;
  }
  if (st == 2) {
    return 18;
  }
  if (st == 3) {
    return 16;
  }
  int w = 0;
  while ((1 << w) <= 3 * n) {
    ++w;
  }
  return w - 1;
}

} // namespace

int main(int argc, char** argv) {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  const char* path = (argc > 1) ? argv[1] : "hottercolder/appeal/Subtask1-data/grader.in.1";
  const int max_n = (argc > 2) ? atoi(argv[2]) : 500;

  FILE* f = fopen(path, "r");
  if (!f) {
    return 1;
  }

  int n, j;
  int total = 0;
  int wrong = 0;
  int worst = 0;
  int st_ok[5] = {0, 0, 0, 0, 0};

  while (fscanf(f, "%d %d", &n, &j) == 2) {
    if (n > max_n) {
      continue;
    }
    ++total;
    secret = j;
    prev = 0;
    guesses = 0;
    const int got = HC(n);
    if (got != j) {
      ++wrong;
      if (wrong <= 5) {
        printf("WRONG n=%d secret=%d got=%d guesses=%d\n", n, j, got, guesses);
      }
    } else {
      for (int st = 1; st <= 4; ++st) {
        if (guesses <= max_guesses(n, st)) {
          ++st_ok[st];
        }
      }
    }
    if (guesses > worst) {
      worst = guesses;
    }
  }
  fclose(f);

  printf("tested %d wrong %d worst_guesses=%d (n<=%d)\n", total, wrong, worst, max_n);
  for (int st = 1; st <= 4; ++st) {
    printf("  subtask %d: %d/%d within limit\n", st, st_ok[st], total - wrong);
  }
  return wrong ? 1 : 0;
}
