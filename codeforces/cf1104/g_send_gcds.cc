
#include <bits/stdc++.h>
#define maxn 2005
#define maxk 1000005
using namespace std;

int T, n, k;
int a[maxn], b[maxn];

string command;

const int MAX_P = 110;
const int MAX_SMOOTH = 18;
const int PARTITION_SIZE = 20;

int sieve[maxk];

vector<int> primes, smooths;
int smooths_index[maxk];
vector<int> primes_powers;

void generate_primes() {
  for (int i = 1; i < maxk; i++) {
    sieve[i] = i;
  }

  for (int i = 2; i < maxk; i++)
    if (sieve[i] == i) {
      primes.push_back(i);
      for (int j = i; j < maxk; j += i) {
        sieve[j] = i;
      }
    }

  primes.resize(MAX_P);
  primes_powers.resize(MAX_P);

  for (int i = 0; i < MAX_P; i++) {
    primes_powers[i] = primes[i];
    while (primes_powers[i] >= maxk / primes[i]) {
      primes_powers[i] *= primes[i];
    }
  }

  for (int i = 1; i < maxk; i++) {
    sieve[i] = i;
  }

  for (auto p : primes) {
    for (int j = p; j < maxk; j += p) {
      while (sieve[j] % p == 0) {
        sieve[j] /= p;
      }
    }
  }

  for (int i = 1; i < maxk; i++)
    if (sieve[i] == 1) {
      smooths.push_back(i);
    }

  smooths.resize(1 << MAX_SMOOTH);
  for (int i = 0; i < smooths.size(); i++) {
    smooths_index[smooths[i]] = i;
  }
}

struct Encoder {
  void solve() {
    cin >> T;
    while (T--) {
      solve_one();
    }
  }

  void solve_one() {
    cin >> n;
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }

    vector<int> bits;
    for (int i = 0; i < n; i++) {
      for (int j = PARTITION_SIZE - 1; j >= 0; j--) {
        if (a[i] & (1 << j)) {
          bits.push_back(1);
        } else {
          bits.push_back(0);
        }
      }
    }

    vector<int> encoded = smooths;
    for (int i = 0; i < bits.size(); i += MAX_SMOOTH) {
      int value = 0;
      for (int j = 0; j < MAX_SMOOTH; j++) {
        if (i + j < bits.size() && bits[i + j]) {
          value |= (1 << j);
        }
      }
      encoded.push_back(value);
    }

    cout << encoded.size() << endl;
    for (auto x : encoded) {
      cout << x << ' ';
    }
    cout << endl;
  }
};

struct Decoder {
  void solve() {
    cin >> T;
    while (T--) {
      solve_one();
    }
  }

  int gcd(int i, int j) {
    cout << "? " << i << ' ' << j << endl;
    cout << flush;

    int x;
    cin >> x;
    return x;
  }

  void output(vector<int>& answer) {
    cout << "! ";
    for (auto x : answer) {
      cout << x << ' ';
    }
    cout << endl;
    cout << flush;
  }

  void solve_one() {
    cin >> n >> k;

    vector<int> recovered;
    vector<int> answer;
    for (int i = MAX_P; i < k; i++) {
      int value = 1;
      for (int j = 0; j < MAX_P; j++) {
        value *= gcd(i + 1, j + 1);
      }
      int idx = smooths_index[value];
    }
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  generate_primes();

  cin >> command;
  if (command == "first") {
    Encoder.solve();
  } else {
    Decoder.solve();
  }
  return 0;
}
