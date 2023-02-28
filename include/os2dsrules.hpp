#ifndef OS2DSRULES_HPP_
#define OS2DSRULES_HPP_

#include <concepts>
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
		const Sensitivity sensitivity = Sensitivity::Critical) noexcept
      : start_(start), end_(end), match_{match}, sensitivity_(sensitivity) {
    };
    const size_t start_;
    const size_t end_;
    const std::string match_;
    const Sensitivity sensitivity_;
  };

  using MatchResults = std::vector<MatchResult>;

};

#endif
