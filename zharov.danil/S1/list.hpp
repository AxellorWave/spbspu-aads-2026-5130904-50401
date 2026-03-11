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
    LIter operator++(int);
    LIter & operator--();
    LIter operator--(int);
    bool operator==(const LIter & it) const;
    bool operator!=(const LIter & it) const;
  };

    template< class T >
  class LCIter {
    friend class List< T >;
    const Node< T > * curr_;
    LCIter(const Node< T > * node);
  public:
    const T & operator*() const;
    const T * operator->() const;
    LCIter & operator++();
    LCIter operator++(int);
    LCIter & operator--();
    LCIter operator--(int);
    bool operator==(const LCIter & it) const;
    bool operator!=(const LCIter & it) const;
  };

}

#endif
