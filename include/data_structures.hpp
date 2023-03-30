#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <algorithm>
#include <concepts>
#include <cstddef>
#include <functional>
#include <memory>
#include <new>
#include <numeric>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace OS2DSRules {

namespace DataStructures {

/*
  Concept for expressing the requirements for a WordCollection.
  A WordCollection is an immutable data structure that contains
  a collection of words (string-like) that can be queried about
  whether it contains a certain word.
 */
template <typename Collection>
concept WordCollection = requires(Collection collection, std::string str,
                                  const char *cp, std::string_view strv) {
                           { collection.size() } -> std::same_as<size_t>;
                           { collection.contains(strv) } -> std::same_as<bool>;
                           { collection.contains(str) } -> std::same_as<bool>;
                           { collection.contains(cp) } -> std::same_as<bool>;
                         };

/*
  Concept for expressing the requirements for a MutableWordCollection.
  Similar to a WordCollection, but this one is mutable and can thus be
  altered after its construction.
 */
template <typename Collection>
concept MutableWordCollection =
    WordCollection<Collection> &&
    requires(Collection collection, std::string str) {
      collection.insert(str);
      collection.remove(str);
    };

/*
  A simple concept that requires that a type implements a hashing function.
 */
template <typename T>
concept is_hashable = requires(T t) {
                        { std::hash<T>()(t) };
                      };

/*
  Compile-time capable, immutable hashset.
 */
template <std::size_t Size>
class FrozenHashSet {
private:
  class Chain {
  public:
    constexpr Chain() noexcept : value_{""}, next_{} {}
    constexpr Chain(std::string_view value) noexcept : value_(value), next_{} {}
    constexpr ~Chain() noexcept {}

    Chain(Chain && other) noexcept
      : value_(std::move(other.value_)), next_(std::move(other.next_)){}
    Chain& operator=(Chain &&other) noexcept {
      if (this != &other) {
	value_ = other.value_;
	next_ = std::move(other.next_);
      }

      return *this;
    }

    void append(const std::string_view value) noexcept {
      if (next_ == nullptr) {
        next_ = std::unique_ptr<Chain>(new Chain(value));
      } else {
        next_->append(value);
      }
    }

    constexpr bool contains(const std::string_view value) const noexcept {
      if (value_ == value)
        return true;

      if (next_ != nullptr) {
        return next_->contains(value);
      } else {
        return false;
      }
    }

    constexpr std::string_view value() const noexcept { return value_; }

    constexpr std::size_t length() const noexcept {
      if (next_) {
	return 1 + next_->length();
      }

      return 1;
    }

  private:
    std::string_view value_;
    std::unique_ptr<Chain> next_;
  };

  std::array<Chain, Size> container_;

  auto get_hash(const std::string_view value) const noexcept {
    std::hash<std::string_view> hash_fun;
    return hash_fun(value) % Size;
  }

  void insert(const std::string_view value) noexcept {
    auto index = get_hash(value);
    auto &chain = container_[index];

    if (!chain.contains(value)) {
      if (chain.value() == "") {
        chain = Chain(value);
      } else {
        chain.append(value);
      }
    }
  }

public:
  constexpr FrozenHashSet() noexcept {};
  FrozenHashSet(std::array<const char*, Size> initializer) noexcept {
    for (const char* value : initializer) {
      insert(std::string_view(value));
    }
  }
  FrozenHashSet(std::array<std::string_view, Size> initializer) noexcept {
    for (auto value : initializer) {
      insert(value);
    }
  }
  constexpr ~FrozenHashSet() noexcept {};

  [[nodiscard]]
  bool contains(const std::string_view value) const noexcept {
    auto index = get_hash(value);
    return container_[index].contains(value);
  }
};

}; // namespace DataStructures

}; // namespace OS2DSRules

#endif
