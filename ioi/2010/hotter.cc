#include "grader.h"
using namespace std;

int HC(int N) {
    Guess(1);
    for (int i = 2; i <= N; i++) {
        int value = Guess(i);
        if (value == -1) {
            return i - 1;
        }
    }
    return N;
}

int main() {
    return 0;
}