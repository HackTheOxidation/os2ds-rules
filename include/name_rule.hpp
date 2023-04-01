#ifndef NAME_RULE_HPP
#define NAME_RULE_HPP

#include <os2dsrules.hpp>
#include <string>
#include <string_view>

#include "os2dsrules.hpp"

namespace OS2DSRules {

namespace NameRule {

class NameRule {
public:
  constexpr NameRule(const bool expansive = false) noexcept
      : expansive_(expansive) {}
  constexpr NameRule(const NameRule &other) noexcept = default;

  [[nodiscard]] MatchResults find_matches(const std::string &) const noexcept;

private:
  const bool expansive_;
  [[nodiscard]] bool contains(const std::string_view) const noexcept;
  [[nodiscard]] bool contains(const std::string) const noexcept;
  [[nodiscard]] bool contains(const std::string::const_iterator,
                              const std::string::const_iterator) const noexcept;
  [[nodiscard]] MatchResults filter_matches(const MatchResults &matches) const noexcept;
};

}; // namespace NameRule

}; // namespace OS2DSRules

#endif
