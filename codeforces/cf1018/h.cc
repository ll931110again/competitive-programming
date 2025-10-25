#include <iostream>
#include <vector>
#define maxn 200005
#define maxm 18
using namespace std;

const int MOD = 998244353;

// Modular addition
inline int mod_add(int a, int b) {
    a += b;
    if (a >= MOD) a -= MOD;
    return a;
}

// Modular subtraction
inline int mod_sub(int a, int b) {
    a -= b;
    if (a < 0) a += MOD;
    return a;
}

// Modular inverse of 2 mod 998244353 is 499122177
const int INV2 = 499122177;

// Implement Fast Walsh Hadamard Transform
void fwht(vector<int>& a, bool invert) {
    int n = a.size();
    for (int len = 1; 2 * len <= n; len <<= 1) {
        for (int i = 0; i < n; i += 2 * len) {
            for (int j = 0; j < len; ++j) {
                int u = a[i + j];
                int v = a[i + j + len];
                a[i + j] = mod_add(u, v);
                a[i + j + len] = mod_sub(u, v);
                if (invert) {
                    a[i + j] = 1LL * a[i + j] * INV2 % MOD;
                    a[i + j + len] = 1LL * a[i + j + len] * INV2 % MOD;
                }
            }
        }
    }
}

// XOR convolution
vector<int> xor_convolution(vector<int> a, vector<int> b) {
    int n = 1;
    while (n < max(a.size(), b.size())) n <<= 1;
    a.resize(n);
    b.resize(n);
    fwht(a, false);
    fwht(b, false);
    for (int i = 0; i < n; ++i)
        a[i] = 1LL * a[i] * b[i] % MOD;
    fwht(a, true);
    return a;
}

int T, n;
pair<int,int> a[maxn];

int solve() {
    return 0;
}

int main() {
    cin >> T;
    while (T--) {
        cin >> n;
        for (int i = 0; i < n; i++) {
            cin >> a[i].first >> a[i].second;
        }

        cout << solve() << endl;
    }
    return 0;
}