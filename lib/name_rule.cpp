#include <algorithm>
#include <array>
#include <string_view>

#include <name_rule.hpp>

namespace OS2DSRules {

namespace NameRule {
using namespace std::string_view_literals;

namespace {
static constexpr auto firstnames = std::to_array({
#include "datasets/female_firstnames.txt"
#include "datasets/male_firstnames.txt"
});

static constexpr auto lastnames = std::to_array({
#include "datasets/lastnames.txt"
});
}; // namespace

MatchResults NameRule::find_matches(const std::string &content) const noexcept {
  MatchResults results;

  auto is_end_of_word = [](char c) {
    return c == '.' || c == ' ' || c == '\n';
  };

  auto word_begin = content.cbegin();

  for (auto iter = content.cbegin(); iter != content.cend(); ++iter) {
    if (std::isupper(*iter)) {
      word_begin = iter;
    } else if (is_end_of_word(*iter)) {
      auto word_end = iter - 1;

      if (contains(word_begin, word_end)) {
        MatchResult result(std::string(word_begin, word_end),
                           std::distance(content.cbegin(), word_begin),
                           std::distance(content.begin(), word_end));

        results.push_back(result);
      }
    }
  }

  return results;
}

constexpr bool
NameRule::contains(const std::string_view target) const noexcept {
  auto eq = [target](auto sv) {
    return std::equal(
        target.begin(), target.end(), sv.begin(), sv.end(),
        [](char a, char b) { return std::toupper(a) == std::toupper(b); });
  };

  for (std::string_view sv : firstnames) {
    if (eq(sv)) {
      return true;
    }
  }

  for (std::string_view sv : lastnames) {
    if (eq(sv)) {
      return true;
    }
  }

  return false;
}

bool NameRule::contains(const std::string target) const noexcept {
  return contains(std::string_view(target.cbegin(), target.cend()));
}

bool NameRule::contains(const std::string::const_iterator start,
                        const std::string::const_iterator stop) const noexcept {
  return contains(std::string_view(start, stop));
}

}; // namespace NameRule

}; // namespace OS2DSRules
