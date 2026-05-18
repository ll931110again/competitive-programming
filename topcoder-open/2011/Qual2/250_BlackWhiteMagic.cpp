#include <bits/stdc++.h>
using namespace std;

class BlackWhiteMagic {
public:
  int count(string creatures) {
    int w = 0;
    for (char c : creatures)
      if (c == 'W')
        w++;
    int res = 0;
    for (int i = 0; i < w; i++)
      if (creatures[i] == 'B')
        res++;
    return res;
  }
};
