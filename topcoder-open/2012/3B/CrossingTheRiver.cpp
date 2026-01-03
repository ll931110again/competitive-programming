#include <algorithm>
#include <bitset>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <functional>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <stack>
#include <utility>
#include <vector>
#include <cstring>
using namespace std;

struct CrossingTheRiver {
	string isItEvenPossible(int waterWidth, int landWidth, vector <int> blockHeight, int depth) {
    	vector<int> cnt(101);
        for (int i = 0; i < blockHeight.size(); i++) {
			cnt[blockHeight[i]]++;
        }
        if (cnt[depth] >= waterWidth) {
			return "POSSIBLE";
        }

        for (int wl = depth; wl <= depth + 1; wl++) {
            for (int wr = wl; wr <= 100 && wr - wl + 1 <= waterWidth; wr++) {
                for (int ll = max(0, wr - depth); ll <= wr - depth + 1; ll++) {
                    for (int lr = ll; lr <= 100 && lr - ll + 1 <= landWidth; lr++) {
                        if (solved(wl, wr, ll, lr, cnt, waterWidth, landWidth)) {
                            return "POSSIBLE";
                        }
                    }
                }
            }
        }
                            
        return "IMPOSSIBLE";
    };
                            
    bool solved(int wl, int wr, int ll, int lr, vector<int> cnt, int waterWidth, int landWidth) {
        for (int i = wl; i <= wr; i++) {
			if (!cnt[i]) {
                return false;
			}
            cnt[i]--;
        }
        for (int i = ll; i <= lr; i++) {
            if (!cnt[i]) {
                return false;
            }
            cnt[i]--;
        }

        int waterOnly = 0, landOnly = 0, both = 0, waterRem = waterWidth - (wr - wl + 1), landRem = landWidth - (lr - ll + 1);
        for (int i = 1; i <= 100; i++) {
            if (wl <= i && i <= wr && ll <= i && i <= lr) {
                both += cnt[i];
            } else if (wl <= i && i <= wr) {
                waterOnly += cnt[i];
            } else if (ll <= i && i <= lr) {
                landOnly += cnt[i];
            }
        }

        waterRem = max(0, waterRem - waterOnly);
        landRem = max(0, landRem - landOnly);
        return (waterRem + landRem <= both);
    };
};