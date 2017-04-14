#include <iostream>
#include <sstream>
#include "ics46goody.hpp"
#include "gtest/gtest.h"
#include "array_set.hpp"
#include "array_map.hpp"
#include "q1solution.hpp"

class Quiz1 : public ::testing::Test {
protected:
    virtual void SetUp()    {}
    virtual void TearDown() {}
};


TEST_F(Quiz1, swap) {
  typedef ics::ArraySet<std::string> string_set;
  ics::ArrayMap<std::string,string_set> names_set (
      {
       ics::pair<std::string,string_set>("Boy", string_set({"Mary","Betty","Mimsi"})),
       ics::pair<std::string,string_set>("Girl",string_set({"Peter","Joey","Joe","Carl"}))
      }
  );

  swap(names_set, std::string("Boy"), std::string("Girl"));

  ASSERT_EQ(string_set({"Peter","Joey","Joe","Carl"}), names_set["Boy"]);
  ASSERT_EQ(string_set({"Mary","Betty","Mimsi"}),      names_set["Girl"]);
}


TEST_F(Quiz1, values_set_to_queue) {
  typedef ics::ArrayQueue<std::string> string_queue;
  typedef ics::ArraySet<std::string>   string_set;
  ics::ArrayMap<std::string,string_set> names_set (
      {
       ics::pair<std::string,string_set>("Girl", string_set({"Mary","Betty","Mimsi"})),
       ics::pair<std::string,string_set>("Boy",  string_set({"Peter","Joey","Joe","Carl"}))
      }
  );

  ics::ArrayMap<std::string,string_queue> names_queue;
  values_set_to_queue(names_set, names_queue);
  ASSERT_EQ(string_set({"Peter","Joey","Joe","Carl"}), string_set(names_queue["Boy"]));
  ASSERT_EQ(string_set({"Mary","Betty","Mimsi"}),      string_set(names_set["Girl"]));

}


TEST_F(Quiz1, sort_yo) {
  typedef int                      ordinal;
  typedef ics::pair<ordinal,Point> op_entry;

  ics::ArrayMap<ordinal,Point> ps1(
    {
         op_entry(1,Point(1,1)),
         op_entry(2,Point(3,2)),
         op_entry(3,Point(3,-3)),
         op_entry(4,Point(-3,4)),
         op_entry(5,Point(-2,-2)),
         op_entry(6,Point(-2,1))
    }
  );
  ics::ArrayQueue<op_entry> ps1_answer(
    {
       op_entry(3,Point(3,-3)),
       op_entry(5,Point(-2,-2)),
       op_entry(6,Point(-2,1)),
       op_entry(1,Point(1,1)),
       op_entry(2,Point(3,2)),
       op_entry(4,Point(-3,4))
    }
  );
  ASSERT_EQ(ps1_answer, sort_yo(ps1));

  ics::ArrayMap<ordinal,Point> ps2(
    {
         op_entry(1,Point(0,5)),
         op_entry(2,Point(2,3)),
         op_entry(4,Point(-5,1)),
         op_entry(5,Point(-2,-2)),
         op_entry(6,Point(4,-2)),
         op_entry(3,Point(-3,3)),
         op_entry(7,Point(5,3)),
         op_entry(8,Point(2,-5))
    }
  );
  ics::ArrayQueue<op_entry> ps2_answer(
    {
      op_entry(8,Point(2,-5)),
      op_entry(6,Point(4,-2)),
      op_entry(5,Point(-2,-2)),
      op_entry(4,Point(-5,1)),
      op_entry(7,Point(5,3)),
      op_entry(3,Point(-3,3)),
      op_entry(2,Point(2,3)),
      op_entry(1,Point(0,5))
    }
  );
  ASSERT_EQ(ps2_answer, sort_yo(ps2));

}


TEST_F(Quiz1, sort_distance) {
  typedef int                      ordinal;
  typedef ics::pair<ordinal,Point> op_entry;

  ics::ArrayMap<ordinal,Point> ps1(
      {
          op_entry(3,Point(3,-3)),
          op_entry(5,Point(-2,-2)),
          op_entry(6,Point(-2,1)),
          op_entry(2,Point(3,2)),
          op_entry(1,Point(1,1)),
          op_entry(4,Point(-3,4))
      }
  );
  ics::ArrayQueue<Point> ps1_answer(
    {
       Point(-3,4),
       Point(3,-3),
       Point(3,2),
       Point(-2,-2),
       Point(-2,1),
       Point(1,1),
    }
  );
  ASSERT_EQ(ps1_answer, sort_distance(ps1));

  ics::ArrayMap<ordinal,Point> ps2(
    {
         op_entry(1,Point(0,5)),
         op_entry(2,Point(2,3)),
         op_entry(4,Point(-5,1)),
         op_entry(5,Point(-2,-2)),
         op_entry(6,Point(4,-2)),
         op_entry(3,Point(-3,3)),
         op_entry(7,Point(5,3)),
         op_entry(8,Point(2,-5))
    }
  );
  ics::ArrayQueue<Point> ps2_answer(
    {
      Point(5,3),
      Point(2,-5),
      Point(-5,1),
      Point(0,5),
      Point(4,-2),
      Point(-3,3),
      Point(2,3),
      Point(-2,-2),
    }
  );
  ASSERT_EQ(ps2_answer, sort_distance(ps2));
}


