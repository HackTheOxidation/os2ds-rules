#ifndef CPR_DETECTOR_HPP
#define CPR_DETECTOR_HPP

#include <cstddef>
#include <list>
#include <memory_resource>
#include <optional>
#include <string>

enum class CPRDetectorState : unsigned char {
  Empty,
  Day,
  Month,
  Year,
  Complete,
  Control,
};

struct CPRResult {
  constexpr CPRResult(const size_t start, const size_t end) noexcept
    : start_(start), end_(end) {};
  const size_t start_;
  const size_t end_;
};

class CPRDetector {
public:
  constexpr CPRDetector(bool check_mod11,
			CPRDetectorState initial_state = CPRDetectorState::Empty) noexcept
    : state_(initial_state), check_mod11_(check_mod11) {}

  CPRDetector(const CPRDetector&) = delete;
  CPRDetector& operator=(const CPRDetector&) = delete;

  constexpr CPRDetector(CPRDetector&&) noexcept;
  constexpr CPRDetector& operator=(CPRDetector&&) noexcept;
  ~CPRDetector() = default; 

  std::optional<CPRResult> search(const std::string&) noexcept;

private:
  CPRDetectorState state_;
  bool check_mod11_;
  void reset();
  void read_day(std::string::iterator&, std::string::iterator&) noexcept;
  void read_month(std::string::iterator&, std::string::iterator&) noexcept;
  void read_year(std::string::iterator&, std::string::iterator) noexcept;
  void clear() noexcept;
};

#endif
