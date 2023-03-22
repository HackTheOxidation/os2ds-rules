#ifndef DATA_STRUCTURES_HPP
#define DATA_STRUCTURES_HPP

#include <concepts>
#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

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
  A constexpr-capable HashMap (hash table) data structure that is similar to
  std::unordered_map. It stores key and value pairs called Records. Each
  Record also has a state associated with it that indicates whether or not
  a Record slot is empty or occupied. Every operation can be evaluated at
  compile-time.
 */
template <typename Key, typename Value>
  requires is_hashable<Key>
class HashMap {
private:
  /*
    Internal enum for representing the state of a
    Record. It can be either empty or occupied.
   */
  enum class RecordState : unsigned char {
    Empty,
    Occupied,
  };

  /*
    Internal struct for containing key-value pairs that is constexpr-capable.
    It also has a state associated with it to indicate whether the key-value
    pair is in use or has been "deleted".
   */
  struct Record {
    constexpr Record() noexcept
        : state(RecordState::Empty), key{0}, value{0} {};
    constexpr Record(RecordState rs, Key k, Value v) noexcept
        : state(rs), key(k), value(v) {}

    constexpr Record(const Record &other) noexcept
        : state(other.state), key(other.key), value(other.value) {}

    constexpr Record &operator=(const Record &other) noexcept {
      key = other.key;
      value = other.value;
      state = other.state;

      return *this;
    }

    RecordState state;
    Key key;
    Value value;
  };

  /*
    Helper function that calculates and returns the hash of a key modulus
    some capacity such that the returned hash is always a valid index.
   */
  [[nodiscard]] constexpr size_t get_hash(const Key k,
                                          size_t capacity) const noexcept {
    return std::hash<Key>()(k) % capacity;
  }

  /*
    Helper function that calculates and returns the load factor of the
    current state of the HashMap. The load factor is given by the
    ratio of elements in use vs. the allocated capacity.
   */
  [[nodiscard]] constexpr double calculate_loadfactor() const noexcept {
    return double(size_) / double(capacity_);
  }

