void play() {
  int index[25][2];

  for (int lt = 0; lt < 25; ++lt) {
    index[lt][0] = 0;
    index[lt][1] = 0;
  }

  for (int i = 1; i <= 50; ++i) {
    const char r = faceup(i);
    const int lt = r - 'A';
    const int k = index[lt][0] ? 1 : 0;
    index[lt][k] = i;
  }

  for (int lt = 0; lt < 25; ++lt) {
    faceup(index[lt][0]);
    faceup(index[lt][1]);
  }
}
