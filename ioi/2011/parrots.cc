#include "parrotslib.h"

#include <algorithm>
#include <array>
#include <cstdint>
#include <vector>

namespace {

// Blank slots in the combinatorial sequence use symbol 256 and are never sent.
// Transmitted parrot calls use values 0..255 only.
constexpr int kSeqPad = 256;

// Minimum sequence length L such that C(256 + L, L) >= 256^n.
constexpr int kSeqLenByN[] = {
    0,   1,   3,   4,   5,   7,   8,   10,  12,  14,  15,  17,  19,  22,  24,  26,  28,
    31,  33,  36,  39,  41,  44,  47,  50,  53,  57,  60,  63,  67,  70,  74,  78,  82,
    86,  90,  94,  98,  102, 107, 111, 116, 121, 126, 131, 136, 141, 147, 152, 158, 164,
    170, 176, 182, 189, 195, 202, 209, 216, 223, 230, 238, 245, 253, 261,
};

constexpr int kBinomMax = 520;

class BigInt {
public:
  static constexpr int kMaxBytes = 99;

  BigInt() = default;

  explicit BigInt(int value) {
    while (value > 0) {
      bytes_[size_++] = static_cast<uint8_t>(value & 255);
      value >>= 8;
    }
  }

  bool operator<(const BigInt& other) const {
    if (size_ != other.size_) {
      return size_ < other.size_;
    }
    for (int i = size_ - 1; i >= 0; --i) {
      if (bytes_[i] != other.bytes_[i]) {
        return bytes_[i] < other.bytes_[i];
      }
    }
    return false;
  }

  bool operator>=(const BigInt& other) const {
    return !(*this < other);
  }

  BigInt& operator+=(const BigInt& other) {
    int carry = 0;
    const int max_index = std::max(size_, other.size_);
    for (int i = 0; i < max_index || carry; ++i) {
      const int sum = byte_at(*this, i) + byte_at(other, i) + carry;
      bytes_[i] = static_cast<uint8_t>(sum & 255);
      carry = sum >> 8;
      if (i >= size_) {
        size_ = i + 1;
      }
    }
    if (carry) {
      bytes_[size_++] = static_cast<uint8_t>(carry);
    }
    trim();
    return *this;
  }

  BigInt operator+(const BigInt& other) const {
    BigInt result = *this;
    result += other;
    return result;
  }

  BigInt& operator-=(const BigInt& other) {
    int borrow = 0;
    for (int i = 0; i < size_; ++i) {
      int diff = static_cast<int>(bytes_[i]) - byte_at(other, i) - borrow;
      if (diff < 0) {
        diff += 256;
        borrow = 1;
      } else {
        borrow = 0;
      }
      bytes_[i] = static_cast<uint8_t>(diff);
    }
    trim();
    return *this;
  }

  void mul256_add(int byte) {
    int carry = byte;
    for (int i = 0; i < size_ || carry; ++i) {
      if (i >= size_) {
        bytes_[size_++] = 0;
      }
      const int prod = static_cast<int>(bytes_[i]) * 256 + carry;
      bytes_[i] = static_cast<uint8_t>(prod & 255);
      carry = prod >> 8;
    }
    trim();
  }

  int pop_lsb() {
    if (size_ == 0) {
      return 0;
    }
    const int lsb = bytes_[0];
    for (int i = 0; i + 1 < size_; ++i) {
      bytes_[i] = bytes_[i + 1];
    }
    --size_;
    return lsb;
  }

private:
  static int byte_at(const BigInt& value, int index) {
    return index < value.size_ ? value.bytes_[index] : 0;
  }

  void trim() {
    while (size_ > 0 && bytes_[size_ - 1] == 0) {
      --size_;
    }
  }

  uint8_t bytes_[kMaxBytes]{};
  int size_ = 0;
};

class BinomialTable {
public:
  BinomialTable() {
    table_[0][0] = BigInt(1);
    for (int n = 1; n < kBinomMax; ++n) {
      table_[n][0] = BigInt(1);
      table_[n][n] = BigInt(1);
      for (int k = 1; k < n; ++k) {
        table_[n][k] = table_[n - 1][k - 1] + table_[n - 1][k];
      }
    }
  }

  const BigInt& choose(int n, int k) const {
    return table_[n][k];
  }

  const BigInt& ways_with_cap(int remaining_len, int max_symbol) const {
    return choose(max_symbol + remaining_len, remaining_len);
  }

private:
  std::array<std::array<BigInt, kBinomMax>, kBinomMax> table_{};
};

class CombinatorialCodec {
public:
  explicit CombinatorialCodec(const BinomialTable& binom) : binom_(binom) {}

  std::vector<int> rank_to_sequence(const BigInt& rank, int seq_len) const {
    std::vector<int> seq(seq_len);
    BigInt offset;
    int cap = kSeqPad;

    for (int i = 0; i < seq_len; ++i) {
      const int remaining = seq_len - i - 1;
      for (int symbol = cap; symbol >= 0; --symbol) {
        const BigInt& block = binom_.ways_with_cap(remaining, symbol);
        BigInt next = offset;
        next += block;
        if (!(rank >= next)) {
          seq[i] = symbol;
          cap = symbol;
          break;
        }
        offset += block;
      }
    }
    return seq;
  }

  BigInt multiset_to_rank(std::vector<int> symbols, int seq_len) const {
    while (static_cast<int>(symbols.size()) < seq_len) {
      symbols.push_back(kSeqPad);
    }
    std::sort(symbols.rbegin(), symbols.rend());
    return sequence_to_rank(symbols);
  }

private:
  BigInt sequence_to_rank(const std::vector<int>& seq) const {
    BigInt rank;
    for (int i = 0; i < static_cast<int>(seq.size()); ++i) {
      const int prev = (i > 0) ? seq[i - 1] : kSeqPad;
      const int remaining = static_cast<int>(seq.size()) - i - 1;
      for (int symbol = seq[i] + 1; symbol <= prev; ++symbol) {
        rank += binom_.ways_with_cap(remaining, symbol);
      }
      if (i + 1 == static_cast<int>(seq.size())) {
        rank += binom_.ways_with_cap(remaining, seq.back());
      }
    }

    BigInt one(1);
    rank -= one;
    return rank;
  }

  const BinomialTable& binom_;
};

int seq_len_for(int n) {
  return kSeqLenByN[n];
}

BigInt pack_message(const int* messages, int n) {
  BigInt value;
  for (int i = 0; i < n; ++i) {
    value.mul256_add(messages[i]);
  }
  return value;
}

void unpack_message(const BigInt& value, int n) {
  BigInt copy = value;
  std::vector<int> bytes(n);
  for (int i = n - 1; i >= 0; --i) {
    bytes[i] = copy.pop_lsb();
  }
  for (int byte : bytes) {
    output(byte);
  }
}

const CombinatorialCodec& codec() {
  static const BinomialTable binom;
  static const CombinatorialCodec instance(binom);
  return instance;
}

} // namespace

void encode(int n, int messages[]) {
  const int seq_len = seq_len_for(n);
  const BigInt rank = pack_message(messages, n);
  for (int symbol : codec().rank_to_sequence(rank, seq_len)) {
    if (symbol != kSeqPad) {
      send(symbol);
    }
  }
}

void decode(int n, int L, int X[]) {
  const int seq_len = seq_len_for(n);
  std::vector<int> symbols(X, X + L);
  const BigInt rank = codec().multiset_to_rank(std::move(symbols), seq_len);
  unpack_message(rank, n);
}
