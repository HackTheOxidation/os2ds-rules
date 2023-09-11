#ifndef WORDLIST_RULE_HPP
#define WORDLIST_RULE_HPP

#include <cmath>
#include <concepts>
#include <cstddef>
#include <os2dsrules.hpp>
#include <string>
#include <string_view>
#include <unordered_set>

namespace OS2DSRules {

namespace WordListRule {

using Words = std::unordered_set<std::string_view>;

class WordListRule {
public:
  template <typename Iter>
    requires WordIterator<Iter>
  WordListRule(Iter begin, Iter end) noexcept {
    for (auto iter = begin; iter != end; ++iter) {
      words_.insert(*iter);
    }
  }
  WordListRule(Words words) noexcept : words_(words) {}
  WordListRule() noexcept = default;
  WordListRule(const WordListRule &) noexcept = default;
  WordListRule(WordListRule &&) noexcept = default;
  ~WordListRule() noexcept = default;

  [[nodiscard]] MatchResults find_matches(const std::string &) const noexcept;

protected:
  Words words_;
  
private:
  [[nodiscard]] bool contains(const std::string_view) const noexcept;
  [[nodiscard]] bool contains(const std::string) const noexcept;
  [[nodiscard]] bool contains(const std::string::const_iterator,
                              const std::string::const_iterator) const noexcept;
  void check_match(MatchResults &, const std::string, const std::size_t,
                   const std::size_t) const noexcept;
};
}; // namespace WordListRule

}; // namespace OS2DSRules

#endif
