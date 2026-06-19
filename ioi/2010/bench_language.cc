#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

int correct = 0, total = 0;
int actual_lang = 0;

int language(int guess) {
  ++total;
  if (guess == actual_lang) {
    ++correct;
  }
  return actual_lang;
}

#include "language.cc"

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  FILE* f = fopen("language/appeal/Subtask1-data/grader.in.1", "r");
  if (!f) {
    fprintf(stderr, "missing language/appeal/Subtask1-data/grader.in.1\n");
    return 1;
  }

  unordered_map<string, int> code_id;
  int next_id = 0;

  char code[8];
  int e[100];
  const auto t0 = chrono::steady_clock::now();
  while (fscanf(f, "%2s", code) == 1) {
    for (int i = 0; i < 100; ++i) {
      fscanf(f, "%d", &e[i]);
    }
    char line[8192];
    fgets(line, sizeof line, f);

    const string key(code);
    if (!code_id.count(key)) {
      code_id[key] = next_id++;
    }
    actual_lang = code_id[key];
    excerpt(e);
  }
  fclose(f);
  const auto t1 = chrono::steady_clock::now();
  const i64 ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();

  printf("%d/%d correct (%.3f accuracy), %lld ms\n", correct, total,
         total ? 1.0 * correct / total : 0.0, ms);
  return 0;
}
