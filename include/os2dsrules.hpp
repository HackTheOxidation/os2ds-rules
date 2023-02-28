#ifndef OS2DSRULES_HPP_
#define OS2DSRULES_HPP_

#include <concepts>
#include <cstddef>
#include <string>
#include <vector>

namespace OS2DSRules {

  enum class Sensitivity {
    Critical
  };

  struct MatchResult {
    MatchResult(const std::string match,
		const size_t start,
		const size_t end,
		const Sensitivity sensitivity = Sensitivity::Critical,
		const double probability = 1.0) noexcept
      : start_(start), end_(end), match_{match}, sensitivity_(sensitivity), probability_(probability) {
    };

    size_t start() const noexcept {
      return start_;
    }

    size_t end() const noexcept {
      return end_;
    }

    const std::string match() const noexcept {
      return match_;
    }

    Sensitivity sensitivity() const noexcept {
      return sensitivity_;
    }

    double probability() const noexcept {
      return probability_;
    }

    private:
    const size_t start_;
    const size_t end_;
    const std::string match_;
    const Sensitivity sensitivity_;
    const double probability_;
  };

  using MatchResults = std::vector<MatchResult>;

};

#endif
