#ifndef HEALTH_RULE_HPP
#define HEALTH_RULE_HPP

#include <os2dsrules.hpp>
#include <string>
#include <string_view>
#include <wordlist_rule.hpp>

namespace OS2DSRules {

namespace HealthRule {

class HealthRule {
public:
  HealthRule() noexcept;
  HealthRule(const HealthRule &) noexcept = default;
  HealthRule(HealthRule &&) noexcept = default;
  ~HealthRule() noexcept = default;

  [[nodiscard]] MatchResults find_matches(const std::string &) const noexcept;

private:
  OS2DSRules::WordListRule::WordListRule rule_;
};
} // namespace HealthRule
} // namespace OS2DSRules

#endif