TEST_F(Quiz1, points) {
  typedef int                      ordinal;
  typedef ics::pair<ordinal,Point> op_entry;

  ics::ArrayMap<ordinal,Point> ps1(
    {
        op_entry(3,Point(3,-3)),
        op_entry(5,Point(-2,-2)),
        op_entry(6,Point(-2,1)),
        op_entry(2,Point(3,2)),
        op_entry(1,Point(1,1)),
        op_entry(4,Point(-3,4))
    }
  );
  ics::ArrayQueue<Point> ps1_answer(
    {
      Point(1,1),
      Point(3,2),
      Point(3,-3),
      Point(-3,4),
      Point(-2,-2),
      Point(-2,1),
    }
  );
  ASSERT_EQ(ps1_answer, points(ps1));

  ics::ArrayMap<ordinal,Point> ps2(
    {
        op_entry(1,Point(0,5)),
        op_entry(2,Point(2,3)),
        op_entry(4,Point(-5,1)),
        op_entry(5,Point(-2,-2)),
        op_entry(6,Point(4,-2)),
        op_entry(3,Point(-3,3)),
        op_entry(7,Point(5,3)),
        op_entry(8,Point(2,-5))
    }
  );
  ics::ArrayQueue<Point> ps2_answer(
    {
      Point(0,5),
      Point(2,3),
      Point(-3,3),
      Point(-5,1),
      Point(-2,-2),
      Point(4,-2),
      Point(5,3),
      Point(2,-5)
    }
  );
  ASSERT_EQ(ps2_answer, points(ps2));
}


TEST_F(Quiz1, angles) {
  typedef int                       ordinal;
  typedef ics::pair<ordinal,Point>  op_entry;
  typedef ics::pair<ordinal,double> ans_entry;

  ics::ArrayMap<ordinal,Point> ps1(
    {
        op_entry(3,Point(3,-3)),
        op_entry(5,Point(-2,-2)),
        op_entry(6,Point(-2,1)),
        op_entry(2,Point(3,2)),
        op_entry(1,Point(1,1)),
        op_entry(4,Point(-3,4))
    }
  );
  ics::ArrayQueue<ics::pair<ordinal,double>> ps1_answer(
    {
        ans_entry(5,atan2(-2,-2)),
        ans_entry(3,atan2(-3,3)),
        ans_entry(2,atan2(2,3)),
        ans_entry(1,atan2(1,1)),
        ans_entry(4,atan2(4,-3)),
        ans_entry(6,atan2(1,-2)),
    }
  );
  ASSERT_EQ(ps1_answer, angles(ps1));

  ics::ArrayMap<ordinal,Point> ps2(
    {
        op_entry(1,Point(0,5)),
        op_entry(2,Point(2,3)),
        op_entry(4,Point(-5,1)),
        op_entry(5,Point(-2,-2)),
        op_entry(6,Point(4,-2)),
        op_entry(3,Point(-3,3)),
        op_entry(7,Point(5,3)),
        op_entry(8,Point(2,-5))
    }
  );
  ics::ArrayQueue<ics::pair<ordinal,double>> ps2_answer(
    {
        ans_entry(5,atan2(-2,-2)),
        ans_entry(8,atan2(-5,2)),
        ans_entry(6,atan2(-2,4)),
        ans_entry(7,atan2(3,5)),
        ans_entry(2,atan2(3,2)),
        ans_entry(1,atan2(5,0)),
        ans_entry(3,atan2(3,-3)),
        ans_entry(4,atan2(1,-5))
    }
  );
  ASSERT_EQ(ps2_answer, angles(ps2));

}


