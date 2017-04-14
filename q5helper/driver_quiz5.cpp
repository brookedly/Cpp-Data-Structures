#include "q5solution.hpp"
#include "ics46goody.hpp"

int main() {
  try {
    //Test maximum

    //Uncomment the statement below (and comment the load/at/build_NTN_tree
    //  to enter the tree by hand
    //NTN<int> temp = build_NTN_tree("","root");

    int load1[] = {8,0};
    int at = 0;
    NTN<int> temp = build_NTN_tree(load1,at);
    std::cout << "Tree =" << std::endl;
    print_NTN_tree(temp,"");
    std::cout << "Maximum in Tree: " << maximum(temp) << std::endl;

    //How to read the values/instructions in the load2[] values array
    //See the printed tree
    //root is 1; it has 2 children;
    //  its first child is 5; it has 3 children;
    //    its first  child is 6; it has 0 children;
    //    its second child is 4; it has 0 children;
    //    its third  child is 2; it has 0 children;
    //  its second child is 7; it has 3 children;
    //    its first  child is 4; it has 0 children;
    //    its second child is 10 it has 0 children;
    //    its third  child is 3; it has 0 children;
    int load2[] = {1,2,5,3,6,0,4,0,2,0,7,3,4,0,10,0,3,0};
    at = 0;
    temp = build_NTN_tree(load2,at);
    std::cout << std::endl << "Tree =" << std::endl;
    print_NTN_tree(temp,"");
    std::cout << "Maximum in Tree: " << maximum(temp) << std::endl;

    int load3[] = {1,2,12,3,6,0,4,0,2,0,7,3,4,0,6,0,3,0};
    at = 0;
    temp = build_NTN_tree(load3,at);
    std::cout << std::endl << "Tree =" << std::endl;
    print_NTN_tree(temp,"");
    std::cout << "Maximum in Tree: " << maximum(temp) << std::endl << std::endl;


    //Test longest_word
    //Preload with the words (enter others as prompted)
    DTN root_DTN;
    std::cout << "adding ante" << std::endl;
    add_a_word(root_DTN,"ante");
    std::cout << "longest word = " << longest_word(root_DTN) << std::endl;

    std::cout << "adding anteater" << std::endl;
    add_a_word(root_DTN,"anteater");
    std::cout << "longest word = " << longest_word(root_DTN) << std::endl;

    std::cout << "adding anthem" << std::endl;
    add_a_word(root_DTN,"anthem");
    std::cout << "longest word = " << longest_word(root_DTN) << std::endl;

    std::cout << "adding anthebellum" << std::endl;
    add_a_word(root_DTN,"antebellum");
    std::cout << "longest word = " << longest_word(root_DTN) << std::endl;

    std::cout << "adding anteatersforics46" << std::endl;
    add_a_word(root_DTN,"aneatersforics46");

    std::cout << "Digital Tree of words (marked with *)" << std::endl;
    print_DTN_tree(root_DTN,"");
    std::cout << "longest word = " << longest_word(root_DTN) << std::endl;

    while (true) {
      std::string word = ics::prompt_string("Enter word to add (QUIT to quit)");
      if (word == "QUIT")
        break;
      add_a_word(root_DTN,word);
    }

    std::cout << "Digital Tree of words (marked with *)" << std::endl;
    print_DTN_tree(root_DTN,"");

    std::cout << "longest word = " << longest_word(root_DTN) << std::endl;

  } catch (ics::IcsError& e) {
    std::cout << e.what() << std::endl;
  }
  return 0;
}

/* Driver should print the following
Tree =
8
Maximum in Tree: 8

Tree =
1
  5
    6
    4
    2
  7
    4
    10
    3
Maximum in Tree: 10

Tree =
1
  12
    6
    4
    2
  7
    4
    6
    3
Maximum in Tree: 12

adding ante
longest word = ante
adding anteater
longest word = anteater
adding anthem
longest word = anteater
adding anthebellum
longest word = antebellum
adding anteatersforics46
Digital Tree of words (marked with *)

  a
    an
      ant
        ante*
          antea
            anteat
              anteate
                anteater*
          anteb
            antebe
              antebel
                antebell
                  antebellu
                    antebellum*
        anth
          anthe
            anthem*
      ane
        anea
          aneat
            aneate
              aneater
                aneaters
                  aneatersf
                    aneatersfo
                      aneatersfor
                        aneatersfori
                          aneatersforic
                            aneatersforics
                              aneatersforics4
                                aneatersforics46*
longest word = aneatersforics46
Enter word to add (QUIT to quit):
   ...you can enter more words here, and then QUIT, then see the longest word
   ...maybe it would be better to first remove the code that adds aneatersforics46,
   ...becuase that words is so big
 */