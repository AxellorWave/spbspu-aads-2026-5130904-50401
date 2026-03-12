#define BOOST_TEST_MODULE S1
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

BOOST_AUTO_TEST_CASE(DefaultListConstructorTest)
{
  zharov::List< int > list;
  BOOST_CHECK_EQUAL(list.size(), 0);
  BOOST_CHECK(list.begin() == list.end());
}

BOOST_AUTO_TEST_CASE(CopyListConstructorTest)
{
  zharov::List< int > list1;
  list1.pushBack(1);
  list1.pushBack(2);
  list1.pushBack(3);
  BOOST_CHECK_EQUAL(list1.front(), 1);
  BOOST_CHECK_EQUAL(list1.back(), 3);
  zharov::List< int > list2 = zharov::List<int>(list1);
  zharov::LIter< int > it = list2.begin();
  BOOST_CHECK_EQUAL(*it, 1);
  ++it;
  BOOST_CHECK_EQUAL(*it, 2);
}