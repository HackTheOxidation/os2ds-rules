#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <array>
#include <memory>
#include <string_view>

namespace OS2DSRules {

namespace DataStructures {

class Names {
    private:
    static constexpr std::string_view male_firstnames_[] = {
        #include "datasets/male_firstnames.txt"
    };
    static constexpr std::string_view female_firstnames_[] = {
        #include "datasets/female_firstnames.txt"
    };
    static constexpr std::string_view lastnames_[] = {
        #include "datasets/lastnames.txt"
    };
    static constexpr std::string_view addresses_[] = {
        #include "datasets/da_addresses.txt"
    };

    public:
    static constexpr auto male_firstnames = std::to_array(std::move(male_firstnames_));
    static constexpr auto female_firstnames = std::to_array(std::move(female_firstnames_));
    static constexpr auto lastnames = std::to_array(std::move(lastnames_));
    static constexpr auto addresses = std::to_array(std::move(addresses_));
    Names() = delete;
};

class Trie;

};

};

#endif
