#ifndef CPR_DETECTOR_HPP
#define CPR_DETECTOR_HPP

#include <cstddef>
#include <list>
#include <memory_resource>
#include <optional>
#include <string>


struct CPRResult {
  constexpr CPRResult(const size_t start, const size_t end) noexcept
    : start_(start), end_(end) {};
  const size_t start_;
  const size_t end_;
};

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
    Nineth,
    Match,
  };

  CPRDetectorState state_;
  bool check_mod11_;
  void reset() noexcept;
  void update() noexcept;
  void check_day_month() noexcept;

public:
  constexpr CPRDetector(bool check_mod11,
			CPRDetectorState initial_state = CPRDetectorState::Empty) noexcept
    : state_(initial_state), check_mod11_(check_mod11) {}

  CPRDetector(const CPRDetector&) = delete;
  CPRDetector& operator=(const CPRDetector&) = delete;

  constexpr CPRDetector(CPRDetector&&) noexcept;
  constexpr CPRDetector& operator=(CPRDetector&&) noexcept;
  ~CPRDetector() = default; 

  void find_matches(const std::string&) noexcept;
};

#endif
