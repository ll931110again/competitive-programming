// C++11
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>
#include <set>
#include <string>

using namespace std;

struct BigInt {
    BigInt(string s) {
        digits.resize(s.size());
        for (int i = 0; i < s.size(); i++) {
            digits[i] = s[i] - '0';
        }
        reverse(digits.begin(), digits.end());
        base = 10;
    }

    BigInt(vector<int> _digits, int _base) : digits(_digits), base(_base) {}

    BigInt toBase(int targetBase) {
        vector<int> d = digits;
        vector<int> newDigits;

        while (!d.empty()) {
            int modVal = 0;
            for (int i = d.size() - 1; i >= 0; i--) {
                modVal = (modVal * base + d[i]) % targetBase;
            }
            newDigits.push_back(modVal);
            for (int i = d.size() - 1; i >= 0; i--) {
                if (i) {
                    d[i - 1] += ((d[i] % targetBase) * base);
                }
                d[i] /= targetBase;
            }

            while (!d.empty() && d.back() == 0) {
                d.pop_back();
            }
        }

        return BigInt(newDigits, targetBase);
    }

    vector<int> digits;
    int base;
};

class NumberCreator {
public:
    vector<string> solutionWithNegativeBase(int Num0, int Num1, string T, int base) {
        numToIdx.clear();
        moves.clear();

        numToIdx[-1] = 0;
        numToIdx[-2] = 1;
        addMove(0, 0, "/");
        numToIdx[1] = 2;

        auto input = BigInt(T).toBase(base);
        if (input.digits.size() == 1) {
            addMove(numToIdx[input.digits.back()], numToIdx[1], "*");
            return moves;
        }

        for (int i = 0; i < input.digits.size(); i++) {
            if (input.digits[i] < 0) {
                continue;
            }
            if (input.digits[i] > base - input.digits[i]) {
                if (i == input.digits.size() - 1) {
                    input.digits.push_back(1);
                } else {
                    input.digits[i + 1]++;                    
                }
                input.digits[i] -= base;
            }
        }

        set<int> inDigits;
        for (int i = 0; i < input.digits.size(); i++) {
            inDigits.insert(abs(input.digits[i]));
        }
        inDigits.insert(base);

        int lastDigit = 1;
        for (int x = 2; x <= base; x++) if (inDigits.count(x)) {
            bool isBaseGood = false;
            for (int i = 1; i < x; i++) if (numToIdx.count(i) && numToIdx.count(x - i)) {
                isBaseGood = true;
                addMove(numToIdx[i], numToIdx[x - i], "+");
                numToIdx[x] = numToIdx.size();
                break;
            }

            if (!isBaseGood) {
                for (int i = 2; i < x; i++)
                    if (x % i == 0 && numToIdx.count(i) && numToIdx.count(x/i)) {
                        isBaseGood = true;
                        addMove(numToIdx[i], numToIdx[x/i], "*");
                        numToIdx[x] = numToIdx.size();
                        break;
                    }                
            }

            if (!isBaseGood) {
                constructSlow(x);
            }

            lastDigit = x;
        }

        for (int i = input.digits.size() - 2; i >= 0; i--) {
            if (i == input.digits.size() - 2) {
                addMove(numToIdx[input.digits.back()], numToIdx[base], "*");
            } else {
                addMove(moves.size() + 1, numToIdx[base], "*");
            }
            int d = input.digits[i];
            if (d > 0) {
                addMove(moves.size() + 1, numToIdx[d], "+");                
            } else if (d < 0) {
                addMove(moves.size() + 1, numToIdx[-d], "-");
            }
        }

        return moves;
    }

    vector<string> solutionWithBase(int Num0, int Num1, string T, int base) {
        numToIdx.clear();
        moves.clear();

        numToIdx[-1] = 0;
        numToIdx[-2] = 1;
        addMove(0, 0, "/");
        numToIdx[1] = 2;

        auto input = BigInt(T).toBase(base);
        if (input.digits.size() == 1) {
            addMove(numToIdx[input.digits.back()], numToIdx[1], "*");
            return moves;
        }

        set<int> inDigits;
        for (int i = 0; i < input.digits.size(); i++) {
            inDigits.insert(input.digits[i]);
        }
        inDigits.insert(base);

        int lastDigit = 1;
        for (int x = 2; x <= base; x++) if (inDigits.count(x)) {
            bool isBaseGood = false;
            for (int i = 1; i < x; i++) if (numToIdx.count(i) && numToIdx.count(x - i)) {
                isBaseGood = true;
                addMove(numToIdx[i], numToIdx[x - i], "+");
                numToIdx[x] = numToIdx.size();
                break;
            }

            if (!isBaseGood) {
                for (int i = 2; i < x; i++)
                    if (x % i == 0 && numToIdx.count(i) && numToIdx.count(x/i)) {
                        isBaseGood = true;
                        addMove(numToIdx[i], numToIdx[x/i], "*");
                        numToIdx[x] = numToIdx.size();
                        break;
                    }                
            }

            if (!isBaseGood) {
                constructSlow(x);
            }

            lastDigit = x;
        }

        for (int i = input.digits.size() - 2; i >= 0; i--) {
            if (i == input.digits.size() - 2) {
                addMove(numToIdx[input.digits.back()], numToIdx[base], "*");
            } else {
                addMove(moves.size() + 1, numToIdx[base], "*");
            }
            int d = input.digits[i];
            if (d > 0) {
                addMove(moves.size() + 1, numToIdx[d], "+");                
            }
        }

        return moves;
    }

    void constructSlow(int x) {
        if (numToIdx.count(x)) {
            return;
        }

        if (!numToIdx.count(2)) {
            addMove(numToIdx[1], numToIdx[1], "+");
            addNewNum(2);
        }

        if (!numToIdx.count(3)) {
            addMove(numToIdx[2], numToIdx[1], "+");
            addNewNum(3);            
        }

        if (x == 2 || x == 3) {
            return;
        }

        constructSlow(x/3);
        if (!numToIdx.count(x/3 * 3)) {
            addMove(numToIdx[x/3], numToIdx[3], "*");
            addNewNum(x/3 * 3);
        }
        if (x % 3) {
            addMove(numToIdx[x/3 * 3], numToIdx[x % 3], "+");
        }
        addNewNum(x);
    }

    vector<string> findSolution(int Num0, int Num1, string T)
    {
        vector<string> optimal;
        int optBase = 0;

        for (int i = 2; i <= 1000; i++) {
            auto choice = solutionWithBase(Num0, Num1, T, i);
            if (optimal.empty() || optimal.size() > choice.size()) {
                optimal = choice;
                optBase = i;
            }
            choice = solutionWithNegativeBase(Num0, Num1, T, i);
            if (optimal.empty() || optimal.size() > choice.size()) {
                optimal = choice;
                optBase = i;
            }            
        }

        return optimal;
    }

    void addNewNum(int x) {
        if (numToIdx.count(x)) {
            return;
        }
        numToIdx[x] = numToIdx.size();
    }

    void addMove(int id0, int id1, string op) {
        auto strOp = to_string(id0) + " " + op + " " + to_string(id1);
        moves.push_back(strOp);
    }

private:
    vector<string> moves;
    map<int, int> numToIdx;
};

int main() {
    NumberCreator nc;
    int Num0;
    int Num1;
    string T;
    cin >> Num0;
    cin >> Num1;
    cin >> T;
    
    vector<string> ret = nc.findSolution(Num0,Num1,T);
    cout << ret.size() << endl;
    for (int i = 0; i < (int)ret.size(); ++i)
        cout << ret[i] << endl;
    cout.flush();
}