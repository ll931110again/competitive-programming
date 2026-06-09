#include "traffic.cc"
#include <dirent.h>
#include <bits/stdc++.h>
using namespace std;

namespace {

bool read_traffic(const char* path, int& n, int p[], int s[], int d[], int& expect) {
  FILE* f = fopen(path, "r");
  if (!f) {
    return false;
  }
  if (fscanf(f, "%d", &n) != 1) {
    fclose(f);
    return false;
  }
  for (int i = 0; i < n; ++i) {
    if (fscanf(f, "%d", &p[i]) != 1) {
      fclose(f);
      return false;
    }
  }
  for (int i = 0; i < n - 1; ++i) {
    if (fscanf(f, "%d %d", &s[i], &d[i]) != 2) {
      fclose(f);
      return false;
    }
  }
  fclose(f);

  string exp_path = path;
  const size_t pos = exp_path.find("grader.in.");
  if (pos == string::npos) {
    return false;
  }
  exp_path.replace(pos, 10, "grader.expect.");
  f = fopen(exp_path.c_str(), "r");
  if (!f) {
    return false;
  }
  const bool ok = fscanf(f, "%d", &expect) == 1;
  fclose(f);
  return ok;
}

vector<string> list_inputs(const char* dir) {
  vector<string> out;
  DIR* dp = opendir(dir);
  if (!dp) {
    return out;
  }
  for (dirent* ent; (ent = readdir(dp)) != nullptr;) {
    if (strncmp(ent->d_name, "grader.in.", 10) == 0) {
      out.push_back(string(dir) + "/" + ent->d_name);
    }
  }
  closedir(dp);
  return out;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int p[1000005], s[1000005], d[1000005];
  long long total_ms = 0;
  int ok = 0, fail = 0;

  printf("%-24s %8s %8s\n", "test", "n", "ms");
  for (int st = 1; st <= 4; ++st) {
    char dir[256];
    snprintf(dir, sizeof dir, "traffic/appeal/Subtask%d-data", st);
    for (const string& path : list_inputs(dir)) {
      int n = 0, expect = 0;
      if (!read_traffic(path.c_str(), n, p, s, d, expect)) {
        fprintf(stderr, "read fail %s\n", path.c_str());
        return 1;
      }
      const auto t0 = chrono::steady_clock::now();
      const int got = LocateCentre(n, p, s, d);
      const auto t1 = chrono::steady_clock::now();
      const long long ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
      total_ms += ms;

      const char* name = strrchr(path.c_str(), '/');
      name = name ? name + 1 : path.c_str();
      const bool pass = got == expect;
      ok += pass;
      fail += !pass;
      printf("%-24s %8d %8lld%s\n", name, n, ms, pass ? "" : " WRONG");
      if (!pass) {
        printf("  expected %d got %d\n", expect, got);
      }
    }
  }
  printf("\n%d/%d passed, total %lld ms\n", ok, ok + fail, total_ms);
  return fail ? 1 : 0;
}
