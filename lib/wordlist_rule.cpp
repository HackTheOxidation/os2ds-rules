#include <string_view>

#include <wordlist_rule.hpp>

namespace OS2DSRules {

namespace WordListRule {

template <typename Iter>
  requires WordIterator<Iter>
WordListRule::WordListRule(Iter begin, Iter end) noexcept {
  for (auto iter = begin; iter != end; ++iter) {
    words_.insert(*iter);
  }
}

}; // namespace WordListRule

}; // namespace OS2DSRules
