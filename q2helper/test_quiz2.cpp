//#include <string>
//#include <iostream>
//#include <fstream>
//#include "gtest/gtest.h"
//#include <vector>
//#include <algorithm>    // std::random_shuffle
//#include "q2solution.hpp"
//
//class Quiz2 : public ::testing::Test {
//protected:
//    virtual void SetUp()    {}
//    virtual void TearDown() {}
//};
//
//
//template<class T>
//LN<T>* build_linked_list_fast(T values[], int length) {  //O(N)
//  if (length == 0)
//    return nullptr;
//
//  LN<T>* front      = new LN<T>(values[0]);
//  LN<T>* rear_cache = front;
//  for (int i=1; i<length; ++i)
//    rear_cache = rear_cache->next = new LN<T>(values[i]);
//
//  return front;
//}
//
//
////Simple way to compare linked lists for equality (useful in googletest)
//template<class T>
//bool operator == (LN<T> l1, LN<T> l2) {
//  for (LN<T>* p1  = &l1, *p2 = &l2; /*see body*/; p1=p1->next, p2=p2->next) {
//    if (p1 == nullptr && p2 == nullptr)
//      return true; //at end of each
//    if (p1 == nullptr || p2 == nullptr)
//      return false; //at end of one but not the other
//    if (p1->value != p2->value)
//      return false;
//  }
//  //terminate via code in loop
//}
//
//
//TEST_F(Quiz2, relation) {
//  ASSERT_EQ('=',relation("",""));
//  ASSERT_EQ('>',relation("a",""));
//  ASSERT_EQ('<',relation("","a"));
//  ASSERT_EQ('=',relation("ant","ant"));
//  ASSERT_EQ('<',relation("ant","anteater"));
//  ASSERT_EQ('>',relation("anteater","ant"));
//  ASSERT_EQ('>',relation("ant","anT"));
//  ASSERT_EQ('<',relation("anT","ant"));
//  ASSERT_EQ('>',relation("x","ant"));
//}
//
//
//TEST_F(Quiz2, remove_ascending_i) {
//  LN<int>* test = build_linked_list_fast(new int[0]{},0);
//  remove_ascending_i(test);
//  ASSERT_EQ(nullptr, test);
//
//  test = build_linked_list_fast(new int[4]{5,7,4,8},4);
//  remove_ascending_i(test);
//  ASSERT_EQ(*build_linked_list_fast(new int[2]{7,8},2), *test);
//
//  test = build_linked_list_fast(new int[7]{5,3,2,7,8,4,2},7);
//  remove_ascending_i(test);
//  ASSERT_EQ(*build_linked_list_fast(new int[5]{5,3,8,4,2},5), *test);
//
//  test = build_linked_list_fast(new int[7]{5,6,7,8,7,6,5},7);
//  remove_ascending_i(test);
//  ASSERT_EQ(*build_linked_list_fast(new int[4]{8,7,6,5},4), *test);
//
//  test = build_linked_list_fast(new int[4]{5,6,7,8},4);
//  remove_ascending_i(test);
//  ASSERT_EQ(*build_linked_list_fast(new int[1]{8},1), *test);
//
//  test = build_linked_list_fast(new int[8]{8,5,3,1,2,3,2,1},8);
//  remove_ascending_i(test);
//  ASSERT_EQ(*build_linked_list_fast(new int[6]{8,5,3,3,2,1},6), *test);
//
//  test = build_linked_list_fast(new int[6]{8,5,3,1,2,3},6);
//  remove_ascending_i(test);
//  ASSERT_EQ(*build_linked_list_fast(new int[4]{8,5,3,3},4), *test);
//
//  test = build_linked_list_fast(new int[6]{8,5,3,1,2,2},6);
//  remove_ascending_i(test);
//  ASSERT_EQ(*build_linked_list_fast(new int[5]{8,5,3,2,2},5), *test);
//
//  test = build_linked_list_fast(new int[5]{5,4,3,2,1},5);
//  remove_ascending_i(test);
//  ASSERT_EQ(*build_linked_list_fast(new int[5]{5,4,3,2,1},5), *test);
//}
//
//
//TEST_F(Quiz2, remove_ascending_r) {
//    LN<int>* test = build_linked_list_fast(new int[0]{},0);
//    remove_ascending_r(test);
//    ASSERT_EQ(nullptr, test);
//
//    test = build_linked_list_fast(new int[4]{5,7,4,8},4);
//    remove_ascending_r(test);
//    ASSERT_EQ(*build_linked_list_fast(new int[2]{7,8},2), *test);
//
//    test = build_linked_list_fast(new int[7]{5,3,2,7,8,4,2},7);
//    remove_ascending_r(test);
//    ASSERT_EQ(*build_linked_list_fast(new int[5]{5,3,8,4,2},5), *test);
//
//    test = build_linked_list_fast(new int[7]{5,6,7,8,7,6,5},7);
//    remove_ascending_r(test);
//    ASSERT_EQ(*build_linked_list_fast(new int[4]{8,7,6,5},4), *test);
//
//    test = build_linked_list_fast(new int[4]{5,6,7,8},4);
//    remove_ascending_r(test);
//    ASSERT_EQ(*build_linked_list_fast(new int[1]{8},1), *test);
//
//    test = build_linked_list_fast(new int[8]{8,5,3,1,2,3,2,1},8);
//    remove_ascending_r(test);
//    ASSERT_EQ(*build_linked_list_fast(new int[6]{8,5,3,3,2,1},6), *test);
//
//    test = build_linked_list_fast(new int[6]{8,5,3,1,2,3},6);
//    remove_ascending_r(test);
//    ASSERT_EQ(*build_linked_list_fast(new int[4]{8,5,3,3},4), *test);
//
//    test = build_linked_list_fast(new int[6]{8,5,3,1,2,2},6);
//    remove_ascending_r(test);
//    ASSERT_EQ(*build_linked_list_fast(new int[5]{8,5,3,2,2},5), *test);
//
//    test = build_linked_list_fast(new int[5]{5,4,3,2,1},5);
//    remove_ascending_r(test);
//    ASSERT_EQ(*build_linked_list_fast(new int[5]{5,4,3,2,1},5), *test);
//}
//
//
//int main(int argc, char **argv) {
//  ::testing::InitGoogleTest(&argc, argv);
//  return RUN_ALL_TESTS();
//}
