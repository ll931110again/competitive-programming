#include "grader.h"

#include <cmath>
#include <cstring>

namespace {

constexpr int kLanguageCount = 56;
constexpr int kExcerptLength = 100;
constexpr int kHashBuckets = 1 << 18;
constexpr int kHashMask = kHashBuckets - 1;
constexpr double kSmoothingAlpha = 0.0015;
constexpr int kLogLookupSize = 1 << 17;

struct NGramOrder {
  int length;
  int count;
};

constexpr NGramOrder kOrders[] = {{2, 99}, {3, 98}, {4, 97}};
constexpr int kOrderCount = sizeof(kOrders) / sizeof(kOrders[0]);

constexpr int kMaxNGramsPerOrder = 99;
constexpr int kMaxDistinctNGrams = 99;

int HashCombine(int hash, int symbol) {
  return (hash * 65537 + symbol) & kHashMask;
}

int HashNGram(const int* symbols, int start, int length) {
  int hash = 0;
  for (int i = 0; i < length; ++i) {
    hash = HashCombine(hash, symbols[start + i]);
  }
  return hash;
}

struct ExcerptProfile {
  int distinct_count[kOrderCount];
  int bucket_ids[kOrderCount][kMaxDistinctNGrams];
  int frequencies[kOrderCount][kMaxDistinctNGrams];
};

class LanguageModels {
public:
  bool IsReady(int language) const {
    if (!seen_[language]) {
      return false;
    }
    for (int order = 0; order < kOrderCount; ++order) {
      if (total_ngrams_[order][language] == 0) {
        return false;
      }
    }
    return true;
  }

  float LogNormalization(int order, int language) const {
    return log_normalization_[order][language];
  }

  int NGramCount(int order, int language, int bucket) const {
    return counts_[order][language][bucket];
  }

  void RecordExcerpt(int language, const int ngram_hashes[kOrderCount][kMaxNGramsPerOrder]) {
    seen_[language] = true;
    for (int order = 0; order < kOrderCount; ++order) {
      for (int i = 0; i < kOrders[order].count; ++i) {
        ++counts_[order][language][ngram_hashes[order][i]];
      }
      total_ngrams_[order][language] += kOrders[order].count;
      log_normalization_[order][language] =
          std::log(total_ngrams_[order][language] + kSmoothingAlpha * kHashBuckets);
    }
  }

private:
  bool seen_[kLanguageCount]{};
  int counts_[kOrderCount][kLanguageCount][kHashBuckets]{};
  int total_ngrams_[kOrderCount][kLanguageCount]{};
  float log_normalization_[kOrderCount][kLanguageCount]{};
};

class LogLookup {
public:
  float LogSmoothedCount(int count) const {
    if (count < kLogLookupSize) {
      return table_[count];
    }
    return std::log(count + kSmoothingAlpha);
  }

  LogLookup() {
    for (int count = 0; count < kLogLookupSize; ++count) {
      table_[count] = std::log(count + kSmoothingAlpha);
    }
  }

private:
  float table_[kLogLookupSize]{};
};

class LanguageIdentifier {
public:
  void ClassifyAndLearn(const int symbols[kExcerptLength]) {
    ComputeNGramHashes(symbols);
    BuildExcerptProfile();

    const int guess = PredictLanguage();
    const int actual_language = language(guess);

    models_.RecordExcerpt(actual_language, ngram_hashes_);
  }

private:
  void ComputeNGramHashes(const int symbols[kExcerptLength]) {
    for (int order = 0; order < kOrderCount; ++order) {
      const int length = kOrders[order].length;
      for (int start = 0; start < kOrders[order].count; ++start) {
        ngram_hashes_[order][start] = HashNGram(symbols, start, length);
      }
    }
  }

  void BuildExcerptProfile() {
    for (int order = 0; order < kOrderCount; ++order) {
      ++profile_generation_;
      if (profile_generation_ == 0) {
        std::memset(profile_stamp_, 0, sizeof profile_stamp_);
        profile_generation_ = 1;
      }

      int distinct = 0;
      for (int i = 0; i < kOrders[order].count; ++i) {
        const int bucket = ngram_hashes_[order][i];
        if (profile_stamp_[bucket] != profile_generation_) {
          profile_stamp_[bucket] = profile_generation_;
          profile_scratch_[bucket] = 1;
          profile_.bucket_ids[order][distinct++] = bucket;
        } else {
          ++profile_scratch_[bucket];
        }
      }

      profile_.distinct_count[order] = distinct;
      for (int i = 0; i < distinct; ++i) {
        const int bucket = profile_.bucket_ids[order][i];
        profile_.frequencies[order][i] = profile_scratch_[bucket];
      }
    }
  }

  float CrossEntropy(int language, int order) const {
    const float inverse_total = 1.0f / kOrders[order].count;
    const float log_norm = models_.LogNormalization(order, language);
    float score = 0.0f;

    for (int i = 0; i < profile_.distinct_count[order]; ++i) {
      const int bucket = profile_.bucket_ids[order][i];
      const float probability = profile_.frequencies[order][i] * inverse_total;
      const int model_count = models_.NGramCount(order, language, bucket);
      score += probability * (log_lookup_.LogSmoothedCount(model_count) - log_norm);
    }
    return score;
  }

  float ScoreLanguage(int language) const {
    float total = 0.0f;
    for (int order = 0; order < kOrderCount; ++order) {
      total += CrossEntropy(language, order);
    }
    return total / kOrderCount;
  }

  int PredictLanguage() const {
    int best_language = 0;
    float best_score = -1e30f;
    bool has_candidate = false;

    for (int language = 0; language < kLanguageCount; ++language) {
      if (!models_.IsReady(language)) {
        continue;
      }
      has_candidate = true;
      const float score = ScoreLanguage(language);
      if (score > best_score) {
        best_score = score;
        best_language = language;
      }
    }

    return has_candidate ? best_language : 0;
  }

  LanguageModels models_;
  LogLookup log_lookup_;
  ExcerptProfile profile_;

  int ngram_hashes_[kOrderCount][kMaxNGramsPerOrder]{};
  int profile_stamp_[kHashBuckets]{};
  int profile_scratch_[kHashBuckets]{};
  int profile_generation_ = 1;
};

LanguageIdentifier g_identifier;

} // namespace

void excerpt(int e[100]) {
  g_identifier.ClassifyAndLearn(e);
}
