#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unordered_set>
#include <vector>
#define maxn 100005
using namespace std;
 
int T, n, deleteCost, insertCost;
int a[maxn];
 
long long solve() {
    scanf("%d %d %d", &n, &deleteCost, &insertCost);
    for (int i = 0; i < n; i++) {
        scanf("%d", &a[i]);
    }
    sort(a, a + n);
 
    // first case corresponds to delete everything and insert 1.
    long long minCost = 1LL * n * deleteCost + insertCost;
    unordered_set<int> reps;
    for (int i = 0; i < n; i++) {
        reps.insert(a[i]);
        minCost = min((unsigned long long) minCost,
                       1LL * (i + 1 - reps.size() + (n - 1 - i)) * deleteCost + 1LL * (a[i] - reps.size()) * insertCost);
    }
 
    return minCost;
}
 
int main() {
    scanf("%d", &T);
    while (T--) {
        printf("%lld\n", solve());
    }
    return 0;
}
