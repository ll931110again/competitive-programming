#include "quality_sweep.cc"
#include <dirent.h>
#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

bool read_test(const char* path, int& r, int& c, int& h, int& w, int q[3001][3001]) {
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

bool read_expect(const char* path, int& expect) {
  FILE* f = fopen(path, "r");
  if (!f) {
    return false;
  }
  const bool ok = fscanf(f, "%d", &expect) == 1;
  fclose(f);
  return ok;
}

vector<string> list_inputs(const char* dir) {
  vector<string> out;
  DIR* d = opendir(dir);
  if (!d) {
    return out;
  }
  for (dirent* ent; (ent = readdir(d)) != nullptr;) {
    if (strncmp(ent->d_name, "grader.in.", 10) != 0) {
      continue;
    }
    out.push_back(string(dir) + "/" + ent->d_name);
  }
  closedir(d);
  return out;
}

int subtask_num(const string& path) {
  const size_t dash = path.rfind('-');
  return dash == string::npos ? 0 : atoi(path.c_str() + dash + 1);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  const char* root = "quality/appeal";
  vector<string> inputs;
  for (int st = 1; st <= 5; ++st) {
    char dir[256];
    snprintf(dir, sizeof(dir), "%s/Subtask%d-data", root, st);
    const auto files = list_inputs(dir);
    inputs.insert(inputs.end(), files.begin(), files.end());
  }

  int q[3001][3001];
  i64 total_ms = 0;
  int ok = 0, fail = 0;

  printf("%-40s %8s %10s %8s\n", "test", "R", "C", "ms");
  printf("%-40s %8s %10s %8s\n", "----", "-", "-", "--");

  for (const string& in_path : inputs) {
    int r, c, h, w;
    if (!read_test(in_path.c_str(), r, c, h, w, q)) {
      fprintf(stderr, "read failed: %s\n", in_path.c_str());
      return 1;
    }

    string exp_path = in_path;
    exp_path.replace(exp_path.find("grader.in."), 10, "grader.expect.");

    int expect = 0;
    if (!read_expect(exp_path.c_str(), expect)) {
      fprintf(stderr, "expect read failed: %s\n", exp_path.c_str());
      return 1;
    }

    const auto t0 = chrono::steady_clock::now();
    const int got = rectangle(r, c, h, w, q);
    const auto t1 = chrono::steady_clock::now();
    const i64 ms = chrono::duration_cast<chrono::milliseconds>(t1 - t0).count();
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
