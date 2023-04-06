#ifndef WORDLIST_RULE_HPP
#define WORDLIST_RULE_HPP

#include <concepts>
#include <os2dsrules.hpp>
#include <string>
#include <string_view>
#include <unordered_set>

namespace OS2DSRules {

namespace WordListRule {

class WordListRule {
public:
  template <typename Iter>
    requires WordIterator<Iter>
  WordListRule(Iter begin, Iter end) noexcept;
  WordListRule() noexcept = default;
  WordListRule(const WordListRule &) noexcept = default;
  WordListRule(WordListRule &&) noexcept = default;
  ~WordListRule() noexcept = default;

  [[nodiscard]] MatchResults find_matches(const std::string &) const noexcept;

private:
  const std::unordered_set<std::string_view> words_;
  [[nodiscard]] bool contains(const std::string_view) const noexcept;
  [[nodiscard]] bool contains(const std::string) const noexcept;
  [[nodiscard]] bool contains(const std::string::const_iterator,
                              const std::string::const_iterator) const noexcept;
};
}; // namespace WordListRule

}; // namespace OS2DSRules

#endif
