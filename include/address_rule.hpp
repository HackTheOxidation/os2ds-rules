#ifndef ADDRESS_RULE_HPP
#define ADDRESS_RULE_HPP

#include <optional>
#include <os2dsrules.hpp>
#include <string>
#include <string_view>

namespace OS2DSRules {

namespace AddressRule {

class AddressRule {
public:
  constexpr AddressRule() noexcept = default;
  constexpr AddressRule(const AddressRule &) noexcept = default;
  constexpr AddressRule(AddressRule &&) noexcept = default;
  constexpr AddressRule &operator=(const AddressRule &) noexcept = default;
  constexpr AddressRule &operator=(AddressRule &&) noexcept = default;

  [[nodiscard]] MatchResults find_matches(const std::string &) const noexcept;

private:
  [[nodiscard]] bool contains(const std::string_view) const noexcept;
  [[nodiscard]] bool contains(const std::string) const noexcept;
  [[nodiscard]] bool contains(const std::string::const_iterator,
                              const std::string::const_iterator) const noexcept;
  [[nodiscard]] MatchResults filter_matches(const MatchResults &matches,
                                            const std::string &) const noexcept;
  [[nodiscard]] std::optional<MatchResult>
  append_number(const MatchResult &, const std::string &) const noexcept;
};
}; // namespace AddressRule

}; // namespace OS2DSRules

#endif
