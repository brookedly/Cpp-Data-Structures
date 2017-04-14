#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include "q2solution.hpp"


void test_relation(const std::string& s1, const std::string& s2) {
  std::cout << "  \"" + s1 + "\" " <<  relation(s1,s2) << " \"" + s2 + "\"" << std::endl;
}


template<class T>
LN<T>* build_linked_list_fast(T values[], int length) {  //O(N)
  if (length == 0)
    return nullptr;

  LN<T>* front      = new LN<T>(values[0]);
  LN<T>* rear_cache = front;
  for (int i=1; i<length; ++i)
    rear_cache = rear_cache->next = new LN<T>(values[i]);

  return front;
}


int main() {
  try{
    //Testing relation
    std::cout << "Testing relation" << std::endl;
    test_relation("","");
    test_relation("a","");
    test_relation("","a");
    test_relation("ant","ant");
    test_relation("ant","anteater");
    test_relation("anteater","ant");
    test_relation("x","ant");
    test_relation("ant","anT");
    test_relation("anT","ant");
    test_relation("ant","anteater");


    //Testing remove_ascending_i
    std::cout << std::endl << std::endl << "Testing remove_ascending_i" << std::endl;
    LN<int>* test = build_linked_list_fast(new int[0]{},0);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_i(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[4]{5,7,4,8},4);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_i(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[7]{5,3,2,7,8,4,2},7);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_i(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[7]{5,6,7,8,7,6,5},7);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_i(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[4]{5,6,7,8},4);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_i(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[8]{8,5,3,1,2,3,2,1},8);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_i(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[6]{8,5,3,1,2,3},6);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_i(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[6]{8,5,3,1,2,2},6);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_i(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[5]{5,4,3,2,1},5);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_i(test);
    std::cout << "After removal : " << test << std::endl;


    //Testing remove_ascending_r
    std::cout << std::endl << std::endl << "Testing remove_ascending_r" << std::endl;
    test = build_linked_list_fast(new int[0]{},0);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_r(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[4]{5,7,4,8},4);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_r(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[7]{5,3,2,7,8,4,2},7);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_r(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[7]{5,6,7,8,7,6,5},7);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_r(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[4]{5,6,7,8},4);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_r(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[8]{8,5,3,1,2,3,2,1},8);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_r(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[6]{8,5,3,1,2,3},6);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_r(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[6]{8,5,3,1,2,2},6);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_r(test);
    std::cout << "After removal : " << test << std::endl;

    test = build_linked_list_fast(new int[5]{5,4,3,2,1},5);
    std::cout << std::endl << "Before removal: " << test << std::endl;
    remove_ascending_r(test);
    std::cout << "After removal : " << test << std::endl;

  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  return 0;
}



/*
Here is the output that the driver should produce:
Testing relation
  "" = ""
  "a" > ""
  "" < "a"
  "ant" = "ant"
  "ant" < "anteater"
  "anteater" > "ant"
  "x" > "ant"
  "ant" > "anT"
  "anT" < "ant"
  "ant" < "anteater"


Testing remove_ascending_i

Before removal: nullptr
After removal : nullptr

Before removal: 5->7->4->8->nullptr
After removal : 7->8->nullptr

Before removal: 5->3->2->7->8->4->2->nullptr
After removal : 5->3->8->4->2->nullptr

Before removal: 5->6->7->8->7->6->5->nullptr
After removal : 8->7->6->5->nullptr

Before removal: 5->6->7->8->nullptr
After removal : 8->nullptr

Before removal: 8->5->3->1->2->3->2->1->nullptr
After removal : 8->5->3->3->2->1->nullptr

Before removal: 8->5->3->1->2->3->nullptr
After removal : 8->5->3->3->nullptr

Before removal: 8->5->3->1->2->2->nullptr
After removal : 8->5->3->2->2->nullptr

Before removal: 5->4->3->2->1->nullptr
After removal : 5->4->3->2->1->nullptr


Testing remove_ascending_r

Before removal: nullptr
After removal : nullptr

Before removal: 5->7->4->8->nullptr
After removal : 7->8->nullptr

Before removal: 5->3->2->7->8->4->2->nullptr
After removal : 5->3->8->4->2->nullptr

Before removal: 5->6->7->8->7->6->5->nullptr
After removal : 8->7->6->5->nullptr

Before removal: 5->6->7->8->nullptr
After removal : 8->nullptr

Before removal: 8->5->3->1->2->3->2->1->nullptr
After removal : 8->5->3->3->2->1->nullptr

Before removal: 8->5->3->1->2->3->nullptr
After removal : 8->5->3->3->nullptr

Before removal: 8->5->3->1->2->2->nullptr
After removal : 8->5->3->2->2->nullptr

Before removal: 5->4->3->2->1->nullptr
After removal : 5->4->3->2->1->nullptr
*/
