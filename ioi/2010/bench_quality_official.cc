#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dirent.h>
#include <string>
#include <vector>

#include "quality_sweep.cc"

static bool read_test(const char* path, int& r, int& c, int& h, int& w, int q[3001][3001]) {
  FILE* f = fopen(path, "r");
  if (!f) {
    return false;
  }
  if (fscanf(f, "%d %d %d %d", &r, &c, &h, &w) != 4) {
    fclose(f);
    return false;
  }
  for (int i = 0; i < r; ++i) {
    for (int j = 0; j < c; ++j) {
      if (fscanf(f, "%d", &q[i][j]) != 1) {
        fclose(f);
        return false;
      }
    }
  }
  fclose(f);
  return true;
}

static bool read_expect(const char* path, int& expect) {
  FILE* f = fopen(path, "r");
  if (!f) {
    return false;
  }
  const bool ok = fscanf(f, "%d", &expect) == 1;
  fclose(f);
  return ok;
}

static std::vector<std::string> list_inputs(const char* dir) {
  std::vector<std::string> out;
  DIR* d = opendir(dir);
  if (!d) {
    return out;
  }
  for (dirent* ent; (ent = readdir(d)) != nullptr;) {
    if (strncmp(ent->d_name, "grader.in.", 10) != 0) {
      continue;
    }
    out.push_back(std::string(dir) + "/" + ent->d_name);
  }
  closedir(d);
  return out;
}

static int subtask_num(const std::string& path) {
  const size_t dash = path.rfind('-');
  return dash == std::string::npos ? 0 : atoi(path.c_str() + dash + 1);
}

int main() {
  const char* root = "quality/appeal";
  std::vector<std::string> inputs;
  for (int st = 1; st <= 5; ++st) {
    char dir[256];
    snprintf(dir, sizeof(dir), "%s/Subtask%d-data", root, st);
    const auto files = list_inputs(dir);
    inputs.insert(inputs.end(), files.begin(), files.end());
  }

  static int q[3001][3001];
  long long total_ms = 0;
  int ok = 0, fail = 0;

  printf("%-40s %8s %10s %8s\n", "test", "R", "C", "ms");
  printf("%-40s %8s %10s %8s\n", "----", "-", "-", "--");

  for (const std::string& in_path : inputs) {
    int r, c, h, w;
    if (!read_test(in_path.c_str(), r, c, h, w, q)) {
      fprintf(stderr, "read failed: %s\n", in_path.c_str());
      return 1;
    }

    std::string exp_path = in_path;
    exp_path.replace(exp_path.find("grader.in."), 10, "grader.expect.");

    int expect = 0;
    if (!read_expect(exp_path.c_str(), expect)) {
      fprintf(stderr, "expect read failed: %s\n", exp_path.c_str());
      return 1;
    }

    const auto t0 = std::chrono::steady_clock::now();
    const int got = rectangle(r, c, h, w, q);
    const auto t1 = std::chrono::steady_clock::now();
    const long long ms = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    total_ms += ms;

    const bool pass = got == expect;
    ok += pass;
    fail += !pass;

    const char* name = strrchr(in_path.c_str(), '/');
    name = name ? name + 1 : in_path.c_str();
    printf("%-40s %8d %8d %8lld%s\n", name, r, c, ms, pass ? "" : "  WRONG");

    if (!pass) {
      printf("  expected %d, got %d (H=%d W=%d subtask %d)\n", expect, got, h, w,
             subtask_num(in_path));
    }
  }

  printf("\n%d/%d passed, total %lld ms (%.2f s)\n", ok, ok + fail, total_ms, total_ms / 1000.0);
  return fail ? 1 : 0;
}
