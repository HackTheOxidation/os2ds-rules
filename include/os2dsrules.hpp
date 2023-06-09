#ifndef OS2DSRULES_HPP_
#define OS2DSRULES_HPP_

#include <concepts>
#include <cstddef>
#include <functional>
#include <string>
#include <type_traits>
#include <vector>

namespace OS2DSRules {

using Predicate = std::function<bool(char)>;

template <typename T>
  requires std::same_as<T, char>
constexpr auto make_predicate(T d) noexcept {
  return [d](T c) { return c == d; };
}

template <typename T, typename... Args>
constexpr auto make_predicate(T d, Args... ds) noexcept {
  if constexpr (sizeof...(ds) > 0) {
    return [d, ds...](T c) {
      return make_predicate(d)(c) || make_predicate(ds...)(c);
    };
  } else
    return make_predicate(d);
}

enum class Sensitivity {
  Information = 0,
  Notice = 250,
  Warning = 500,
  Problem = 750,
  Critical = 1000,
};

struct MatchResult {
  MatchResult(std::string match, size_t start, size_t end,
              Sensitivity sensitivity = Sensitivity::Critical,
              double probability = 1.0) noexcept
      : start_(start), end_(end), match_(match), sensitivity_(sensitivity),
        probability_(probability){};
  MatchResult(const MatchResult &) noexcept = default;
  MatchResult(MatchResult &&) noexcept = default;
  MatchResult &operator=(const MatchResult &) noexcept = default;
  MatchResult &operator=(MatchResult &&) noexcept = default;
  bool operator==(const MatchResult &) const noexcept = default;

  [[nodiscard]] bool is_next_to(const MatchResult &other) const noexcept {
    return (other.start_ - end_) == 1;
  }

  [[nodiscard]] bool is_after(const MatchResult &other) const noexcept {
    return (start_ - other.end_) == 1;
  }

  [[nodiscard]] std::size_t distance(const MatchResult &other) const noexcept {
    if (end_ < other.start_) {
      return other.start_ - end_;
    } else {
      return start_ - other.end_;
    }
  }

  [[nodiscard]] size_t start() const noexcept { return start_; }

  [[nodiscard]] size_t end() const noexcept { return end_; }

  [[nodiscard]] const std::string match() const noexcept { return match_; }

  [[nodiscard]] Sensitivity sensitivity() const noexcept {
    return sensitivity_;
  }

  [[nodiscard]] double probability() const noexcept { return probability_; }

private:
  size_t start_;
  size_t end_;
  std::string match_;
  Sensitivity sensitivity_;
  double probability_;
};

using MatchResults = std::vector<MatchResult>;

// Concept behind a scanner rule.
template <typename Rule>
concept ScannerRule = requires(Rule rule, std::string s) {
  { Rule::sensitivity } -> std::same_as<Sensitivity>;
  { rule.find_matches(s) } -> std::same_as<MatchResults>;
};

// An WordIterator is any iterator that has value_type = std::string_view.
template <typename Iter>
concept WordIterator = requires(Iter iter) {
  { ++iter };
  { iter++ };
  { *iter } -> std::convertible_to<std::string_view>;
};

}; // namespace OS2DSRules

#endif
