#include <data_structures.hpp>

#include <array>
#include <string_view>

namespace OS2DSRules {

namespace DataStructures {
using namespace std::string_view_literals;

namespace {
static constexpr auto addresses = std::to_array({
#include "datasets/da_addresses.txt"
});
}; // namespace

}; // namespace DataStructures

}; // namespace OS2DSRules
