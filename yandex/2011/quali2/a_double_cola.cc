// Codeforces 82 (Yandex.Algorithm 2011: Qualification 2) — A. Double Cola
// Submission: https://codeforces.com/contest/82/submission/434629

#include <bits/stdc++.h>
using namespace std;

string s[5] = {"Sheldon","Leonard","Penny","Rajesh","Howard"};

int main()
{
    int n;  scanf("%d", &n);
    long long T = 1;
    while (1)
    {
        for (int iter = 0; iter < 5; iter++) if (n <= T)
        {
            cout << s[iter] << endl;  return 0;
        }
        else n -= T;
        T *= 2LL;
    }
}
