/*
 * Solution for problem A: Alignment of Code.
 *
 */

#ifdef ONLINE_JUDGE
  #include <bits/stdc++.h>
#endif
#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <deque>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <string>
#include <utility>
#include <unordered_set>
#include <vector>
#define maxn 205
using namespace std;

vector<vector<string>> texts;
char s[maxn];

vector<string> split() {
  vector<string> tokens;
  string current_token = "";

  for (int i = 0; i < strlen(s); i++) {
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
  int m = texts.size();
  vector<string> output(m);

  for (int j = 0; ; j++) {
    int max_alignment = 0;
    for (int i = 0; i < m; i++) if (texts[i].size() > j) {
      max_alignment = max(max_alignment, texts[i][j].size());
    }

    if (max_alignment == 0) {
      break;
    }

    for (int i = 0; i < m; i++) if (texts[i].size() > j) {
      if (j > 0) {
        output[i] += ' ';
      }
      output[i] += texts[i][j];
      for (int k = texts[i][j].size(); k < max_alignment; k++) {
        output[i] += ' ';
      }
    }
  }

  return output;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  freopen("alignment.in", "r", stdin);
  freopen("alignment.out", "w", stdout);

  while (gets(s)) {
    texts.push_back(split());
  }

  auto output = align(texts);
  for (auto row : output) {
    cout << row << endl;
  }

  return 0;
}
