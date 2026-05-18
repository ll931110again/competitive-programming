#include <bits/stdc++.h>
using namespace std;

class BankLottery {
public:
  double expectedAmount(vector<int> accountBalance, int weeklyJackpot, int weekCount) {
    int total = 0, mine = accountBalance[0];
    for (int i = 0; i < accountBalance.size(); i++)
      total += accountBalance[i];
    double ret = mine;
    for (int i = 0; i < weekCount; i++) {
      ret *= (1 + 1.0 * weeklyJackpot / total);
      total += weeklyJackpot;
    }
    return ret;
  }
};
