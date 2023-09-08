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
concept WordCollection =
    requires(Collection collection, std::string_view strv) {
      { collection.size() } -> std::same_as<size_t>;
      { collection.contains(strv) } -> std::same_as<bool>;
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

class AbstractHashSet {
protected:
  class Chain {
  public:
    constexpr Chain() noexcept = default;
    constexpr Chain(std::size_t hash) noexcept : hash_(hash) {}

    Chain(Chain &&) noexcept = default;
    Chain &operator=(Chain &&) noexcept = default;
    ~Chain() noexcept = default;

    void append(const std::size_t hash) noexcept {
      if (next_ == nullptr) {
        next_ = std::make_unique<Chain>(hash);
      } else {
        next_->append(hash);
      }
    }

    void remove(const std::size_t hash, Chain *prev = nullptr) noexcept {
      if (hash_ == hash) {
        if (prev && prev != this) {
          prev->next_.swap(next_);
          next_.release();
        }
      } else {
        if (next_) {
          next_->remove(hash, this);
        }
      }
    }

    [[nodiscard]] constexpr bool
    contains(const std::size_t hash) const noexcept {
      if (hash_ == hash)
        return true;

      if (next_ != nullptr) {
        return next_->contains(hash);
      } else {
        return false;
      }
    }

    [[nodiscard]] constexpr std::size_t hash() const noexcept { return hash_; }

    [[nodiscard]] std::size_t length() const noexcept {
      if (next_) {
        return 1 + next_->length();
      }

      return 1;
    }

  private:
    std::size_t hash_;
    std::unique_ptr<Chain> next_;
  };
};

/*
  Immutable hash table.
 */
template <std::size_t Size> class FrozenHashSet : public AbstractHashSet {
private:
  std::array<Chain, Size> container_;

  auto get_hash(const std::string_view value) const noexcept {
    std::hash<std::string_view> hash_fun;
    return hash_fun(value);
  }

  void insert(const std::string_view value) noexcept {
    auto hash = get_hash(value);
    auto index = hash % Size;
    auto &chain = container_[index];

    if (!chain.contains(hash)) {
      if (chain.hash() == 0) {
        chain = Chain(hash);
      } else {
        chain.append(hash);
      }
    }
  }

public:
  FrozenHashSet() noexcept = delete;

  FrozenHashSet(std::array<const char *, Size> initializer) noexcept {
    for (const char *value : initializer) {
      insert(std::string_view(value));
    }
  }

  FrozenHashSet(std::array<std::string_view, Size> initializer) noexcept {
    for (auto value : initializer) {
      insert(value);
    }
  }

  FrozenHashSet(FrozenHashSet &&) noexcept = default;
  FrozenHashSet &operator=(FrozenHashSet &&) noexcept = default;
  ~FrozenHashSet() noexcept = default;

  [[nodiscard]] bool contains(const std::string_view value) const noexcept {
    auto hash = get_hash(value);
    auto index = hash % Size;
    return container_[index].contains(hash);
  }
};

  /*
    This is a Suffix Tree for holding and searching for multiple strings
    of varying length.
   */
  class SuffixTree {
    public:
    constexpr SuffixTree() noexcept = default;
    constexpr SuffixTree(const SuffixTree &) noexcept = default;
    constexpr SuffixTree(SuffixTree &&) noexcept = default;
    constexpr SuffixTree &operator=(const SuffixTree &) noexcept = default;
    constexpr SuffixTree &operator=(SuffixTree &&) noexcept = default;

    constexpr std::size_t size() const noexcept;
    
    void insert(const std::string &) noexcept;
    void remove(const std::string &) noexcept;

  private:
    std::size_t size_ = 0;

  };

}; // namespace DataStructures

}; // namespace OS2DSRules

#endif
