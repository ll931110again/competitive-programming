#pragma once

#include <cstdint>
#include <iostream>

using i64 = long long;

// Header-only modular arithmetic helper for competitive programming.
// Intended usage:
//   static constexpr unsigned MOD = 1'000'000'007;
//   using Mint = ModInt<MOD>;
template <unsigned M_> struct ModInt {
  static constexpr unsigned M = M_;
  unsigned x;

  constexpr ModInt() : x(0U) {}
  constexpr ModInt(unsigned v) : x(v % M) {}
  constexpr ModInt(unsigned long long v) : x(static_cast<unsigned>(v % M)) {}
  constexpr ModInt(int v) : x(((v %= static_cast<int>(M)) < 0) ? (v + static_cast<int>(M)) : v) {}
  constexpr ModInt(i64 v) : x(((v %= static_cast<i64>(M)) < 0) ? (v + static_cast<i64>(M)) : v) {}

  static constexpr unsigned mod() {
    return M;
  }

  ModInt& operator+=(const ModInt& a) {
    x = ((x += a.x) >= M) ? (x - M) : x;
    return *this;
  }
  ModInt& operator-=(const ModInt& a) {
    x = ((x -= a.x) >= M) ? (x + M) : x;
    return *this;
  }
  ModInt& operator*=(const ModInt& a) {
    x = static_cast<unsigned>((static_cast<unsigned long long>(x) * a.x) % M);
    return *this;
  }
  ModInt& operator/=(const ModInt& a) {
    return (*this *= a.inv());
  }

  ModInt pow(i64 e) const {
    if (e < 0)
      return inv().pow(-e);
    ModInt a = *this, b = 1U;
    for (; e; e >>= 1) {
      if (e & 1)
        b *= a;
      a *= a;
    }
    return b;
  }

  ModInt inv() const {
    // Works when M is prime or x and M are coprime.
    unsigned a = M, b = x;
    int y = 0, z = 1;
    for (; b;) {
      const unsigned q = a / b;
      const unsigned c = a - q * b;
      a = b;
      b = c;
      const int w = y - static_cast<int>(q) * z;
      y = z;
      z = w;
    }
    return ModInt(y);
  }

  ModInt operator+() const {
    return *this;
  }
  ModInt operator-() const {
    ModInt a;
    a.x = x ? (M - x) : 0U;
    return a;
  }

  ModInt operator+(const ModInt& a) const {
    return (ModInt(*this) += a);
  }
  ModInt operator-(const ModInt& a) const {
    return (ModInt(*this) -= a);
  }
  ModInt operator*(const ModInt& a) const {
    return (ModInt(*this) *= a);
  }
  ModInt operator/(const ModInt& a) const {
    return (ModInt(*this) /= a);
  }

  ModInt& operator++() {
    *this += 1U;
    return *this;
  }
  ModInt operator++(int) {
    ModInt tmp = *this;
    ++(*this);
    return tmp;
  }

  template <class T> friend ModInt operator+(T a, const ModInt& b) {
    return (ModInt(a) += b);
  }
  template <class T> friend ModInt operator-(T a, const ModInt& b) {
    return (ModInt(a) -= b);
  }
  template <class T> friend ModInt operator*(T a, const ModInt& b) {
    return (ModInt(a) *= b);
  }
  template <class T> friend ModInt operator/(T a, const ModInt& b) {
    return (ModInt(a) /= b);
  }

  explicit operator bool() const {
    return x;
  }
  bool operator==(const ModInt& a) const {
    return x == a.x;
  }
  bool operator!=(const ModInt& a) const {
    return x != a.x;
  }

  friend std::ostream& operator<<(std::ostream& os, const ModInt& a) {
    return os << a.x;
  }
};
