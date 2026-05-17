#include <algorithm>
#include <iostream>
#include <numeric>
#include <set>
#include <vector>

using namespace std;

using int64 = long long;

static vector<int> sieve_primes(int n) {
  vector<bool> is_prime(n + 1, true);
  is_prime[0] = is_prime[1] = false;
  for (int i = 2; i * i <= n; i++) {
    if (!is_prime[i])
      continue;
    for (int j = i * i; j <= n; j += i)
      is_prime[j] = false;
  }
  vector<int> primes;
  for (int i = 2; i <= n; i++)
    if (is_prime[i])
      primes.push_back(i);
  return primes;
}

static int64 mod_norm(int64 x, int64 mod) {
  x %= mod;
  if (x < 0)
    x += mod;
  return x;
}

static int64 mod_inv(int64 a, int64 mod) {
  int64 t = 0, newt = 1;
  int64 r = mod, newr = mod_norm(a, mod);
  while (newr != 0) {
    int64 q = r / newr;
    int64 nt = newt;
    newt = t - q * newt;
    t = nt;
    int64 nr = newr;
    newr = r - q * newr;
    r = nr;
  }
  if (r != 1)
    return -1;
  if (t < 0)
    t += mod;
  return t;
}

static bool verify(const vector<int64>& s, int64 P, int64 A, int64 B) {
  int K = (int)s.size();
  for (int i = 0; i < K - 1; i++) {
    if (((A * s[i] + B) % P + P) % P != s[i + 1])
      return false;
  }
  return true;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  const int MAXV = 1000000;
  vector<int> primes = sieve_primes(MAXV);

  int T;
  cin >> T;
  for (int tc = 1; tc <= T; tc++) {
    int D, K;
    cin >> D >> K;
    int64 maxP = 1;
    for (int i = 0; i < D; i++)
      maxP *= 10;

    vector<int64> s(K);
    for (int i = 0; i < K; i++)
      cin >> s[i];

    set<int64> cand_next;

    bool all_equal = true;
    for (int i = 1; i < K; i++) {
      if (s[i] != s[0])
        all_equal = false;
    }

    for (int P : primes) {
      if ((int64)P > maxP)
        break;

      bool ok_range = true;
      for (int i = 0; i < K; i++) {
        if (s[i] >= P) {
          ok_range = false;
          break;
        }
      }
      if (!ok_range)
        continue;

      if (K == 1) {
        continue;
      }

      if (all_equal) {
        int64 c = s[0];
        cand_next.insert(c);
        continue;
      }

      if (K == 2) {
        int64 d = mod_norm(s[1] - s[0], P);
        if (d == 0)
          continue;
        int64 g = std::gcd(d, (int64)P);
        if (g == 1) {
          int64 n0 = mod_norm(s[1], P);
          int64 n1 = mod_norm(d + s[1], P);
          cand_next.insert(n0);
          cand_next.insert(n1);
        } else {
          for (int64 A = 0; A < P; A++) {
            int64 B = mod_norm(s[1] - A * s[0], P);
            if (!verify(s, P, A, B))
              continue;
            cand_next.insert((A * s[1] + B) % P);
          }
        }
        continue;
      }

      int64 d = mod_norm(s[1] - s[0], P);
      int64 e = mod_norm(s[2] - s[1], P);

      if (d == 0) {
        if (e != 0)
          continue;
        bool bad = false;
        for (int i = 1; i < K - 1; i++) {
          int64 di = mod_norm(s[i] - s[i - 1], P);
          int64 ei = mod_norm(s[i + 1] - s[i], P);
          if (di == 0 && ei != 0) {
            bad = true;
            break;
          }
        }
        if (bad)
          continue;
        int64 c = s[0];
        for (int64 A = 0; A < P; A++) {
          int64 B = mod_norm(c - A * c, P);
          if (!verify(s, P, A, B))
            continue;
          cand_next.insert((A * s[K - 1] + B) % P);
        }
        continue;
      }

      int64 inv = mod_inv(d, P);
      if (inv < 0)
        continue;
      int64 A = (e * inv) % P;

      bool eq_ok = true;
      for (int i = 2; i < K - 1; i++) {
        int64 di = mod_norm(s[i] - s[i - 1], P);
        int64 ei = mod_norm(s[i + 1] - s[i], P);
        if ((A * di) % P != ei) {
          eq_ok = false;
          break;
        }
      }
      if (!eq_ok)
        continue;

      int64 B = mod_norm(s[1] - A * s[0], P);
      if (!verify(s, P, A, B))
        continue;

      int64 nxt = (A * s[K - 1] + B) % P;
      cand_next.insert(nxt);
    }

    cout << "Case #" << tc << ": ";
    if (cand_next.size() == 1) {
      cout << *cand_next.begin() << "\n";
    } else {
      cout << "I don't know.\n";
    }
  }
  return 0;
}