  /*
    Doubles the capacity of the current HashMap and recalculates the hash
    values for each record in the HashMap, which is moved to the newly
    allocated memory.
   */
  void rehash() noexcept {
    size_t new_capacity = capacity_ * 2;
    Record *new_records = new Record[capacity_];

    for (size_t i = 0; i < capacity_; i++) {
      Record r = records_[i];
      size_t index = get_hash(r.key, new_capacity);

      if (new_records[index].state == RecordState::Empty) {
        new_records[index] = r;
      } else {
        for (size_t j = index + 1 % new_capacity; j != index;
             j = j + 1 % new_capacity) {
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

  Record *records_ = nullptr;
  size_t size_;
  size_t capacity_;
  const double loadfactor_limit_ = 0.65;

public:
  using key_type = Key;
  using value_type = Value;

  constexpr HashMap() noexcept : HashMap(5) {}
  constexpr HashMap(size_t capacity) noexcept : size_(0), capacity_(capacity) {
    records_ = new Record[capacity_];
  }

  constexpr HashMap(const HashMap &other) noexcept {
    size_ = other.size_;
    capacity_ = other.capacity_;
    records_ = new Record[capacity_];

    for (size_t i = 0; i < capacity_; i++) {
      records_[i] = other.records_[i];
    }
  }

  constexpr HashMap &operator=(const HashMap &other) noexcept {
    if (records_ != nullptr)
      delete[] records_;

    size_ = other.size_;
    capacity_ = other.capacity_;
    records_ = new Record[capacity_];

    for (size_t i = 0; i < capacity_; i++) {
      records_[i] = other.records_[i];
    }

    return *this;
  }

  constexpr ~HashMap() {
    if (records_ != nullptr)
      delete[] records_;
  }

  /*
    Getter method for size_.
   */
  [[nodiscard]] constexpr size_t size() const noexcept { return size_; }

  /*
    Getter method for capacity_.
   */
  [[nodiscard]] constexpr size_t capacity() const noexcept { return capacity_; }

  /*
    Inserts a key-value pair into the HashMap. This is done by
    calculating the hash of the key to obtain an index. If the
    index is empty the key-value pair is written to that index.
    Otherwise, a suitable index is found using linear probing.

    If the insertion causes the load factor to exceed the limit
    a rehashing is performed.

    Returns a reference to the inserted Record.
   */
  constexpr std::optional<Record> insert(const Key k, const Value v) noexcept {
    if (calculate_loadfactor() >= loadfactor_limit_) {
      rehash();
    }

    size_t index = get_hash(k, capacity_);
    Record record(RecordState::Occupied, k, v);

    if (records_[index].state == RecordState::Empty) {
      records_[index] = record;
      ++size_;

      return records_[index];
    } else {
      for (size_t i = (index + 1) % capacity_; i != index;
           i = (i + 1) % capacity_) {
        if (records_[i].state == RecordState::Empty) {
          records_[i] = record;
          ++size_;

          return records_[i];
        }
      }
    }

    return {};
  }

  /*
    Searches the HashMap for a member that have a certain key.
    This is done by calculating the hash of the key to determine
    the index. If the index does not contain an occupied Record,
    linear probing is used to search the remaining records.

    Returns true if an occupied record is found.
   */
  [[nodiscard]] constexpr bool contains(const Key k) const noexcept {
    if (size_ == 0) {
      return false;
    }

    auto index = get_hash(k, capacity_);

    if (records_[index].key == k &&
        records_[index].state == RecordState::Occupied) {
      return true;
    } else {
      for (size_t i = (index + 1) % capacity_; i != index;
           i = (i + 1) % capacity_) {
        if (records_[i].key == k &&
            records_[i].state == RecordState::Occupied) {
          return true;
        }
      }
    }

    return false;
  }

  /*
    Searches the HashMap for a Record with a certain key.
    The initial index is determined by calculating the
    hash of the key. If that index does not contain a
    Record with the specified key, linear probing is
    used to search the rest of the entries.

    If a matching Record is found, a reference to that
    Record is returned as a std::optional. Otherwise
    a std::nullopt is returned.
   */
  [[nodiscard]] constexpr std::optional<Record>
  find(const Key k) const noexcept {
    if (size_ == 0) {
      return {};
    }

    auto index = get_hash(k, capacity_);

    if (records_[index].key == k &&
        records_[index].state == RecordState::Occupied) {
      return records_[index];
    } else {
      for (size_t i = (index + 1) % capacity_; i != index;
           i = (i + 1) % capacity_) {
        if (records_[i].key == k &&
            records_[i].state == RecordState::Occupied) {
          return records_[i];
        }
      }
    }

    return {};
  }

  /*
    Removes a Record with a certain key. If a matching Record
    is found, it is "removed" by setting its state to Empty.
    Thus no data is actually deleted, because this is cheaper
    than overwriting all members to their zero values.

    If a matching Record is found, a copy of that record is
    returned as a std::optional. Otherwise, a std::nullopt
    is returned.
   */
  constexpr std::optional<Record> remove(const Key k) noexcept {
    Record target = this->operator[](k);

    target.state = RecordState::Empty;
    --size_;
    return target;
  }

  /*
    Searches for a Record with Key k. The initial index is
    determined by the hash value of k. If the initial index
    does not contain a matching Record, use linear probing
    to search the remaining records. Finally, if no matching
    Record was found, an insertion of Key k is made with
    Value constructed in-place.
   */
  constexpr Record& operator[](const Key& k) noexcept {
    auto index = get_hash(k, capacity_);

    if (records_[index].key == k
	&& records_[index].state == RecordState::Occupied) {
      return records_[index];
    } else {
      // Use linear probing to search.
      for (size_t i = (index + 1) % capacity_;
	   i != index;
	   i = (i + 1) % capacity_) {
	if (records_[i].key == k
	    && records_[i].state == RecordState::Occupied) {
	  return records_[i];
	}
      }

      // Otherwise insert a Record with Key k and zero-value of Value.
      auto inserted = this->insert(k, Value{0});
      return inserted.value();
    }
  }
};

/*
  An implementation of an immutable, read-only suffix-tree
  (trie), where every operation can be evaluated at compile-time.
 */
class ReadOnlyTrie {
public:
  constexpr ReadOnlyTrie() noexcept {}

  /*
    Searches the trie for a string that completes a word.
   */
  [[nodiscard]] bool contains(std::string str) const noexcept {
    auto begin = str.cbegin();

    if (auto found = nodes_.find(*begin)) {
      return found->value.contains(begin, str.cend());
    }

    return false;
  }

private:
  class Node {
  public:
    constexpr Node(char value, bool completes_word = false) noexcept
        : value_(value), completes_word_(completes_word) {}

    constexpr ~Node() noexcept = default;

    [[nodiscard]] constexpr char value() const noexcept { return value_; }

    [[nodiscard]] constexpr bool completes_word() const noexcept {
      return completes_word_;
    }

    constexpr void set_completes_word(bool b) noexcept { completes_word_ = b; }

    [[nodiscard]] constexpr bool
    contains(std::string::const_iterator it,
             std::string::const_iterator end) const noexcept {
      char ch = *it;

      if (ch == value_) {
        if (it == end) {
          return true && completes_word_;
        } else {
          ++it;
          auto n = nodes_.find(*it);

          if (n != std::nullopt) {
            return n.value().value.contains(it, end);
          }
        }
      }

      return false;
    }

    constexpr void insert(std::string::const_iterator it,
                          std::string::const_iterator end) noexcept {
      char ch = *it;

      auto next = nodes_.insert(ch, Node(ch));

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

}; // namespace DataStructures

}; // namespace OS2DSRules

#endif
