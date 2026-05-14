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
    Value& at(Key k);
    const Value& at(Key k) const;

  private:
    Hash hasher_;
    Equal comparator_;
    State* states_;
    Slot< Key, Value >* slots_;
    size_t capacity_;
    size_t size_;
  };
}

template < class Key, class Value, class Hash, class Equal >
zharov::HashTable< Key, Value, Hash, Equal >::HashTable():
  HashTable(16)
{}

template < class Key, class Value, class Hash, class Equal >
zharov::HashTable< Key, Value, Hash, Equal >::HashTable(size_t capacity):
  hasher_(Hash{}),
  comparator_(Equal{}),
  states_(nullptr),
  slots_(nullptr),
  capacity_(capacity),
  size_(0),
{
  try
  {
    states_ = new State[capacity_]{};
    slots_ =
      static_cast< Slot< Key, Value >* >(::operator new(sizeof(Slot< Key, Value >) * capacity_));
  }
  catch (...)
  {
    delete[] states_;
    ::operator delete(slots_);
    throw;
  }
}

template < class Key, class Value, class Hash, class Equal >
zharov::HashTable< Key, Value, Hash, Equal >::HashTable(const HashTable& table):
  HashTable(table.capacity_)
{
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (table.states_[i] == State::OCCUPIED)
    {
      new (slots_ + i) Slot< Key, Value >(table.slots_[i].first, table.slots_[i].second);
      ++size_;
    }
    states_[i] = table.states_[i];
  }
}

template < class Key, class Value, class Hash, class Equal >
zharov::HashTable< Key, Value, Hash, Equal >::~HashTable()
{
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (states_[i] == State::OCCUPIED)
    {
      (slots_ + i)->~Slot();
    }
  }
  delete[] states_;
  ::operator delete(slots_);
}

template < class Key, class Value, class Hash, class Equal >
size_t zharov::HashTable< Key, Value, Hash, Equal >::getCapacity() const
{
  return capacity_;
}

template < class Key, class Value, class Hash, class Equal >
size_t zharov::HashTable< Key, Value, Hash, Equal >::getSize() const
{
  return size;
}

#endif
