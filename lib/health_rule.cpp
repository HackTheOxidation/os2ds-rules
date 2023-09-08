#include <algorithm>
#include <array>
#include <string_view>
#include <unordered_set>

#include <os2dsrules.hpp>
#include <health_rule.hpp>

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

HealthRule::HealthRule() noexcept {
  words_ = health_terms_set;
 }

} // namespace HealthRule
} // namespace OS2DSRules
