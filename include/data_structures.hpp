#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <array>
#include <concepts>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace OS2DSRules {

namespace DataStructures {

  template<typename T>
  concept Trie = requires(T trie, std::string str, const char* cp, std::string_view strv) {
    { trie.size() } -> std::same_as<size_t>;
    { trie.contains(strv) } -> std::same_as<bool>;
    { trie.contains(str) } -> std::same_as<bool>;
    { trie.contains(cp) } -> std::same_as<bool>;
  };

  template<typename T>
  concept MutableTrie = Trie<T> && requires(T trie, std::string str) {
    trie.insert(str);
  };

  template<typename Key, typename Value>
  class HashMap {
    private:
    enum class RecordState : unsigned char {
      Empty,
      Occupied,
    };

    struct Record {
      constexpr Record() noexcept : value{0} {};
      constexpr Record(RecordState rs, Key k, Value v) noexcept
	: state(rs), key(k), value(v) {}
      RecordState state;
      Key key;
      Value value;
    };

    [[nodiscard]]
    constexpr size_t hash(const Key k) const noexcept {
      return k % capacity_;
    }

    void rehash() noexcept {
    }

    Record* records_ = nullptr;
    size_t size_;
    size_t capacity_;
    const double loadfactor_limit_ = 0.65;

    public:
    constexpr HashMap() noexcept
    : size_(0), capacity_(5) {
      records_ = new Record[capacity_];
    }

    using key_type = Key;
    using value_type = Value;

    ~HashMap() {
      if (records_ != nullptr)
	delete [] records_;
    }

    constexpr std::optional<Record> insert(const Key, const Value) noexcept {

      return {};
    }

    constexpr bool contains(const Key) const noexcept {

      return false;
    }

    constexpr std::optional<Record> find(const Key) const noexcept {

      return {};
    } 

  };

  class ReadOnlyTrie {
    public:
    template<typename Collection>
    ReadOnlyTrie(Collection collection) noexcept {
      
    }

    bool contains(std::string str) const noexcept {
      auto begin = str.cbegin();
      auto end = str.cend();

      char ch = *begin;

      auto found = nodes_.find(ch);

      if (found != std::nullopt) {
	return found->value.contains(begin, end);
      }

      return false;
    }

    private:
    class Node {
      public:
      constexpr Node(char value, bool completes_word = false) noexcept
	: value_(value), completes_word_(completes_word) {}

      constexpr char value() const noexcept {
	return value_;
      }

      constexpr bool completes_word() const noexcept {
	return completes_word_;
      }

      constexpr void set_completes_word(bool b) noexcept {
	completes_word_ = b;
      }

      [[nodiscard]]
      bool contains(std::string::const_iterator it, std::string::const_iterator) const noexcept {
	char ch = *it;

	if (ch == value_) {
	}

	return false;
      }
 
      void insert(std::string::const_iterator it, std::string::const_iterator end) noexcept {
	char ch = *it;

	Node n(ch);
	auto next = nodes_.insert(ch, n);

	++it;

	if (next.has_value()) {
	  if (it != end) {
	    next->value.insert(it, end);
	  } else {
	    next->value.set_completes_word(true);
	  }
	}
      }

      private:
      char value_;
      bool completes_word_;
      HashMap<char, Node> nodes_;
    };

    HashMap<char, Node> nodes_;

    void insert(const std::string str) {
      std::string::const_iterator cit = str.cbegin();
      std::string::const_iterator cend = str.cend();

      char ch = *cit;

      auto next = nodes_.find(ch);

      if (next == std::nullopt) {
	Node n(ch);
	n.insert(++cit, cend);
	nodes_.insert(ch, n);
      } else {
	next->value.insert(cit, cend);
      }
    }
  };

}; // DataStructures

}; // OS2DSRules

#endif
