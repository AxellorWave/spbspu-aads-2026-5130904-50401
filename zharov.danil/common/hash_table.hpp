#ifndef HASH_TABLE_HPP
#define HASH_TABLE_HPP
#include <cmath>
#include <cstddef>
#include <stdexcept>

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
    Slot() = delete;
    Slot(const Key& key, const Value& value):
      key_(key),
      value_(value)
    {}
    Slot(Key&& key, Value&& value):
      key_(std::move(key)),
      value_(std::move(value))
    {}
    Key key_;
    Value value_;
  };

  template < class Key, class Value, class Hash, class Equal >
  class HashTable;

  template < class Key, class Value, class Hash, class Equal >
  class Iter
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    State* states_;
    Slot< Key, Value >* slots_;
    size_t curr_;
    size_t capacity_;
    Iter(State* states, Slot< Key, Value >* slots, size_t curr, size_t capacity):
      states_(states),
      slots_(slots),
      curr_(curr),
      capacity_(capacity)
    {}

  public:
    Slot< Key, Value >& operator*() const;
    Slot< Key, Value >* operator->() const;
    Iter& operator++();
    Iter operator++(int);
    Iter& operator--();
    Iter operator--(int);
    bool operator==(const Iter& it) const noexcept;
    bool operator!=(const Iter& it) const noexcept;
  };

  template < class Key, class Value, class Hash, class Equal >
  class CIter
  {
    friend class HashTable< Key, Value, Hash, Equal >;
    State* states_;
    Slot< Key, Value >* slots_;
    size_t curr_;
    size_t capacity_;
    CIter(State* states, Slot< Key, Value >* slots, size_t curr, size_t capacity):
      states_(states),
      slots_(slots),
      curr_(curr),
      capacity_(capacity)
    {}

  public:
    const Slot< Key, Value >& operator*() const;
    const Slot< Key, Value >* operator->() const;
    CIter& operator++();
    CIter operator++(int);
    CIter& operator--();
    CIter operator--(int);
    bool operator==(const CIter& it) const noexcept;
    bool operator!=(const CIter& it) const noexcept;
  };

  template < class Key, class Value, class Hash, class Equal >
  class HashTable
  {
  public:
    using iterator = Iter< Key, Value, Hash, Equal >;
    using const_iterator = CIter< Key, Value, Hash, Equal >;
    HashTable();
    HashTable(size_t capacity);
    HashTable(const HashTable& table);
    HashTable(HashTable&& table) noexcept;
    ~HashTable();

    HashTable& operator=(const HashTable& table);
    HashTable& operator=(HashTable&& table) noexcept;

    void add(const Key& k, const Value& v);
    void add(Key&& k, Value&& v);
    void remove(const Key& k);
    bool has(const Key& k) const;
    void rehash(size_t slots = 0);
    void swap(HashTable& table) noexcept;
    size_t getSize() const noexcept;
    size_t getCapacity() const noexcept;
    Value& at(const Key& k);
    const Value& at(const Key& k) const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

  private:
    template < class K, class V >
    void addImpl(K&& k, V&& v);

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
  size_(0)
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
      new (slots_ + i) Slot< Key, Value >(table.slots_[i].key_, table.slots_[i].value_);
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
  table.states_ = nullptr;
  table.slots_ = nullptr;
  table.capacity_ = 0;
  table.size_ = 0;
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
size_t zharov::HashTable< Key, Value, Hash, Equal >::getCapacity() const noexcept
{
  return capacity_;
}

template < class Key, class Value, class Hash, class Equal >
size_t zharov::HashTable< Key, Value, Hash, Equal >::getSize() const noexcept
{
  return size_;
}

template < class Key, class Value, class Hash, class Equal >
bool zharov::HashTable< Key, Value, Hash, Equal >::has(const Key& k) const
{
  size_t hash = hasher_(k);
  size_t i = 0;
  size_t pos = 0;
  for (; i < capacity_; ++i)
  {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == State::OCCUPIED && comparator_(slots_[pos].key_, k))
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
void zharov::HashTable< Key, Value, Hash, Equal >::add(const Key& k, const Value& v)
{
  addImpl(k, v);
}

template < class Key, class Value, class Hash, class Equal >
void zharov::HashTable< Key, Value, Hash, Equal >::add(Key&& k, Value&& v)
{
  addImpl(std::forward< Key >(k), std::forward< Value >(v));
}

template < class Key, class Value, class Hash, class Equal >
template < class K, class V >
void zharov::HashTable< Key, Value, Hash, Equal >::addImpl(K&& k, V&& v)
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
  new (slots_ + pos) Slot< Key, Value >(std::forward< K >(k), std::forward< V >(v));
  states_[pos] = State::OCCUPIED;
  ++size_;
}

template < class Key, class Value, class Hash, class Equal >
void zharov::HashTable< Key, Value, Hash, Equal >::remove(const Key& k)
{
  size_t hash = hasher_(k);
  size_t i = 0;
  size_t pos = 0;
  for (; i < capacity_; ++i)
  {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == State::EMPTY)
    {
      throw std::logic_error("Key not found");
    }
    else if (states_[pos] == State::OCCUPIED && comparator_(k, slots_[pos].key_))
    {
      (slots_ + pos)->~Slot();
      states_[pos] = State::TOMBSTONE;
      --size_;
      return;
    }
  }
}

