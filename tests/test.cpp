#include <cpr-detector.hpp>
#include <exception>
#include <string>
#include <iostream>


int main(void) {
  std::string content = "1111111118 2304516782";
  CPRDetector::CPRDetector detector(false);

  auto result = detector.find_matches(content);

  std::cout << "Found results: " << result.size() << '\n';
  for (auto res : result) {
    std::cout << res.cpr_ << '\n';
  }
}
