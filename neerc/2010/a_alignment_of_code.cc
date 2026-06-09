/*
 * Solution for problem A: Alignment of Code.
 *
 */

#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_max_n = 205;
vector<vector<string>> texts;
char s[k_max_n];

vector<string> split() {
  vector<string> tokens;
  string current_token = "";

  for (size_t i = 0; s[i] != '\0'; ++i) {
    if (s[i] == ' ') {
      if (!current_token.empty()) {
        tokens.push_back(current_token);
        current_token.clear();
      }
    } else {
      current_token += s[i];
    }
  }

  if (!current_token.empty()) {
    tokens.push_back(current_token);
  }

  return tokens;
}

vector<string> align(vector<vector<string>> texts) {
  const size_t m = texts.size();
  vector<string> output(m);

  for (size_t j = 0;; ++j) {
    size_t max_alignment = 0;
    for (size_t i = 0; i < m; ++i) {
      if (texts[i].size() > j) {
        max_alignment = max(max_alignment, texts[i][j].size());
      }
    }

    if (max_alignment == 0) {
      break;
    }

    for (size_t i = 0; i < m; ++i) {
      if (texts[i].size() > j) {
        if (j > 0) {
          output[i] += ' ';
        }
        output[i] += texts[i][j];
        // No trailing spaces on a line: only pad column j when this line has word j+1.
        if (j + 1 < texts[i].size()) {
          for (size_t k = texts[i][j].size(); k < max_alignment; ++k) {
            output[i] += ' ';
          }
        }
      }
    }
  }

  return output;
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("alignment.in", "r", stdin);
  freopen("alignment.out", "w", stdout);

  while (fgets(s, k_max_n, stdin)) {
    size_t L = strlen(s);
    while (L > 0 && (s[L - 1] == '\n' || s[L - 1] == '\r')) {
      s[--L] = '\0';
    }
    texts.push_back(split());
  }

  auto output = align(texts);
  for (auto row : output) {
    cout << row << endl;
  }

  return 0;
}
