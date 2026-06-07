#include <chrono>
#include <cstdio>
#include <dirent.h>
#include <string>
#include <vector>

#include "traffic.cc"

static bool read_traffic(const char* path, int& n, int p[], int s[], int d[], int& expect) {
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

  std::string exp_path = path;
  const size_t pos = exp_path.find("grader.in.");
  if (pos == std::string::npos) {
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

static std::vector<std::string> list_inputs(const char* dir) {
  std::vector<std::string> out;
  DIR* dp = opendir(dir);
  if (!dp) {
    return out;
  }
  for (dirent* ent; (ent = readdir(dp)) != nullptr;) {
    if (strncmp(ent->d_name, "grader.in.", 10) == 0) {
      out.push_back(std::string(dir) + "/" + ent->d_name);
    }
  }
  closedir(dp);
  return out;
}

int main() {
  static int p[1000005], s[1000005], d[1000005];
  long long total_ms = 0;
  int ok = 0, fail = 0;

  printf("%-24s %8s %8s\n", "test", "n", "ms");
  for (int st = 1; st <= 4; ++st) {
    char dir[256];
    snprintf(dir, sizeof dir, "traffic/appeal/Subtask%d-data", st);
    for (const std::string& path : list_inputs(dir)) {
      int n = 0, expect = 0;
      if (!read_traffic(path.c_str(), n, p, s, d, expect)) {
        fprintf(stderr, "read fail %s\n", path.c_str());
        return 1;
      }
      const auto t0 = std::chrono::steady_clock::now();
      const int got = LocateCentre(n, p, s, d);
      const auto t1 = std::chrono::steady_clock::now();
      const long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
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
