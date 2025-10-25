#include <bits/stdc++.h>
#define maxn 100005
using namespace std;
 
long long ss[maxn];
 
int besthub(int R, int L, int X[], long long B) {
    ss[0] = X[0];
    for (int i = 1; i < R; i++) {
        ss[i] = ss[i - 1] + X[i];
    }
 
    auto ssSum = [&](int i, int j) {
        return ss[j] - (i > 0 ? ss[i - 1] : 0);
    };
 
    auto computeCost = [&](int i, int j) {
        int k = (i + j) / 2;
        long long dist_left = 1LL * (k - i + 1) * X[k] - ssSum(i, k);
        long long dist_right = ssSum(k, j) - 1LL * (j - k + 1) * X[k];
        return dist_left + dist_right;
    };
 
    int i = 0, j = 0, max_count = 1;
    while (i < R) {
        if (j < i) {
            j = i;
        }
        while (j < R && computeCost(i, j) <= B) {
            j++;
        }
        max_count = max(max_count, j - i);
        i++;
    }
 
    return max_count;
}
