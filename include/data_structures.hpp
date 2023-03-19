#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <array>
#include <concepts>
#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>

namespace OS2DSRules {

namespace DataStructures {

  template<typename T>
  concept Trie = requires(T trie, std::string str, const char* cp, std::string_view strv) {
    { trie.size() } -> std::same_as<size_t>;
    { trie.contains(strv) } -> std::same_as<bool>;
    { trie.contains(str) } -> std::same_as<bool>;
    { trie.contains(cp) } -> std::same_as<bool>;
  };

  template<typename T>
  concept MutableTrie = Trie<T> && requires(T trie, std::string str) {
    trie.insert(str);
  };

  class ReadOnlyTrie {
    public:
    constexpr ReadOnlyTrie() noexcept;
    constexpr size_t size() const noexcept {
      return size_;
    };

    private:
    const size_t size_;
  };

}; // DataStructures

}; // OS2DSRules

#endif
