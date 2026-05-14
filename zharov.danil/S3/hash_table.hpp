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
    void swap(HashTable& table) noexcept;
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
  capacity_(std::pow(2, ceil(log2(capacity)))),
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
zharov::HashTable< Key, Value, Hash, Equal >::HashTable(HashTable&& table) noexcept:
  hasher_(table.hasher_),
  comparator_(table.comparator_),
  states_(table.states_),
  slots_(table.slots_),
  capacity_(table.capacity_),
  size_(table.size_)
{
  states_ = nullptr;
  slots_ = nullptr;
  capacity_ = 0;
  size_ = 0;
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
void zharov::HashTable< Key, Value, Hash, Equal >::swap(HashTable& table) noexcept
{
  std::swap(table.hasher_, hasher_);
  std::swap(table.comparator_, comparator_);
  std::swap(table.states_, states_);
  std::swap(table.slots_, slots_);
  std::swap(table.capacity_, capacity_);
  std::swap(table.size_, size_);
}

template < class Key, class Value, class Hash, class Equal >
zharov::HashTable< Key, Value, Hash, Equal >&
zharov::HashTable< Key, Value, Hash, Equal >::operator=(const HashTable& table)
{
  if (std::addressof(table) == this)
  {
    return *this;
  }

  HashTable< Key, Value, Hash, Equal > cp(table);
  swap(cp);
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
zharov::HashTable< Key, Value, Hash, Equal >&
zharov::HashTable< Key, Value, Hash, Equal >::operator=(HashTable&& table) noexcept
{
  if (std::addressof(table) == this)
  {
    return *this;
  }

  HashTable< Key, Value, Hash, Equal > cp(std::move(table));
  swap(cp);
  return *this;
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

template < class Key, class Value, class Hash, class Equal >
bool zharov::HashTable< Key, Value, Hash, Equal >::has(Key k)
{
  size_t hash = hasher_(k);
  size_t i = 0;
  size_t pos = 0;
  for (; i < capacity_; ++i)
  {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == State::OCCUPIED && comparator_(slots_[pos].key, k))
    {
      return true;
    }
    else if (states_[pos] == State::EMPTY)
    {
      return false;
    }
  }
  return false;
}

template < class Key, class Value, class Hash, class Equal >
void zharov::HashTable< Key, Value, Hash, Equal >::add(Key k, Value v)
{
  if (size_ == capacity_)
  {
    throw std::logic_error("Not enough place to add");
  }

  if (has(k))
  {
    throw std::logic_error("Key already exist");
  }
  size_t hash = hasher_(k);
  size_t i = 0;
  size_t pos = 0;
  for (; i < capacity_; ++i)
  {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == State::EMPTY || states_[pos] == State::TOMBSTONE)
    {
      break;
    }
  }
  new (slots_ + pos) Slot< Key, Value >(k, val);
  states_[pos] = State::OCCUPIED;
  ++size_;
}

#endif
