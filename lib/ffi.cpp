#include <cpr-detector.hpp>
#include <iostream>

extern "C" {

  void find_matches(const char* content) {
    std::cout << "Called find_matches with content: " << content << '\n';
  }
};
