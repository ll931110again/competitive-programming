#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
#include "parrotslib.h"
#define maxn 105
using namespace std;
 
__int128_t binom[maxn][maxn];
 
__int128_t count_encoding(int max_val, int len) {
    return binom[max_val + len][len];
}
 
int BLOCK_SIZE = 8;
int MAX_VAL = 32;
int LEN = 36;
 
vector<int> generate_encoding(__int128_t val) {
    int len = LEN;
    int max_val = MAX_VAL;
 
    vector<int> ans(len);
 
    __int128_t cnt = 0;
    for (int i = 0; i < len; i++) {
        for (int j = max_val; j >= 0; j--) {
            ans[i] = j;
            __int128_t tmp = count_encoding(len - i - 1, j);
            if (cnt + tmp > val) {
                break;
            }
            cnt += tmp;
        }
        max_val = ans[i];
    }
 
    return ans;
}
 
__int128_t generate_decoding(vector<int> encoded) {
    __int128_t val = 0;
    for (int i = 0; i < encoded.size(); i++) {
        int prev = (i > 0) ? encoded[i - 1] : MAX_VAL;
        for (int j = encoded[i] + 1; j <= prev; j++) {
            val += count_encoding(encoded.size() - i - 1, j);
        }
        if (i == encoded.size() - 1) {
            val += count_encoding(encoded.size() - i - 1, encoded.back());
        }
    }
    return val - 1;
}
 
void encode(int n, int messages[]) {
    binom[0][0] = 1;
    for (int i = 1; i < maxn; i++) {
        for (int j = 0; j <= i; j++) {
            binom[i][j] = binom[i - 1][j];
            if (j) {
                binom[i][j] += binom[i - 1][j - 1];
            }
        }
    }
 
    for (int i = 0; i < n; i += BLOCK_SIZE) {
        __int128_t val = 0;
        for (int j = i; j < i + BLOCK_SIZE; j++) {
            val = val * 256;
            if (j < n) {
                val += messages[j];
            }
        }
 
        auto encoded = generate_encoding(val);
        for (auto x : encoded) if (x != MAX_VAL) {
            // cerr << i/8 * 32 + x << ", ";
            send((i/8) * 32 + x);
        }
        // cerr << endl;
    }
 
    // cerr << endl;
}
 
void decode(int n, int L, int X[]) {
    binom[0][0] = 1;
    for (int i = 1; i < maxn; i++) {
        for (int j = 0; j <= i; j++) {
            binom[i][j] = binom[i - 1][j];
            if (j) {
                binom[i][j] += binom[i - 1][j - 1];
            }
        }
    }
 
    vector< vector<int> > collected(n);
    for (int i = 0; i < L; i++) {
        collected[X[i] / MAX_VAL].push_back(X[i] % MAX_VAL);
    }
 
    for (int i = 0; i * BLOCK_SIZE < n; i++) {
        while (collected[i].size() < LEN) {
            collected[i].push_back(MAX_VAL);
        }
        sort(collected[i].rbegin(), collected[i].rend());
 
        __int128_t val = generate_decoding(collected[i]);
        vector<int> ans;
        for (int j = i; j < i + BLOCK_SIZE; j++) {
            ans.push_back(val % 256);
            val /= 256;
        }
        reverse(ans.begin(), ans.end());
        for (int j = i * BLOCK_SIZE; j < (i + 1) * BLOCK_SIZE && j < n; j++) {
            // cerr << ans[j - i * BLOCK_SIZE] << endl;
            output(ans[j - i * 8]);
        }
    }
}