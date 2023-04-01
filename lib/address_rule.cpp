#include <algorithm>
#include <array>
#include <iostream>
#include <optional>
#include <os2dsrules.hpp>
#include <string>
#include <string_view>

#include <address_rule.hpp>
#include <data_structures.hpp>

using namespace OS2DSRules::DataStructures;

namespace OS2DSRules {

namespace AddressRule {
using namespace std::string_view_literals;

namespace {
static constexpr auto addresses = std::to_array({
#include "datasets/da_addresses.txt"
});
static const auto addresses_set = FrozenHashSet(addresses);
}; // namespace

[[nodiscard]] MatchResults
AddressRule::find_matches(const std::string &content) const noexcept {
  MatchResults results;

  auto is_end_of_word = [](char c) { return c == ' '; };

  bool in_word = false;
  std::size_t counter = 0;
  std::size_t word_begin, word_end = counter;
  std::string address = "";

  for (char ch : content) {
    if (std::isupper(ch) && !in_word) {
      word_begin = counter;
      address = "";
      in_word = true;
    }

    if (in_word) {
      if (is_end_of_word(ch)) {
        word_end = counter;

        if (contains(address.begin(), address.end())) {
          results.push_back(MatchResult(address, word_begin, word_end));
        }

        in_word = false;
      } else {
        address += ch;
      }
    }

    ++counter;
  }

  return filter_matches(results, content);
  //return results;
}

[[nodiscard]] MatchResults
AddressRule::filter_matches(const MatchResults &matches,
                            const std::string &content) const noexcept {
  MatchResults results;

  for (auto m : matches) {
    auto address_opt = append_number(m, content);
    if (address_opt) {
      results.push_back(address_opt.value());
    }
  }

  return results;
}

[[nodiscard]] bool
AddressRule::contains(const std::string_view target) const noexcept {
  return addresses_set.contains(target);
}

[[nodiscard]] bool
AddressRule::contains(const std::string target) const noexcept {
  return contains(std::string_view(target.cbegin(), target.cend()));
}

[[nodiscard]] bool
AddressRule::contains(const std::string::const_iterator start,
                      const std::string::const_iterator stop) const noexcept {
  return contains(std::string_view(start, stop));
}

[[nodiscard]] std::optional<MatchResult>
AddressRule::append_number(const MatchResult &m,
                           const std::string &content) const noexcept {
  if (content.size() == m.end() + 1)
    return {};

  if (content[m.end()] != ' ')
    return {};

  auto is_digit = [](char c) { return '0' <= c && c <= '9'; };
  auto is_nonzero_digit = [](char c) { return '0' < c && c <= '9'; };

  auto iter = content.begin() + m.end() + 1;
  std::cout << "Iter: " << *iter << '\n';

  if (!is_nonzero_digit(*iter))
    return {};

  std::string number = "";
  number += *(iter++);
  std::size_t counter = 1;

  while (iter != content.end() && is_digit(*iter)) {
    std::cout << "Iter: " << *iter << '\n';
    number += *(iter++);
    ++counter;
  }

  std::string match_string = m.match() + " " + number;
  return MatchResult(match_string, m.start(), m.end() + counter);
}

}; // namespace AddressRule

}; // namespace OS2DSRules