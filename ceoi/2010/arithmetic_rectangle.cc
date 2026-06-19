/*
 * Solution for CEOI 2010 Day 1: An Arithmetic Rectangle.
 *
 * grid[i][j] = A + j*(B-A) + i*(C-A) + i*j*(A-B-C+D). Each known cell is a
 * linear equation in (A,B,C,D); solve by Gaussian elimination with integers.
 */

#include <bits/stdc++.h>
using namespace std;

using i64 = long long;
namespace {

struct Fraction {
  i64 num = 0;
  i64 den = 1;

  Fraction() = default;
  Fraction(i64 n) : num(n), den(1) {}
  Fraction(i64 n, i64 d) {
    if (d < 0) {
      n = -n;
      d = -d;
    }
    num = n;
    den = d;
    normalize();
  }

  void normalize() {
    if (den == 0) {
      return;
    }
    i64 g = gcd(abs(num), abs(den));
    num /= g;
    den /= g;
  }

  bool zero() const {
    return num == 0;
  }

  Fraction operator+(const Fraction& other) const {
    return Fraction(num * other.den + other.num * den, den * other.den);
  }

  Fraction operator-(const Fraction& other) const {
    return Fraction(num * other.den - other.num * den, den * other.den);
  }

  Fraction operator*(const Fraction& other) const {
    return Fraction(num * other.num, den * other.den);
  }

  Fraction operator/(const Fraction& other) const {
    return Fraction(num * other.den, den * other.num);
  }

  bool operator==(const Fraction& other) const {
    return num * other.den == other.num * den;
  }

  string str() const {
    if (den == 1) {
      return to_string(num);
    }
    return to_string(num) + "/" + to_string(den);
  }
};

int rows, cols;
vector<vector<Fraction>> eqs;
Fraction vars[4];

void add_equation(int i, int j, i64 value) {
  vector<Fraction> eq(5);
  eq[0] = Fraction(1 - j - i + i * j);
  eq[1] = Fraction(j - i * j);
  eq[2] = Fraction(i - i * j);
  eq[3] = Fraction(i * j);
  eq[4] = Fraction(value);
  eqs.push_back(eq);
}

void eliminate() {
  int pivot_row = 0;
  for (int col = 0; col < 4; col++) {
    int sel = -1;
    for (int row = pivot_row; row < (int)eqs.size(); row++) {
      if (!eqs[row][col].zero()) {
        sel = row;
        break;
      }
    }
    if (sel < 0) {
      continue;
    }
    swap(eqs[pivot_row], eqs[sel]);

    for (int row = 0; row < (int)eqs.size(); row++) {
      if (row == pivot_row || eqs[row][col].zero()) {
        continue;
      }
      Fraction factor = eqs[row][col] / eqs[pivot_row][col];
      for (int k = 0; k < 5; k++) {
        eqs[row][k] = eqs[row][k] - factor * eqs[pivot_row][k];
      }
    }
    pivot_row++;
  }
}

bool solve() {
  eliminate();

  for (const auto& eq : eqs) {
    bool all_zero = true;
    for (int i = 0; i < 4; i++) {
      if (!eq[i].zero()) {
        all_zero = false;
        break;
      }
    }
    if (all_zero && !eq[4].zero()) {
      return false;
    }
  }

  vector<int> pivot_col(4, -1);
  for (int row = 0; row < (int)eqs.size(); row++) {
    int lead = -1;
    for (int col = 0; col < 4; col++) {
      if (!eqs[row][col].zero()) {
        lead = col;
        break;
      }
    }
    if (lead >= 0) {
      pivot_col[lead] = row;
    }
  }

  for (int col = 3; col >= 0; col--) {
    if (pivot_col[col] < 0) {
      vars[col] = Fraction(0);
      continue;
    }
    Fraction rhs = eqs[pivot_col[col]][4];
    for (int k = col + 1; k < 4; k++) {
      rhs = rhs - eqs[pivot_col[col]][k] * vars[k];
    }
    vars[col] = rhs / eqs[pivot_col[col]][col];
  }
  return true;
}

Fraction cell_value(int i, int j) {
  return vars[0] + Fraction(j) * (vars[1] - vars[0]) + Fraction(i) * (vars[2] - vars[0]) +
         Fraction(i * j) * (vars[0] - vars[1] - vars[2] + vars[3]);
}

} // namespace

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  cin >> rows >> cols;
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      string token;
      cin >> token;
      if (token != ".") {
        add_equation(i, j, stoll(token));
      }
    }
  }

  if (!solve()) {
    cout << "No solution.\n";
    return 0;
  }

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      if (j) {
        cout << ' ';
      }
      cout << cell_value(i, j).str();
    }
    cout << '\n';
  }
  return 0;
}
