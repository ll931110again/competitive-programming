#include "grader.h"

void Solve() {
  int t[] = {0, 6, 10, 6};
  int r;
  do {
    r = Theory(t[1], t[2], t[3]);
    if (r != 0) {
      --t[r];
    }
  } while (r != 0);
}
