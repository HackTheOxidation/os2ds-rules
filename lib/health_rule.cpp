#include <algorithm>
#include <array>
#include <string_view>
#include <unordered_set>

#include <health_rule.hpp>
#include <os2dsrules.hpp>

using namespace OS2DSRules::WordListRule;

namespace OS2DSRules {

namespace HealthRule {
using namespace std::string_view_literals;

namespace {
static constexpr auto health_terms = std::to_array({
#include "datasets/health_terms.txt"
});

static const auto health_terms_set = std::unordered_set<std::string_view>(
    health_terms.cbegin(), health_terms.cend());
} // namespace

HealthRule::HealthRule() noexcept : rule_(health_terms_set) {}

[[nodiscard]] MatchResults
HealthRule::find_matches(const std::string &content) const noexcept {
  return rule_.find_matches(content);
}

} // namespace HealthRule
} // namespace OS2DSRules