TEST_F(Quiz1, near) {
  typedef ics::ArraySet<char>             char_set;
  typedef ics::ArrayMap<char,char_set>    map;
  typedef ics::pair<char,char_set>        entry;

  ASSERT_EQ(map({
                    entry({'b', char_set({'o'})}),
                    entry({'o', char_set({'o','k'})}),
                    entry({'k', char_set({'k','e'})}),
                    entry({'e', char_set({'e','p','r'})}),
                    entry({'p', char_set({'e'})})
                 }
                ), follows("bookkeeper"));

  ASSERT_EQ(map({
                    entry({'a', char_set({'t'})}),
                    entry({'t', char_set({'t','e','s'})}),
                    entry({'e', char_set({'m'})}),
                    entry({'m', char_set({'p'})}),
                    entry({'p', char_set({'t'})})
                }
  ), follows("attempts"));

  ASSERT_EQ(map({
                    entry({'m', char_set({'o'})}),
                    entry({'o', char_set({'r'})}),
                    entry({'r', char_set({'a','i'})}),
                    entry({'a', char_set({'t'})}),
      entry({'t', char_set({'o'})}),
            entry({'i', char_set({'u'})}),
            entry({'u', char_set({'m'})}),
}
), follows("moratorium"));

}


TEST_F(Quiz1, got_called) {
  typedef std::string                       caller;
  typedef std::string                       callee;
  typedef ics::ArrayMap<callee,int>         call_count;
  typedef ics::ArrayMap<caller, call_count> db;
  typedef ics::pair<callee,int>             cc_entry;
  typedef ics::pair<caller,call_count>      db_entry;

  db calls1 (
      {
        db_entry("a",call_count({cc_entry("b",2),cc_entry("c",1)})),
        db_entry("b",call_count({cc_entry("a",3),cc_entry("c",1)})),
        db_entry("c",call_count({cc_entry("a",1),cc_entry("d",2)}))
      }
  );
  ics::ArrayMap<callee,int> calls1_answer (
      {
        cc_entry("b",2),
        cc_entry("c",2),
        cc_entry("a",4),
        cc_entry("d",2)
      }
  );
  ASSERT_EQ(calls1_answer,got_called(calls1));

  db calls2 (
      {
        db_entry("a",call_count({cc_entry("b",2),cc_entry("c",1),cc_entry("e",2)})),
        db_entry("b",call_count({cc_entry("a",3),cc_entry("c",1)})),
        db_entry("c",call_count({cc_entry("a",1),cc_entry("d",2),cc_entry("e",6)})),
        db_entry("e",call_count({cc_entry("a",3),cc_entry("c",2),cc_entry("d",1)}))
      }
  );
  ics::ArrayMap<callee,int> calls2_answer (
      {
        cc_entry("b",2),
        cc_entry("c",4),
        cc_entry("e",8),
        cc_entry("a",7),
        cc_entry("d",3),
      }
  );
  ASSERT_EQ(calls1_answer,got_called(calls1));
}


TEST_F(Quiz1, invert) {
  typedef std::string                       caller;
  typedef std::string                       callee;
  typedef ics::ArrayMap<callee,int>         call_count;
  typedef ics::ArrayMap<caller, call_count> db;
  typedef ics::pair<callee,int>             cc_entry;
  typedef ics::pair<caller,call_count>      db_entry;

  db calls1 (
      {
        db_entry("a",call_count({cc_entry("b",2),cc_entry("c",1)})),
        db_entry("b",call_count({cc_entry("a",3),cc_entry("c",1)})),
        db_entry("c",call_count({cc_entry("a",1),cc_entry("d",2)}))
      }
  );
  db calls1_answer (
      {
        db_entry("b",call_count({cc_entry("a",2)})),
        db_entry("c",call_count({cc_entry("a",1),cc_entry("b",1)})),
        db_entry("a",call_count({cc_entry("b",3),cc_entry("c",1)})),
        db_entry("d",call_count({cc_entry("c",2)})),
      }
  );
  ASSERT_EQ(calls1_answer,invert(calls1));

  db calls2 (
      {
        db_entry("a",call_count({cc_entry("b",2),cc_entry("c",1),cc_entry("e",2)})),
        db_entry("b",call_count({cc_entry("a",3),cc_entry("c",1)})),
        db_entry("c",call_count({cc_entry("a",1),cc_entry("d",2),cc_entry("e",6)})),
        db_entry("e",call_count({cc_entry("a",3),cc_entry("c",2),cc_entry("d",1)}))
      }
  );
  db calls2_answer (
      {
        db_entry("b",call_count({cc_entry("a",2)})),
        db_entry("c",call_count({cc_entry("a",1),cc_entry("b",1),cc_entry("e",2)})),
        db_entry("e",call_count({cc_entry("a",2),cc_entry("c",6)})),
        db_entry("a",call_count({cc_entry("b",3),cc_entry("c",1),cc_entry("e",3)})),
        db_entry("d",call_count({cc_entry("c",2),cc_entry("e",1)}))

      }
  );
  ASSERT_EQ(calls2_answer,invert(calls2));
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
