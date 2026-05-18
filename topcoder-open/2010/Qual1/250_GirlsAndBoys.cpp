#include <bits/stdc++.h>
using namespace std;

class GirlsAndBoys {
public:
  int sortThem(string row) {
    int n = row.size();
    int swapsBFirst = 0, swapsGFirst = 0;
    int bRight = 0, gRight = 0;
    for (int i = n - 1; i >= 0; i--) {
      if (row[i] == 'G')
        swapsBFirst += bRight;
      else
        bRight++;
      if (row[i] == 'B')
        swapsGFirst += gRight;
      else
        gRight++;
    }
    return min(swapsBFirst, swapsGFirst);
  }
};
