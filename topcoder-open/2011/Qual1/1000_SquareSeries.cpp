#include <bits/stdc++.h>
using namespace std;

class SquareSeries {
  bool append(char ch, int& len, char& last) {
    if (len == 0) {
      len = 1;
      last = ch;
      return true;
    }
    if (ch == last) {
      if (len == 1)
        return false;
      len--;
    } else {
      len++;
      last = ch;
    }
    return true;
  }

  bool buildRight(const string& s, int len, char last, const string& right, int lastLength,
                  string& out) {
    int l = len;
    char c = last;
    for (char ch : right) {
      if (!append(ch, l, c))
        return false;
    }
    if (l != lastLength)
      return false;
    out = s + right;
    return true;
  }

public:
  string completeIt(string pattern, int lastLength) {
    int q = pattern.find('?');
    string left = pattern.substr(0, q);
    string right = pattern.substr(q + 1);

    string cur;
    int len = 0;
    char last = 0;
    for (char ch : left) {
      if (!append(ch, len, last))
        return "...";
      cur.push_back(ch);
    }

    string ans = "...";
    queue<string> qu;
    qu.push(cur);

    while (!qu.empty()) {
      int sz = qu.size();
      vector<string> layer(sz);
      for (int it = 0; it < sz; it++) {
        layer[it] = qu.front();
        qu.pop();
      }
      bool found = false;
      for (const string& s : layer) {
        int l = 0;
        char c = 0;
        for (char ch : s) {
          if (!append(ch, l, c))
            break;
        }
        string cand;
        if (buildRight(s, l, c, right, lastLength, cand)) {
          if (ans == "..." || cand.size() < ans.size() || (cand.size() == ans.size() && cand < ans))
            ans = cand;
          found = true;
        }
      }
      if (found)
        break;

      for (const string& s : layer) {
        for (char ch : {'B', 'W'}) {
          int l = 0;
          char c = 0;
          bool ok = true;
          for (char x : s) {
            if (!append(x, l, c)) {
              ok = false;
              break;
            }
          }
          if (!ok)
            continue;
          if (!append(ch, l, c))
            continue;
          qu.push(s + ch);
        }
      }
    }
    return ans;
  }
};
