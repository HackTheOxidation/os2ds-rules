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
    constexpr size_t hash(const Key k, size_t capacity) const noexcept {
      std::hash<Key> key_hash;
      return key_hash(k) % capacity;
    }

    void rehash() noexcept {
      size_t new_capacity = capacity_ * 2;
      Record* new_records = new Record[capacity_];

      for (size_t i = 0; i < capacity_; i++) {
	Record r = records_[i];
	size_t index = hash(r.key, new_capacity);

	if (new_records[index].state == RecordState::Empty) {
	  new_records[index] = r;
	} else {
	  for (size_t j = index + 1 % new_capacity; j != index; j = j + 1 % new_capacity) {
	    if (new_records[j].state == RecordState::Empty) {
	      new_records[j] = r;
	      break;
	    }
	  }
	}
      }

      delete[] records_;

      records_ = new_records;
      capacity_ = new_capacity;
    }

    Record* records_ = nullptr;
    size_t size_;
    size_t capacity_;
    const double loadfactor_limit_ = 0.65;

    public:
    constexpr HashMap() noexcept
    : HashMap(5) {}
    constexpr HashMap(size_t capacity) noexcept
      : size_(0), capacity_(capacity) {
      records_ = new Record[capacity_];
    }

    constexpr HashMap(const HashMap& other) noexcept {
      size_ = other.size_;
      capacity_ = other.capacity_;
      records_ = new Record[capacity_];

      for (size_t i = 0; i < capacity_; i++) {
	records_[i].key = other.records_[i].key;
	records_[i].value = other.records_[i].value;
	records_[i].state = other.records_[i].state;
      }
    }

    constexpr HashMap& operator=(const HashMap& other) noexcept {
      if (records_ != nullptr)
	delete[] records_;

      size_ = other.size_;
      capacity_ = other.capacity_;
      records_ = new Record[capacity_];

      for (size_t i = 0; i < capacity_; i++) {
	records_[i].key = other.records_[i].key;
	records_[i].value = other.records_[i].value;
	records_[i].state = other.records_[i].state;
      }

      return *this;
    }

    using key_type = Key;
    using value_type = Value;

    ~HashMap() {
      if (records_ != nullptr)
	delete [] records_;
    }

    constexpr std::optional<Record> insert(const Key k, const Value v) noexcept {
      if (size_ + 1 == capacity_)
	rehash();

      auto index = hash(k, capacity_);

      if (records_[index].state == RecordState::Empty) {
	auto obj = records_[index];
	obj.key = k;
	obj.value = v;
	obj.state = RecordState::Occupied;
	return obj;
      }

      return {};
    }

    constexpr bool contains(const Key k) const noexcept {
      if (size_ == 0)
	return false;
      
      auto index = hash(k, capacity_);

      if (records_[index].key == k)
	return true;

      for (size_t i = index + 1 % capacity_; i != index; i = i + 1 % capacity_) {
	if (records_[i] == k)
	  return true;
      }

      return false;
    }

    constexpr std::optional<Record> find(const Key k) const noexcept {
      if (size_ == 0)
	return {};
      
      auto index = hash(k, capacity_);

      if (records_[index].key == k
	  && records_[index].state == RecordState::Occupied)
	return records_[index];

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
