#ifndef RH_HASH_TABLE_HPP
#define RH_HASH_TABLE_HPP
#include <cstddef>
#include <stdexcept>
#include <utility>

namespace zharov
{
  template < class Key, class Value, class Hash, class Equal >
  class RHHashTable;

  namespace detail
  {
    template < class Key, class Value >
    struct Slot
    {
      Slot() = delete;
      Slot(const Key& key, const Value& value, size_t psl):
        key_(key),
        value_(value),
        psl_(psl)
      {}
      Slot(Key&& key, Value&& value, size_t psl):
        key_(std::move(key)),
        value_(std::move(value)),
        psl_(psl)
      {}
      Key key_;
      Value value_;
      size_t psl_;
    };
  }

  template < class Key, class Value, class Hash, class Equal >
  class Iter
  {
  public:
    detail::Slot< Key, Value >& operator*() const;
    detail::Slot< Key, Value >* operator->() const;
    Iter& operator++();
    Iter operator++(int);
    bool operator==(const Iter& it) const;
    bool operator!=(const Iter& it) const;

  private:
    friend class RHHashTable< Key, Value, Hash, Equal >;
    bool* occupied_;
    detail::Slot< Key, Value >* slots_;
    size_t curr_;
    size_t capacity_;
    Iter(bool* occupied, detail::Slot< Key, Value >* slots, size_t curr, size_t capacity);
  };

  template < class Key, class Value, class Hash, class Equal >
  class CIter
  {
  public:
    const detail::Slot< Key, Value >& operator*() const;
    const detail::Slot< Key, Value >* operator->() const;
    CIter& operator++();
    CIter operator++(int);
    bool operator==(const CIter& it) const;
    bool operator!=(const CIter& it) const;

  private:
    friend class RHHashTable< Key, Value, Hash, Equal >;
    bool* occupied_;
    detail::Slot< Key, Value >* slots_;
    size_t curr_;
    size_t capacity_;
    CIter(bool* occupied, detail::Slot< Key, Value >* slots, size_t curr, size_t capacity);
  };

  template < class Key, class Value, class Hash, class Equal >
  class RHHashTable
  {
  public:
    using iterator = Iter< Key, Value, Hash, Equal >;
    using const_iterator = CIter< Key, Value, Hash, Equal >;

    RHHashTable();
    explicit RHHashTable(size_t capacity);
    RHHashTable(const RHHashTable& table);
    RHHashTable(RHHashTable&& table) noexcept;
    ~RHHashTable();

    RHHashTable& operator=(const RHHashTable& table);
    RHHashTable& operator=(RHHashTable&& table) noexcept;

    void add(const Key& k, const Value& v);
    void add(Key&& k, Value&& v);
    void remove(const Key& k);
    bool has(const Key& k) const;
    void rehash(size_t slots = 0);
    void swap(RHHashTable& table) noexcept;
    size_t size() const;
    size_t capacity() const;
    Value& at(const Key& k);
    const Value& at(const Key& k) const;

    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;

  private:
    Hash hasher_;
    Equal equal_;
    bool* occupied_;
    detail::Slot< Key, Value >* slots_;
    size_t capacity_;
    size_t size_;
  };
}

#endif
