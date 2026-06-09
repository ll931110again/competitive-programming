#include "bits/stdc++.h"
#include <bits/stdc++.h>
using namespace std;

namespace {

struct Machine {
  long long day_available;
  long long buy_price;
  long long resale_price;
  long long daily_profit;
};

struct Line {
  long long slope = 0;
  long long intercept = -(1LL << 60);
  Line() = default;
  Line(long long slope_, long long intercept_) : slope(slope_), intercept(intercept_) {}
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
    Node* left = nullptr;
    Node* right = nullptr;
    explicit Node(Line line_) : line(line_) {}
  };

  long long min_x, max_x;
  Node* root = nullptr;

  LiChaoMax(long long minX_, long long maxX_) : min_x(minX_), max_x(maxX_) {}

  void add_line(Line new_line) {
    add_line_rec(root, min_x, max_x, new_line);
  }

  void add_line_rec(Node*& node, long long seg_left, long long seg_right, Line new_line) {
    if (!node) {
      node = new Node(new_line);
      return;
    }

    long long mid = seg_left + ((seg_right - seg_left) >> 1);
    bool better_at_left = new_line.value(seg_left) > node->line.value(seg_left);
    bool better_at_mid = new_line.value(mid) > node->line.value(mid);
    if (better_at_mid)
      swap(new_line, node->line);

    if (seg_left == seg_right)
      return;
    if (better_at_left != better_at_mid)
      add_line_rec(node->left, seg_left, mid, new_line);
    else
      add_line_rec(node->right, mid + 1, seg_right, new_line);
  }

  long long query(long long x) const {
    return query_rec(root, min_x, max_x, x);
  }

  long long query_rec(Node* node, long long seg_left, long long seg_right, long long x) const {
    if (!node)
      return LLONG_MIN;
    long long best = node->line.value(x);
    if (seg_left == seg_right)
      return best;
    long long mid = seg_left + ((seg_right - seg_left) >> 1);
    if (x <= mid)
      return max(best, query_rec(node->left, seg_left, mid, x));
    return max(best, query_rec(node->right, mid + 1, seg_right, x));
  }
};

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int case_no = 1;

  int machine_count;
  long long initial_cash, last_day;
  while (cin >> machine_count >> initial_cash >> last_day) {
    if (machine_count == 0 && initial_cash == 0 && last_day == 0)
      break;

    vector<Machine> machines;
    machines.reserve(machine_count);
    for (int i = 0; i < machine_count; i++) {
      Machine machine;
      cin >> machine.day_available >> machine.buy_price >> machine.resale_price >>
          machine.daily_profit;
      machines.push_back(machine);
    }
    sort(machines.begin(), machines.end(), [](const Machine& a, const Machine& b) {
      if (a.day_available != b.day_available)
        return a.day_available < b.day_available;
      return a.buy_price < b.buy_price;
    });

    LiChaoMax cash_at_day(1, last_day + 1);
    cash_at_day.add_line(Line(0, initial_cash)); // keep cash, no machine

    for (int i = 0; i < machine_count;) {
      long long day = machines[i].day_available;
      long long cash_today = cash_at_day.query(day);

      vector<Line> to_add;
      for (; i < machine_count && machines[i].day_available == day; i++) {
        const auto& machine = machines[i];
        if (cash_today < machine.buy_price)
          continue;
        // If we buy this machine on 'day' using 'money' cash, then selling on
        // day x yields: money - p + r + g*(x - day - 1) = g*x + (money - p + r
        // - g*(day+1))
        long long intercept = cash_today - machine.buy_price + machine.resale_price -
                              machine.daily_profit * (day + 1);
        to_add.push_back(Line(machine.daily_profit, intercept));
      }

      for (const auto& line : to_add)
        cash_at_day.add_line(line);
    }

    long long max_cash_end = cash_at_day.query(last_day + 1);
    cout << "Case " << case_no++ << ": " << max_cash_end << "\n";
  }

  return 0;
}
