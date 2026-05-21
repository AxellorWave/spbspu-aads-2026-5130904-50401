#include <string>
#include <boost/test/unit_test.hpp>
#include <stdexcept>
#include "bstree.hpp"

struct comp
{
  bool operator()(int f, int s) const
  {
    return f < s;
  }
};

BOOST_AUTO_TEST_SUITE(DefaultConstructorSuite)
BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  zharov::BSTree< int, std::string, comp > tree;
  BOOST_CHECK_EQUAL(tree.getSize(), 0);
  BOOST_CHECK_EQUAL(tree.height(), 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CopyConstructorSuite)
BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  zharov::BSTree< int, std::string, comp > original;
  original.push(1, "one");
  original.push(2, "two");
  zharov::BSTree< int, std::string, comp > copy(original);
  BOOST_CHECK_EQUAL(copy.getSize(), 2);
  BOOST_CHECK_EQUAL(copy.get(1), "one");
  BOOST_CHECK_EQUAL(copy.get(2), "two");
  original.push(3, "three");
  BOOST_CHECK_EQUAL(copy.getSize(), 2);
  BOOST_CHECK_THROW(copy.get(3), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MoveConstructorSuite)
BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  zharov::BSTree< int, std::string, comp > original;
  original.push(10, "ten");
  original.push(20, "twenty");
  zharov::BSTree< int, std::string, comp > moved(std::move(original));
  BOOST_CHECK_EQUAL(moved.getSize(), 2);
  BOOST_CHECK_EQUAL(moved.get(10), "ten");
  BOOST_CHECK_EQUAL(moved.get(20), "twenty");
  BOOST_CHECK_EQUAL(original.getSize(), 0);
  BOOST_CHECK_EQUAL(original.height(), 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(CopyAssignmentOperatorSuite)
BOOST_AUTO_TEST_CASE(CopyAssignmentSelfAssign)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(5, "five");
  tree = tree;
  BOOST_CHECK_EQUAL(tree.getSize(), 1);
  BOOST_CHECK_EQUAL(tree.get(5), "five");
}
BOOST_AUTO_TEST_CASE(CopyAssignmentDifferentTrees)
{
  zharov::BSTree< int, std::string, comp > a, b;
  a.push(100, "hundred");
  b.push(200, "two hundred");
  b = a;
  BOOST_CHECK_EQUAL(b.getSize(), 1);
  BOOST_CHECK_EQUAL(b.get(100), "hundred");
  BOOST_CHECK_THROW(b.get(200), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(MoveAssignmentOperatorSuite)
BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  zharov::BSTree< int, std::string, comp > a, b;
  a.push(7, "seven");
  a.push(8, "eight");
  b = std::move(a);
  BOOST_CHECK_EQUAL(b.getSize(), 2);
  BOOST_CHECK_EQUAL(b.get(7), "seven");
  BOOST_CHECK_EQUAL(b.get(8), "eight");
  BOOST_CHECK_EQUAL(a.getSize(), 0);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(SwapSuite)
BOOST_AUTO_TEST_CASE(Swap)
{
  zharov::BSTree< int, std::string, comp > a, b;
  a.push(1, "first");
  b.push(2, "second");
  a.swap(b);
  BOOST_CHECK_EQUAL(a.getSize(), 1);
  BOOST_CHECK_EQUAL(a.get(2), "second");
  BOOST_CHECK_EQUAL(b.getSize(), 1);
  BOOST_CHECK_EQUAL(b.get(1), "first");
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ClearSuite)
BOOST_AUTO_TEST_CASE(Clear)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(42, "answer");
  tree.push(13, "unlucky");
  tree.clear();
  BOOST_CHECK_EQUAL(tree.getSize(), 0);
  BOOST_CHECK_EQUAL(tree.height(), 0);
  BOOST_CHECK_THROW(tree.at(42), std::logic_error);
  BOOST_CHECK_THROW(tree.get(13), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(PushSuite)
BOOST_AUTO_TEST_CASE(PushLvalue)
{
  zharov::BSTree< int, std::string, comp > tree;
  const int key = 10;
  const std::string value = "ten";
  tree.push(key, value);
  BOOST_CHECK_EQUAL(tree.getSize(), 1);
  BOOST_CHECK_EQUAL(tree.get(10), "ten");
}
BOOST_AUTO_TEST_CASE(PushRvalue)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(20, std::string("twenty"));
  BOOST_CHECK_EQUAL(tree.getSize(), 1);
  BOOST_CHECK_EQUAL(tree.get(20), "twenty");
}
BOOST_AUTO_TEST_CASE(PushDuplicateUpdatesValue)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(3, "old");
  tree.push(3, "new");
  BOOST_CHECK_EQUAL(tree.getSize(), 1);
  BOOST_CHECK_EQUAL(tree.get(3), "new");
}
BOOST_AUTO_TEST_CASE(PushMultipleElements)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(2, "two");
  tree.push(4, "four");
  BOOST_CHECK_EQUAL(tree.getSize(), 5);
  BOOST_CHECK_EQUAL(tree.height(), 3);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(GetSuite)
BOOST_AUTO_TEST_CASE(GetExistingKey)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(15, "fifteen");
  BOOST_CHECK_EQUAL(tree.get(15), "fifteen");
}
BOOST_AUTO_TEST_CASE(GetNonExistentKeyThrows)
{
  zharov::BSTree< int, std::string, comp > tree;
  BOOST_CHECK_THROW(tree.get(99), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(AtSuite)
BOOST_AUTO_TEST_CASE(AtNonConstModifies)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(50, "initial");
  tree.at(50) = "modified";
  BOOST_CHECK_EQUAL(tree.get(50), "modified");
}
BOOST_AUTO_TEST_CASE(AtConstReturnsRef)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(60, "constant");
  const auto& ctree = tree;
  BOOST_CHECK_EQUAL(ctree.at(60), "constant");
}
BOOST_AUTO_TEST_CASE(AtThrowsOnMissingKey)
{
  zharov::BSTree< int, std::string, comp > tree;
  BOOST_CHECK_THROW(tree.at(404), std::logic_error);
  const auto& ctree = tree;
  BOOST_CHECK_THROW(ctree.at(404), std::logic_error);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(DropSuite)
BOOST_AUTO_TEST_CASE(DropLeafNode)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(8, "eight");
  tree.push(3, "three");
  std::string removed = tree.drop(3);
  BOOST_CHECK_EQUAL(removed, "three");
  BOOST_CHECK_EQUAL(tree.getSize(), 1);
  BOOST_CHECK_THROW(tree.get(3), std::logic_error);
  BOOST_CHECK_EQUAL(tree.get(8), "eight");
}
BOOST_AUTO_TEST_CASE(DropNodeWithOneChild)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(8, "eight");
  tree.push(3, "three");
  tree.push(1, "one");
  tree.drop(3);
  BOOST_CHECK_EQUAL(tree.getSize(), 2);
  BOOST_CHECK_THROW(tree.get(3), std::logic_error);
  BOOST_CHECK_EQUAL(tree.get(1), "one");
  BOOST_CHECK_EQUAL(tree.get(8), "eight");
}
BOOST_AUTO_TEST_CASE(DropNodeWithTwoChildren)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(10, "ten");
  tree.push(5, "five");
  tree.push(15, "fifteen");
  tree.push(3, "three");
  tree.push(7, "seven");

  std::string removed = tree.drop(5);
  BOOST_CHECK_EQUAL(removed, "five");
  BOOST_CHECK_EQUAL(tree.getSize(), 4);
  BOOST_CHECK_THROW(tree.get(5), std::logic_error);

  BOOST_CHECK_EQUAL(tree.get(7), "seven");
  BOOST_CHECK_NO_THROW(tree.get(7));
}
BOOST_AUTO_TEST_CASE(DropRoot)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(100, "hundred");
  std::string removed = tree.drop(100);
  BOOST_CHECK_EQUAL(removed, "hundred");
  BOOST_CHECK_EQUAL(tree.getSize(), 0);
  BOOST_CHECK_EQUAL(tree.height(), 0);
}
BOOST_AUTO_TEST_CASE(DropNonExistentKeyThrows)
{
  zharov::BSTree< int, std::string, comp > tree;
  BOOST_CHECK_THROW(tree.drop(123), std::out_of_range);
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(HeightSuite)
BOOST_AUTO_TEST_CASE(EmptyTreeHeight)
{
  zharov::BSTree< int, std::string, comp > tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);
}
BOOST_AUTO_TEST_CASE(SingleElementHeight)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.height(), 1);
}
BOOST_AUTO_TEST_CASE(LinearTreeHeight)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(1, "a");
  tree.push(2, "b");
  tree.push(3, "c");
  BOOST_CHECK_EQUAL(tree.height(), 3);
}
BOOST_AUTO_TEST_CASE(BalancedTreeHeight)
{
  zharov::BSTree< int, std::string, comp > tree;
  tree.push(4, "root");
  tree.push(2, "L");
  tree.push(6, "R");
  tree.push(1, "LL");
  tree.push(3, "LR");
  tree.push(5, "RL");
  tree.push(7, "RR");
  BOOST_CHECK_EQUAL(tree.height(), 3);
}
BOOST_AUTO_TEST_SUITE_END()
