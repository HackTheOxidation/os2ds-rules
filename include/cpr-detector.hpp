#ifndef CPR_DETECTOR_HPP
#define CPR_DETECTOR_HPP

#include <array>
#include <concepts>
#include <cstddef>
#include <functional>
#include <iterator>
#include <os2dsrules.hpp>
#include <string>
#include <string_view>
#include <vector>

namespace OS2DSRules {

namespace CPRDetector {

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

constexpr bool is_nonzero_digit(char c) noexcept { return '0' < c && c <= '9'; }

constexpr bool is_digit(char c) noexcept { return '0' <= c && c <= '9'; }

const auto is_separator = make_predicate(' ', '-', '/');

const auto is_previous_ok = make_predicate(char(0), ' ', '\n', '\t', '\0');

constexpr bool is_space(const char c) noexcept { return c == ' '; }

const std::array<int, 10> modulus11_factors = {4, 3, 2, 7, 6, 5, 4, 3, 2, 1};

class CPRDetector {
private:
  enum class CPRDetectorState : unsigned char {
    Empty,
    First,
    Second,
    Third,
    Fourth,
    Fifth,
    Sixth,
    Seventh,
    Eighth,
    Match,
  };

  bool check_mod11_;
  bool examine_context_;
  void reset(CPRDetectorState &state) noexcept;
  char update(char, CPRDetectorState, CPRDetectorState &, Predicate) noexcept;
  bool check_day_month(const std::string &, CPRDetectorState &) noexcept;
  void check_leap_year(const std::string &, CPRDetectorState &) noexcept;
  void check_and_append_cpr(std::string &, MatchResults &, size_t,
                            size_t) noexcept;
  bool check_mod11(const MatchResult &) noexcept;
  bool examine_context(const std::string &) noexcept;

public:
  constexpr CPRDetector(bool check_mod11 = false,
                        bool examine_context = false) noexcept
      : check_mod11_(check_mod11), examine_context_(examine_context) {}

  constexpr CPRDetector(const CPRDetector &) noexcept = default;
  constexpr CPRDetector(CPRDetector &&) noexcept = default;
  constexpr CPRDetector &operator=(const CPRDetector &) noexcept = default;
  constexpr CPRDetector &operator=(CPRDetector &&) noexcept = default;
  ~CPRDetector() = default;

  MatchResults find_matches(const std::string &) noexcept;

  static const Sensitivity sensitivity = Sensitivity::Critical;
};

}; // namespace CPRDetector
}; // namespace OS2DSRules

#endif
