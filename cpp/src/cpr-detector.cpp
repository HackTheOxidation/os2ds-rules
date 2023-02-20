#include <cpr-detector.hpp>
#include <cstddef>
#include <string>


constexpr CPRDetector::CPRDetector(CPRDetector&& other) noexcept
  : state_(std::move(other.state_)), check_mod11_(std::move(other.check_mod11_)) {}

constexpr CPRDetector& CPRDetector::operator=(CPRDetector&& other) noexcept {
  if (this != &other) {
    std::swap(other.check_mod11_, check_mod11_);
    state_ = std::move(other.state_);
  }

  return *this;
}

void CPRDetector::find_matches(const std::string& content) noexcept {
  auto content_length = content.size();
  if (content_length < 10) {
    return;
  }

  // TODO: find a reasonable data type to store a CPR-number.
  char cpr[10] = {0};
  char* previous = nullptr;
  size_t begin = 0;
  bool allow_separator = false;

  for(auto it = std::begin(content); it != std::end(content); ++it) {
    switch (state_) {
    case CPRDetectorState::Empty:
      break;
    default:
      break;
    }
  }
}
