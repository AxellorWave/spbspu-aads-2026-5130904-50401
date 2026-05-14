#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cstddef>

namespace zharov
{
  enum class State
  {
    EMPTY,
    OCCUPIED,
    TOMBSTONE
  };

  template < class Key, class Value >
  struct Slot
  {
    Key key_;
    Value value_;
  };

  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    HashTable();
    HashTable(size_t capacity);
    HashTable(const HashTable& table);
    HashTable(HashTable&& table) noexcept;
    ~HashTable();

    HashTable& operator=(const HashTable& table);
    HashTable& operator=(HashTable&& table) noexcept;

    void add(Key k, Value v);
    Value drop(Key k);
    bool has(Key k);
    void rehash(size_t slots);
    size_t getSize() const;
    size_t getCapacity() const;
    Value& at(Key key);
    const Value& at(Key key) const;

  private:
    Hash hasher_;
    Equal comparator_;
    State* states_;
    Slot< Key, Value >* sllots_;
    size_t capacity_;
    size_t size_;
  };
}

#endif
