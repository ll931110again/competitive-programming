#include <cstdlib>
#include <cstring>
#include <iostream>
#include <map>
#include <random>
#include <string>
#include <utility>
#include <vector>
#define maxn 150
using namespace std;

int T, n;
int board[maxn][maxn];

int main() {
    cin >> n >> T;
    memset(board, 0, sizeof(board));

    random_device rd;
    mt19937 rng(rd());

    map< pair<int, int>, int > current_distances;
    pair<int, int> current_point = {1, 1};

    int dx[4] = {0, 0, 1, -1};
    int dy[4] = {1, -1, 0, 0};

    while (T--) {
        pair<int, int> chosen = {-1, -1};
        for (int i = 0; i < 4; i++) {
            int nx = current_point.first + dx[i];
            int ny = current_point.second + dy[i];
            if (nx < 1 || nx > n || ny < 1 || ny > n) {
                continue;
            }
            if (board[nx][ny] == -1) {
                continue;
            }
            if (current_distances.find({nx, ny}) != current_distances.end()) {
                continue;
            }
            chosen = {nx, ny};
            break;
        }
        cout << chosen.first << " " << chosen.second << endl;
        cout << flush;

        int dist;
        cin >> dist;

        if (dist == -1) {
            board[chosen.first][chosen.second] = -1;
        } else if (dist == 0) {
            // Found the target, move to the next target
            current_point = chosen;
            current_distances.clear();
        } else {
            // Add the new distance to the map
            current_distances[chosen] = dist;
            if (current_distances[chosen] < current_distances[current_point]) {
                current_point = chosen;
            }
        }
    }
    return 0;
}
