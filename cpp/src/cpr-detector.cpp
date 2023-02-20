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

std::optional<CPRResult> search(const std::string text) noexcept {
  if (text.size() < 10)
    return std::nullopt;

  auto cursor = text.begin();
  auto stop = text.end();

  return std::nullopt;
}

void CPRDetector::read_day(std::string::iterator& cursor,
			   std::string::iterator& stop) noexcept {
  
}

void CPRDetector::read_month(std::string::iterator& cursor,
			     std::string::iterator& stop) noexcept {

}

void CPRDetector::read_year(std::string::iterator& cursor,
			    std::string::iterator& stop) noexcept {

}