template < class Key, class Value, class Hash, class Equal >
void zharov::HashTable< Key, Value, Hash, Equal >::rehash(size_t slots)
{
  slots = slots ? std::pow(2, ceil(log2(slots))) : capacity_ * 2;
  HashTable< Key, Value, Hash, Equal > new_table(slots);
  for (size_t i = 0; i < capacity_; ++i)
  {
    if (states_[i] == State::OCCUPIED)
    {
      new_table.add(slots_[i].key_, slots_[i].value_);
    }
  }
  swap(new_table);
}

template < class Key, class Value, class Hash, class Equal >
Value& zharov::HashTable< Key, Value, Hash, Equal >::at(const Key& k)
{
  const HashTable* const_table = this;
  return const_cast< Value& >((*const_table).at(k));
}

template < class Key, class Value, class Hash, class Equal >
const Value& zharov::HashTable< Key, Value, Hash, Equal >::at(const Key& k) const
{
  size_t hash = hasher_(k);
  size_t i = 0;
  size_t pos = 0;
  for (; i < capacity_; ++i)
  {
    pos = (hash + (i + i * i) / 2) % capacity_;
    if (states_[pos] == State::OCCUPIED && comparator_(slots_[pos].key_, k))
    {
      return slots_[pos].value_;
    }
    else if (states_[pos] == State::EMPTY)
    {
      break;
    }
  }
  throw std::logic_error("Key not found");
}

template < class Key, class Value, class Hash, class Equal >
zharov::Slot< Key, Value >& zharov::Iter< Key, Value, Hash, Equal >::operator*() const
{
  return slots_[curr_];
}

template < class Key, class Value, class Hash, class Equal >
zharov::Slot< Key, Value >* zharov::Iter< Key, Value, Hash, Equal >::operator->() const
{
  return slots_ + curr_;
}

