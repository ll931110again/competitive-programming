#include "bits/stdc++.h"
using namespace std;

struct Machine {
  long long dayAvailable;
  long long buyPrice;
  long long resalePrice;
  long long dailyProfit;
};

struct Line {
  long long slope = 0;
  long long intercept = -(1LL << 60);
  Line() = default;
  Line(long long slope_, long long intercept_)
      : slope(slope_), intercept(intercept_) {}
  inline long long value(long long x) const {
    __int128 v = (__int128)slope * (__int128)x + (__int128)intercept;
    if (v < (__int128)LLONG_MIN)
      return LLONG_MIN;
    if (v > (__int128)LLONG_MAX)
      return LLONG_MAX;
    return (long long)v;
  }
};

struct LiChaoMax {
  struct Node {
    Line line;
    Node *left = nullptr;
    Node *right = nullptr;
    explicit Node(Line line_) : line(line_) {}
  };

  long long minX, maxX;
  Node *root = nullptr;

  LiChaoMax(long long minX_, long long maxX_) : minX(minX_), maxX(maxX_) {}

  void addLine(Line newLine) { addLineRec(root, minX, maxX, newLine); }

  void addLineRec(Node *&node, long long segLeft, long long segRight,
                  Line newLine) {
    if (!node) {
      node = new Node(newLine);
      return;
    }

    long long mid = segLeft + ((segRight - segLeft) >> 1);
    bool betterAtLeft = newLine.value(segLeft) > node->line.value(segLeft);
    bool betterAtMid = newLine.value(mid) > node->line.value(mid);
    if (betterAtMid)
      swap(newLine, node->line);

    if (segLeft == segRight)
      return;
    if (betterAtLeft != betterAtMid)
      addLineRec(node->left, segLeft, mid, newLine);
    else
      addLineRec(node->right, mid + 1, segRight, newLine);
  }

  long long query(long long x) const { return queryRec(root, minX, maxX, x); }

  long long queryRec(Node *node, long long segLeft, long long segRight,
                     long long x) const {
    if (!node)
      return LLONG_MIN;
    long long best = node->line.value(x);
    if (segLeft == segRight)
      return best;
    long long mid = segLeft + ((segRight - segLeft) >> 1);
    if (x <= mid)
      return max(best, queryRec(node->left, segLeft, mid, x));
    return max(best, queryRec(node->right, mid + 1, segRight, x));
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int caseNo = 1;

  int machineCount;
  long long initialCash, lastDay;
  while (cin >> machineCount >> initialCash >> lastDay) {
    if (machineCount == 0 && initialCash == 0 && lastDay == 0)
      break;

    vector<Machine> machines;
    machines.reserve(machineCount);
    for (int i = 0; i < machineCount; i++) {
      Machine machine;
      cin >> machine.dayAvailable >> machine.buyPrice >> machine.resalePrice >>
          machine.dailyProfit;
      machines.push_back(machine);
    }
    sort(machines.begin(), machines.end(),
         [](const Machine &a, const Machine &b) {
           if (a.dayAvailable != b.dayAvailable)
             return a.dayAvailable < b.dayAvailable;
           return a.buyPrice < b.buyPrice;
         });

    LiChaoMax cashAtDay(1, lastDay + 1);
    cashAtDay.addLine(Line(0, initialCash)); // keep cash, no machine

    for (int i = 0; i < machineCount;) {
      long long day = machines[i].dayAvailable;
      long long cashToday = cashAtDay.query(day);

      vector<Line> toAdd;
      for (; i < machineCount && machines[i].dayAvailable == day; i++) {
        const auto &machine = machines[i];
        if (cashToday < machine.buyPrice)
          continue;
        // If we buy this machine on 'day' using 'money' cash, then selling on
        // day x yields: money - p + r + g*(x - day - 1) = g*x + (money - p + r
        // - g*(day+1))
        long long intercept = cashToday - machine.buyPrice +
                              machine.resalePrice -
                              machine.dailyProfit * (day + 1);
        toAdd.push_back(Line(machine.dailyProfit, intercept));
      }

      for (const auto &line : toAdd)
        cashAtDay.addLine(line);
    }

    long long maxCashEnd = cashAtDay.query(lastDay + 1);
    cout << "Case " << caseNo++ << ": " << maxCashEnd << "\n";
  }

  return 0;
}
