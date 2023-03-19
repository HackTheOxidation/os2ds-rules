#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <array>
#include <cstddef>
#include <string_view>

namespace OS2DSRules {

namespace DataStructures {

  class Trie {
    public:
    Trie() noexcept;
    size_t size() const noexcept;

    private:
    size_t size_;
  };

}; // DataStructures

}; // OS2DSRules

#endif
