int ss[3001][3001];

int rectangle(int R, int C, int H, int W, int a[3001][3001]) {
    int low = 1, high = R * C, ans = high;
    while (low <= high) {
        int mid = (low + high) / 2;
        for (int i = 0; i < R; i++) {
            for (int j = 0; j < C; j++) {
                ss[i][j] = (a[i][j] <= mid) ? 1 : -1;
            }
        }

        for (int i = 1; i < R; i++) {
            for (int j = 0; j < C; j++) {
                ss[i][j] += ss[i - 1][j];
            }
        }

        for (int j = 1; j < C; j++) {
            for (int i = 0; i < R; i++) {
                ss[i][j] += ss[i][j - 1];
            }
        }

        bool ok = false;
        for (int i = H - 1; i < R; i++) {
            if (ok) {
                break;
            }
            for (int j = W - 1; j < C; j++) {
                if (ok) {
                    break;
                }

                if (i < 0 || j < 0) {
                    continue;
                }

                int value = ss[i][j];
                if (i >= H) {
                    value -= ss[i - H][j];
                }
                if (j >= W) {
                    value -= ss[i][j - W];
                }
                if (i >= H && j >= W) {
                    value += ss[i - H][j - W];
                }
                if (value >= 0) {
                    ok = true;
                }
            }
        }

        if (ok) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    return ans;
}