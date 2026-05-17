#ifndef HASHER_HPP
#define HASHER_HPP
#include <boost/hash2/blake2.hpp>
#include <boost/hash2/hash_append.hpp>
#include <cstddef>

namespace zharov
{
  template < class T >
  struct Blake2Hasher
  {
    Blake2Hasher(std::size_t s = 0):
      seed_(s)
    {}

    std::size_t operator()(const T& value) const
    {
      boost::hash2::blake2b_512 h(seed_);
      boost::hash2::hash_append(h, {}, value);

      return *reinterpret_cast< const std::size_t* >(h.result().data());
    }

  private:
    std::size_t seed_;
  };
}

#endif
