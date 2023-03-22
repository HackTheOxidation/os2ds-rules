#ifndef OS2DSRULES_HPP_
#define OS2DSRULES_HPP_

#include "cpr-detector.hpp"
#include <concepts>
#include <cstddef>
#include <string>
#include <type_traits>
#include <vector>

namespace OS2DSRules {

enum class Sensitivity {
  Information = 0,
  Notice = 250,
  Warning = 500,
  Problem = 750,
  Critical = 1000,
};

struct MatchResult {
  MatchResult(const std::string match, const size_t start, const size_t end,
              const Sensitivity sensitivity = Sensitivity::Critical,
              const double probability = 1.0) noexcept
      : start_(start), end_(end), match_{match}, sensitivity_(sensitivity),
        probability_(probability){};

  [[nodiscard]] size_t start() const noexcept { return start_; }

  [[nodiscard]] size_t end() const noexcept { return end_; }

  [[nodiscard]] const std::string match() const noexcept { return match_; }

  [[nodiscard]] Sensitivity sensitivity() const noexcept {
    return sensitivity_;
  }

  [[nodiscard]] double probability() const noexcept { return probability_; }

private:
  const size_t start_;
  const size_t end_;
  const std::string match_;
  const Sensitivity sensitivity_;
  const double probability_;
};

using MatchResults = std::vector<MatchResult>;

template <typename Rule>
concept ScannerRule = requires(Rule rule, std::string s) {
                        { Rule::sensitivity } -> std::same_as<Sensitivity>;
                        { rule.find_matches(s) } -> std::same_as<MatchResults>;
                      };

}; // namespace OS2DSRules

#endif
