#ifndef HEALTH_RULE_HPP
#define HEALTH_RULE_HPP

#include <os2dsrules.hpp>
#include <string>
#include <string_view>
#include <wordlist_rule.hpp>

using namespace OS2DSRules::WordListRule;

namespace OS2DSRules {

namespace HealthRule {

class HealthRule final : public WordListRule {
public:
  HealthRule() noexcept;
  ~HealthRule() noexcept = default;
};
} // namespace HealthRule
} // namespace OS2DSRules

#endif
