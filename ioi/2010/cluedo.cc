#include "grader.h"

void Solve() {
    int a = 1, b = 1, c = 1;
    while (true) {
        int output = Theory(a, b, c);
        if (output == 0) {
            break;
        } else if (output == 1) {
            a++;
        } else if (output == 2) {
            b++;
        } else if (output == 3) {
            c++;
        }
    }
}

int main() {
    return 0;
}