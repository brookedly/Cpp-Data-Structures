#ifndef SOLUTION_HPP_
#define SOLUTION_HPP_

#include <string>
#include <iostream>
#include <fstream>
template<class T>
class LN {
  public:
    LN ()                        : next(nullptr){}
    LN (const LN<T>& ln)         : value(ln.value), next(ln.next){}
    LN (T v, LN<T>* n = nullptr) : value(v), next(n){}
    T      value;
    LN<T>* next;
};

//Simple way to print linked lists (useful for driver and debugging)
template<class T>
std::ostream& operator << (std::ostream& outs, LN<T>* l) {
  for (LN<T>* p = l; p!=nullptr; p=p->next)
    outs << p->value << "->";
  outs << "nullptr";
  return outs;
}

//Simple way to print linked lists given first node
//  (useful for driver and debugging)
template<class T>
std::ostream& operator << (std::ostream& outs, LN<T> l) {
  outs << l.value << "->";
  for (LN<T>* p = l.next; p!=nullptr; p=p->next)
    outs << p->value << "->";
  outs << "nullptr";
  return outs;
}


char relation (const std::string& s1, const std::string& s2) {
    if (s1.length() == 0 and s2.length() == 0 )
    {
        return '=';
    }
    else if (s1[0] == s2[0])
    {
        return relation(s1.substr(1, s1.length() ), s2.substr(1, s2.length()));
    }
    else if (s1[0] > s2[0]) {
        return '>';
    }
    else {
        return '<';
    }
}


template<class T>
void remove_ascending_i (LN<T>*& l) {
    LN<T> **p = &l;
    if ((*p) == nullptr) {
        return;
    }
    else {
        while ((*p)->next != nullptr) {
            if ((*p)->value < (*p)->next->value) {
                LN<T> *del_me = *p;
                (*p) = (*p)->next;
                delete del_me;
            } else {
                p = &((*p)->next);
            }
        }
    }
}

template<class T>
void remove_ascending_r (LN<T>*& l) {
//    LN<T> **p = &l;
//    if ((*p) == nullptr)
//    {
//        return;
//    }
//    else {
//        //while ((*p) != nullptr) {
//        if ((*p)->value < (*p)->next->value) {
//            LN<T> *to_delete = *p;
//            (*p) = (*p)->next;
//            delete to_delete;
//            p = &((*p)->next);
//            remove_ascending_r(*p);
//        } else {
//
//            p = &((*p)->next);
//            remove_ascending_r(*p);
//        }
//    }
}

#endif /* SOLUTION_HPP_ */
