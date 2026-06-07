#include "grader.h"

#include <cstring>

static bool seen[56][65536];
static bool have_lang[56];
static int bigram[56][1 << 18];

static int big_index(int a, int b) {
  return (a * 65537 + b) & ((1 << 18) - 1);
}

void excerpt(int e[100]) {
  int best = 0;
  int best_score = -1;

  for (int lang = 0; lang < 56; ++lang) {
    if (!have_lang[lang]) {
      continue;
    }

    int score = 0;
    bool local[65536];
    memset(local, 0, sizeof local);

    for (int i = 0; i < 100; ++i) {
      const int sym = e[i];
      if (!local[sym] && seen[lang][sym]) {
        ++score;
      }
      local[sym] = true;
    }

    for (int i = 1; i < 100; ++i) {
      score += bigram[lang][big_index(e[i - 1], e[i])];
    }

    if (score > best_score) {
      best_score = score;
      best = lang;
    }
  }

  const int actual = language(best);
  have_lang[actual] = true;

  for (int i = 0; i < 100; ++i) {
    seen[actual][e[i]] = true;
  }
  for (int i = 1; i < 100; ++i) {
    ++bigram[actual][big_index(e[i - 1], e[i])];
  }
}