template < class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal >& zharov::Iter< Key, Value, Hash, Equal >::operator++()
{
  ++curr_;
  while (curr_ < capacity_ && states_[curr_] != State::OCCUPIED)
  {
    ++curr_;
  }
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal > zharov::Iter< Key, Value, Hash, Equal >::operator++(int)
{
  Iter< Key, Value, Hash, Equal > temp = *this;
  ++(*this);
  return temp;
}

template < class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal >& zharov::Iter< Key, Value, Hash, Equal >::operator--()
{
  --curr_;
  while (curr_ != 0 && states_[curr_] != State::OCCUPIED)
  {
    --curr_;
  }
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal > zharov::Iter< Key, Value, Hash, Equal >::operator--(int)
{
  Iter< Key, Value, Hash, Equal > temp = *this;
  --(*this);
  return temp;
}

template < class Key, class Value, class Hash, class Equal >
bool zharov::Iter< Key, Value, Hash, Equal >::operator==(const Iter& it) const noexcept
{
  return slots_ == it.slots_ && states_ == it.states_ && curr_ == it.curr_ &&
    capacity_ == it.capacity_;
}

template < class Key, class Value, class Hash, class Equal >
bool zharov::Iter< Key, Value, Hash, Equal >::operator!=(const Iter& it) const noexcept
{
  return !(it == *this);
}

template < class Key, class Value, class Hash, class Equal >
const zharov::Slot< Key, Value >& zharov::CIter< Key, Value, Hash, Equal >::operator*() const
{
  return slots_[curr_];
}

template < class Key, class Value, class Hash, class Equal >
const zharov::Slot< Key, Value >* zharov::CIter< Key, Value, Hash, Equal >::operator->() const
{
  return slots_ + curr_;
}

template < class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal >& zharov::CIter< Key, Value, Hash, Equal >::operator++()
{
  ++curr_;
  while (curr_ < capacity_ && states_[curr_] != State::OCCUPIED)
  {
    ++curr_;
  }
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal > zharov::CIter< Key, Value, Hash, Equal >::operator++(int)
{
  CIter< Key, Value, Hash, Equal > temp = *this;
  ++(*this);
  return temp;
}

template < class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal >& zharov::CIter< Key, Value, Hash, Equal >::operator--()
{
  --curr_;
  while (curr_ != 0 && states_[curr_] != State::OCCUPIED)
  {
    --curr_;
  }
  return *this;
}

template < class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal > zharov::CIter< Key, Value, Hash, Equal >::operator--(int)
{
  CIter< Key, Value, Hash, Equal > temp = *this;
  --(*this);
  return temp;
}

template < class Key, class Value, class Hash, class Equal >
bool zharov::CIter< Key, Value, Hash, Equal >::operator==(const CIter& it) const noexcept
{
  return slots_ == it.slots_ && states_ == it.states_ && curr_ == it.curr_ &&
    capacity_ == it.capacity_;
}

template < class Key, class Value, class Hash, class Equal >
bool zharov::CIter< Key, Value, Hash, Equal >::operator!=(const CIter& it) const noexcept
{
  return !(it == *this);
}

template < class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal > zharov::HashTable< Key, Value, Hash, Equal >::begin()
{
  Iter< Key, Value, Hash, Equal > it(states_, slots_, 0, capacity_);
  while (it.curr_ < it.capacity_ && it.states_[it.curr_] != State::OCCUPIED)
  {
    ++it.curr_;
  }
  return it;
}

template < class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal > zharov::HashTable< Key, Value, Hash, Equal >::begin() const
{
  CIter< Key, Value, Hash, Equal > it(states_, slots_, 0, capacity_);
  while (it.curr_ < it.capacity_ && it.states_[it.curr_] != State::OCCUPIED)
  {
    ++it.curr_;
  }
  return it;
}

template < class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal >
zharov::HashTable< Key, Value, Hash, Equal >::cbegin() const
{
  CIter< Key, Value, Hash, Equal > it(states_, slots_, 0, capacity_);
  while (it.curr_ < it.capacity_ && it.states_[it.curr_] != State::OCCUPIED)
  {
    ++it.curr_;
  }
  return it;
}

template < class Key, class Value, class Hash, class Equal >
zharov::Iter< Key, Value, Hash, Equal > zharov::HashTable< Key, Value, Hash, Equal >::end()
{
  return Iter< Key, Value, Hash, Equal >(states_, slots_, capacity_, capacity_);
}

template < class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal > zharov::HashTable< Key, Value, Hash, Equal >::end() const
{
  return CIter< Key, Value, Hash, Equal >(states_, slots_, capacity_, capacity_);
}

template < class Key, class Value, class Hash, class Equal >
zharov::CIter< Key, Value, Hash, Equal > zharov::HashTable< Key, Value, Hash, Equal >::cend() const
{
  return CIter< Key, Value, Hash, Equal >(states_, slots_, capacity_, capacity_);
}

#endif
