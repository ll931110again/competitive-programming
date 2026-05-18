#include <bits/stdc++.h>
using namespace std;

class YetAnotherBingoProblem {
  vector<set<int>> parseCards(const vector<string>& cards) {
    vector<set<int>> res;
    string all;
    for (const string& p : cards)
      all += p;
    string cur;
    for (size_t i = 0; i <= all.size(); i++) {
      if (i == all.size() || all[i] == ',') {
        if (!cur.empty()) {
          set<int> card;
          stringstream ss(cur);
          int x;
          while (ss >> x)
            card.insert(x);
          res.push_back(card);
          cur.clear();
        }
      } else {
        cur.push_back(all[i]);
      }
    }
    return res;
  }

  bool canWin(const vector<set<int>>& mine, const vector<set<int>>& his, vector<int> order) {
    vector<set<int>> mc = mine, hc = his;
    for (int num : order) {
      for (auto& c : mc)
        c.erase(num);
      for (auto& c : hc)
        c.erase(num);
      bool mFull = false, hFull = false;
      for (const auto& c : mc)
        if (c.empty())
          mFull = true;
      for (const auto& c : hc)
        if (c.empty())
          hFull = true;
      if (mFull && hFull)
        return false;
      if (hFull)
        return false;
      if (mFull)
        return true;
    }
    return false;
  }

public:
  int longestWinningSequence(vector<string> cards1, vector<string> cards2) {
    vector<set<int>> mine = parseCards(cards1);
    vector<set<int>> his = parseCards(cards2);
    set<int> allNums;
    for (const auto& c : mine)
      for (int x : c)
        allNums.insert(x);
    for (const auto& c : his)
      for (int x : c)
        allNums.insert(x);

    vector<int> nums(allNums.begin(), allNums.end());
    int best = -1;

    function<void(int, vector<int>&)> dfs = [&](int pos, vector<int>& ord) {
      if (pos == (int)nums.size()) {
        if (canWin(mine, his, ord))
          best = max(best, (int)ord.size());
        return;
      }
      for (int i = pos; i < (int)nums.size(); i++) {
        swap(nums[pos], nums[i]);
        ord.push_back(nums[pos]);
        dfs(pos + 1, ord);
        ord.pop_back();
        swap(nums[pos], nums[i]);
      }
    };

    if ((int)nums.size() <= 10) {
      vector<int> ord;
      dfs(0, ord);
      return best;
    }

    vector<int> needM, needH, other;
    set<int> mNeed, hNeed;
    for (const auto& c : mine) {
      int last = -1;
      for (int x : c)
        if (last < 0 || x > last)
          last = x;
      mNeed.insert(last);
    }
    for (const auto& c : his) {
      int last = -1;
      for (int x : c)
        if (last < 0 || x > last)
          last = x;
      hNeed.insert(last);
    }
    for (int x : nums) {
      bool im = mNeed.count(x), ih = hNeed.count(x);
      if (im && !ih)
        needM.push_back(x);
      else if (ih && !im)
        needH.push_back(x);
      else
        other.push_back(x);
    }

    vector<int> order;
    for (int x : other)
      order.push_back(x);
    for (int x : needH)
      order.push_back(x);
    for (int x : needM)
      order.push_back(x);

    if (canWin(mine, his, order))
      return (int)order.size();

    sort(nums.begin(), nums.end());
    reverse(nums.begin(), nums.end());
    if (canWin(mine, his, nums))
      return (int)nums.size();

    return -1;
  }
};
