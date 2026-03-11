#ifndef LIST_HPP
#define LIST_HPP

namespace zharov {

  template< class T >
  struct Node {
    T val_;
    Node< T > * next_;
    Node< T > * prev_;
  };

  template< class T >
  class List;

  template< class T >
  class LIter {
    friend class List< T >;
    Node< T > * curr_;
    LIter(Node< T > * node);
  public:
    T & operator*() const;
    T * operator->() const;
    LIter & operator++();
    LIter & operator++(int);
    LIter & operator--();
    LIter & operator--(int);
    bool operator==(const LIter & it) const;
    bool operator!=(const LIter & it) const;
  };

}

#endif