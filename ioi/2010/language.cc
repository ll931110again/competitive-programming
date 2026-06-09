#include "grader.h"
#include <bits/stdc++.h>
using namespace std;

namespace {

constexpr int k_language_count = 56;
constexpr int k_excerpt_length = 100;
constexpr int k_hash_buckets = 1 << 18;
constexpr int k_hash_mask = k_hash_buckets - 1;
constexpr double k_smoothing_alpha = 0.0015;
constexpr int k_log_lookup_size = 1 << 17;

struct NGramOrder {
  int length;
  int count;
};

constexpr NGramOrder k_orders[] = {{2, 99}, {3, 98}, {4, 97}};
constexpr int k_order_count = sizeof(k_orders) / sizeof(k_orders[0]);

constexpr int k_max_n_grams_per_order = 99;
constexpr int k_max_distinct_n_grams = 99;

int HashCombine(int hash, int symbol) {
  return (hash * 65537 + symbol) & k_hash_mask;
}

int HashNGram(const int* symbols, int start, int length) {
  int hash = 0;
  for (int i = 0; i < length; ++i) {
    hash = HashCombine(hash, symbols[start + i]);
  }
  return hash;
}

struct ExcerptProfile {
  int distinct_count[k_order_count];
  int bucket_ids[k_order_count][k_max_distinct_n_grams];
  int frequencies[k_order_count][k_max_distinct_n_grams];
};

class LanguageModels {
public:
  bool IsReady(int language) const {
    if (!seen_[language]) {
      return false;
    }
    for (int order = 0; order < k_order_count; ++order) {
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

  void RecordExcerpt(int language, const int ngram_hashes[k_order_count][k_max_n_grams_per_order]) {
    seen_[language] = true;
    for (int order = 0; order < k_order_count; ++order) {
      for (int i = 0; i < k_orders[order].count; ++i) {
        ++counts_[order][language][ngram_hashes[order][i]];
      }
      total_ngrams_[order][language] += k_orders[order].count;
      log_normalization_[order][language] =
          log(total_ngrams_[order][language] + k_smoothing_alpha * k_hash_buckets);
    }
  }

private:
  bool seen_[k_language_count]{};
  int counts_[k_order_count][k_language_count][k_hash_buckets]{};
  int total_ngrams_[k_order_count][k_language_count]{};
  float log_normalization_[k_order_count][k_language_count]{};
};

class LogLookup {
public:
  float LogSmoothedCount(int count) const {
    if (count < k_log_lookup_size) {
      return table_[count];
    }
    return log(count + k_smoothing_alpha);
  }

  LogLookup() {
    for (int count = 0; count < k_log_lookup_size; ++count) {
      table_[count] = log(count + k_smoothing_alpha);
    }
  }

private:
  float table_[k_log_lookup_size]{};
};

class LanguageIdentifier {
public:
  void ClassifyAndLearn(const int symbols[k_excerpt_length]) {
    ComputeNGramHashes(symbols);
    BuildExcerptProfile();

    const int guess = PredictLanguage();
    const int actual_language = language(guess);

    models_.RecordExcerpt(actual_language, ngram_hashes_);
  }

private:
  void ComputeNGramHashes(const int symbols[k_excerpt_length]) {
    for (int order = 0; order < k_order_count; ++order) {
      const int length = k_orders[order].length;
      for (int start = 0; start < k_orders[order].count; ++start) {
        ngram_hashes_[order][start] = HashNGram(symbols, start, length);
      }
    }
  }

  void BuildExcerptProfile() {
    for (int order = 0; order < k_order_count; ++order) {
      ++profile_generation_;
      if (profile_generation_ == 0) {
        memset(profile_stamp_, 0, sizeof profile_stamp_);
        profile_generation_ = 1;
      }

      int distinct = 0;
      for (int i = 0; i < k_orders[order].count; ++i) {
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
    const float inverse_total = 1.0f / k_orders[order].count;
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
    for (int order = 0; order < k_order_count; ++order) {
      total += CrossEntropy(language, order);
    }
    return total / k_order_count;
  }

  int PredictLanguage() const {
    int best_language = 0;
    float best_score = -1e30f;
    bool has_candidate = false;

    for (int language = 0; language < k_language_count; ++language) {
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

  int ngram_hashes_[k_order_count][k_max_n_grams_per_order]{};
  int profile_stamp_[k_hash_buckets]{};
  int profile_scratch_[k_hash_buckets]{};
  int profile_generation_ = 1;
};

LanguageIdentifier g_identifier;

} // namespace

void excerpt(int e[100]) {
  g_identifier.ClassifyAndLearn(e);
}
