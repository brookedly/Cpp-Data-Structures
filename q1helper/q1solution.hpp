#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

#include <string>
#include <iostream>
#include <fstream>
#include <math.h>            /* for atan2 and sqrt */
#include "ics46goody.hpp"
#include "ics_exceptions.hpp"
#include "array_queue.hpp"
#include "array_priority_queue.hpp"
#include "array_map.hpp"

class Point {
public:
  Point() : x(0), y(0) {} // Needed for pair
  Point(int x_val, int y_val) : x(x_val), y(y_val) {}
  friend bool operator == (const Point& p1, const Point& p2) {
    return p1.x == p2.x && p1.y == p2.y;
  }
  friend std::ostream& operator << (std::ostream& outs, const Point& p) {
    outs << "(" << p.x << "," << p.y << ")";
    return outs;
  }

  int x;
  int y;
};

//Helper Functions (you decide what is useful)
//Hint: I used helpers for sort_yo, sort_angle, points, and first_quad

//Problem #1a and #1b
template<class KEY,class T>
void swap (ics::ArrayMap<KEY,T>& m, KEY key1, KEY key2) {
    ics::ArraySet<std::string> change = m[key1];
    m[key1] = m[key2];
    m[key2] = change;
}


template<class KEY,class T>
void values_set_to_queue (const ics::ArrayMap<KEY,ics::ArraySet<T>>& m1,
                          ics::ArrayMap<KEY,ics::ArrayQueue<T>>&     m2) {
    for (auto k_v : m1)
    {
        for (auto each : k_v.second)
        {
            m2[k_v.first].enqueue(each);
        }
    }
}

typedef ics::pair<int,Point>  PointInput;

bool point_gt (const PointInput& a, const PointInput& b)
{
    if (a.second.y == b.second.y) {
        return a.first > b.first;
    }
    else {
        return a.second.y < b.second.y;
    }
}


////Problem #2a, #2b, #2c, and #2d
ics::ArrayQueue<ics::pair<int,Point>> sort_yo (const ics::ArrayMap<int,Point>& m) {
    ics::ArrayPriorityQueue<ics::pair<int,Point>> sorted(point_gt);
    ics::ArrayQueue<ics::pair<int,Point>> result;
    sorted.enqueue_all(m);
    result.enqueue_all(sorted);
    return result;
}

bool d(const PointInput& a, const PointInput& b)
{
    double distx = sqrt((a.second.x * a.second.x)+(a.second.y * a.second.y));
    double disty = sqrt((b.second.x * b.second.x)+(b.second.y * b.second.y));
    return distx > disty;
}

ics::ArrayQueue<Point> sort_distance (const ics::ArrayMap<int,Point>& m) {
    ics::ArrayPriorityQueue<ics::pair<int, Point>> sorted(d);
    ics::ArrayQueue<Point> result;
    sorted.enqueue_all(m);
    for (auto kv : sorted)
    {
        result.enqueue(Point(kv.second.x, kv.second.y));
    }
    return result;
}

bool gt (const PointInput& a, const PointInput& b) { return a.first < b.first; }

ics::ArrayQueue<Point> points (const ics::ArrayMap<int,Point>& m) {
     ics::ArrayPriorityQueue<PointInput> sorted(gt);
     ics::ArrayQueue<Point> answer;
     sorted.enqueue_all(m);
        for (auto kv : sorted)
        {
            answer.enqueue(kv.second);
        }
    return answer;
}

bool lt_angle(const ics::pair<int, double>& a, const ics::pair<int, double>& b )
{
    return a.second < b.second;
}

ics::ArrayQueue<ics::pair<int,double>> angles (const ics::ArrayMap<int,Point>& m) {
    ics::ArrayPriorityQueue<ics::pair<int, double>> sorted(lt_angle);
    ics::ArrayQueue<ics::pair<int, double>> answer;
    ics::pair<int, double> inside;
    for (auto kv : m)
    {
        inside.first = kv.first;
        inside.second = atan2(kv.second.y, kv.second.x);
        sorted.enqueue(inside);
    }
    for (auto k : sorted)
    {
        answer.enqueue(k);
    }
    return answer;
}

//Problem #3
ics::ArrayMap<char,ics::ArraySet<char>> follows(const std::string word) {
    ics::ArrayMap<char, ics::ArraySet<char>> result;
    int next1 = 1;
    int next2 = 2;
    for (int x = 0; x < word.size(); x++) {
        for (auto go = std:: max(word.begin(), word.begin() + x + next1);
             go < std:: min(word.end(), word.begin() + x + next2); ++go)
        {
            result[word[x]].insert(*go);
        }
    }
    return result;
}

//Problem #4a and #4b
ics::ArrayMap<std::string,int> got_called(const  ics::ArrayMap<std::string,ics::ArrayMap<std::string,int>>& calls) {
    ics::ArrayMap<std::string, int> answer;
    for (auto caller : calls)
        //caller has 2 attributes: FIRST is the caller and SECOND is the inner MAP
    {
        for (auto inner : caller.second)
        //inner has two attributes: FIRST is the callee and SECOND is the times called
        {
            if (answer.has_key(caller.first) == false)
                //if the answer map does not have 'a' then the callees aren't in the answer map
                //then go ahead and add them WITH their corresponding times called.
                //move onto caller 'b' who is already in the answer map, then goes to the else stmt
            {
                answer[inner.first] =  inner.second;
            }
            else {
                answer[inner.first] += inner.second;
            }
        }
    }
    return answer;
}

  ics::ArrayMap<std::string,ics::ArrayMap<std::string,int>> invert (const ics::ArrayMap<std::string,ics::ArrayMap<std::string,int>>& calls) {
    ics::ArrayMap<std::string, ics::ArrayMap<std::string,int>> answer;
      for (auto caller : calls)
      {
          for (auto inner : caller.second)
          {
              if (answer.has_key(inner.first) == false)
              {
                  answer[inner.first][caller.first] = inner.second;
              }
              else {
                  //if the callee exists already in the map, then add  the new caller
                  answer[inner.first].put(caller.first, inner.second);
              }
          }
      }
      return answer;
  }
#endif /* SOLUTION_HPP_ */


