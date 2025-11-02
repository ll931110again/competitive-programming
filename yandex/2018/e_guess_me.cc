#ifdef ONLINE_JUDGE
    #include <bits/stdc++.h>
#endif
 
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <random>
#include <iostream>
#include <set>
#include <stack>
#include <map>
#include <queue>
#include <vector>
#include <unordered_map>
#define maxn 1005
using namespace std;
 
int n;
vector<int> permutations;
bool good[maxn];
 
int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
 
    cin >> n;
    for (int i = 1; i <= n; i++) {
        permutations.push_back(i);
    }
 
    // Initialize random number generator
    std::random_device rd; // Provides a non-deterministic seed
    std::mt19937 g(rd());  // Mersenne Twister engine seeded with rd
 
    memset(good, true, sizeof good);
    int distincts = n;
    for (int it = 0; it < 50; it++) {
        shuffle(permutations.begin(), permutations.end(), g);
        /*
         * We pick a random permutation and iterate through the elements.
         * If we increase an element, and the number of distinct elements decreases,
         * then we know that element cannot be the maximum element.
         * 
         * If j is a non maximum element, we can detect that if j lies before both j - 1 and j + 1 in the permutation.
         * The probability of this happening is 1/3.
         * 
         * After 50 iterations, the probability of not finding the maximum element is (2/3)^50, which is approximately 10^-10.
         * For n = 1000, this is approximately 10^-7, which is negligible.
         */
        for (int i = 0; i < n; i++) {
            cout << 0 << ' ' << permutations[i] << endl;
            cout << flush;
 
            int diff;
            cin >> diff;
            if (diff < distincts) {
                good[permutations[i]] = false;
            }
 
            distincts = diff;
        }
    }
 
    for (int i = 1; i <= n; i++) if (good[i]) {
        cout << 1 << ' ' << i << endl;
        cout << flush;
        return 0;
    }
 
    return 0;
}
