#ifndef CPR_DETECTOR_HPP
#define CPR_DETECTOR_HPP

#include <concepts>
#include <cstddef>
#include <functional>
#include <string>
#include <vector>

namespace CPRDetector {

  using Predicate = std::function<bool(char)>;

  template<typename T>
  requires std::same_as<T, char>
  constexpr Predicate make_predicate(T d) noexcept {
    return [d](T c) { return c == d; };
  }

  template<typename T, typename... Args>
  constexpr Predicate make_predicate(T d, Args... ds) noexcept {
    if constexpr(sizeof...(ds) > 0) {
      return [d, ds...](T c) {
	return make_predicate(d)(c) || make_predicate(ds...)(c);
      };
    } else
      return make_predicate(d);
  }

  constexpr bool is_nonzero_digit(char c) noexcept {
    return '0' < c && c <= '9';
  }

  constexpr bool is_digit(char c) noexcept {
    return '0' <= c && c <= '9';
  }

  const auto is_separator = make_predicate(' ', '-', '/');

  const auto is_previous_ok = make_predicate(char(0), ' ', '\n', '\t', '\0');

  constexpr bool is_space(const char c) noexcept {
    return c == ' ';
  }

  struct CPRResult {
    CPRResult(const std::string cpr, const size_t start, const size_t end) noexcept
      : start_(start), end_(end), cpr_{cpr} {
    };
    const size_t start_;
    const size_t end_;
    const std::string cpr_;
  };

  using CPRResults = std::vector<CPRResult>;

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

    CPRDetectorState state_;
    bool check_mod11_;
    void reset() noexcept;
    char update(char c, CPRDetectorState new_state, Predicate is_acceptable) noexcept;
    bool check_day_month(const std::string&) noexcept;
    void check_leap_year(const std::string&) noexcept;
    void check_and_append_cpr(std::string&, CPRResults&, size_t, size_t) noexcept;
    void search(CPRResults&, char*, std::string&, char&, size_t&, bool&, bool&, Predicate&) noexcept;

  public:
    constexpr CPRDetector(bool check_mod11,
			  CPRDetectorState initial_state = CPRDetectorState::Empty) noexcept
      : state_(initial_state), check_mod11_(check_mod11) {}

    CPRDetector(const CPRDetector&) = delete;
    CPRDetector& operator=(const CPRDetector&) = delete;

    constexpr CPRDetector(CPRDetector&&) noexcept;
    constexpr CPRDetector& operator=(CPRDetector&&) noexcept;
    ~CPRDetector() = default; 

    [[maybe_unused]]
    CPRResults find_matches(const std::string&) noexcept;
    [[maybe_unused]]
    CPRResults find_matches(const char*, size_t) noexcept;
  };
};

#endif
