// Submitter: brookedl(Ly, Brooke)
// Partner  : jiayueg(Gao, Isley)
// We certify that we worked cooperatively on this programming
//   assignment, according to the rules for pair programming

#ifndef LINKED_QUEUE_HPP_
#define LINKED_QUEUE_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
using namespace std;


namespace ics {


template<class T> class LinkedQueue {
  public:
    //Destructor/Constructors
    ~LinkedQueue();

    LinkedQueue          ();
    LinkedQueue          (const LinkedQueue<T>& to_copy);
    explicit LinkedQueue (const std::initializer_list<T>& il);

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    explicit LinkedQueue (const Iterable& i);


    //Queries
    bool empty      () const;
    int  size       () const;
    T&   peek       () const;
    std::string str () const; //supplies useful debugging information; contrast to operator <<


    //Commands
    int  enqueue (const T& element);
    T    dequeue ();
    void clear   ();

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    int enqueue_all (const Iterable& i);


    //Operators
    LinkedQueue<T>& operator = (const LinkedQueue<T>& rhs);
    bool operator == (const LinkedQueue<T>& rhs) const;
    bool operator != (const LinkedQueue<T>& rhs) const;

    template<class T2>
    friend std::ostream& operator << (std::ostream& outs, const LinkedQueue<T2>& q);

  private:
    class LN;

  public:
    class Iterator {
      public:
        //Private constructor called in begin/end, which are friends of LinkedQueue<T>
        ~Iterator();
        T           erase();
        std::string str  () const;
        LinkedQueue<T>::Iterator& operator ++ ();
        LinkedQueue<T>::Iterator  operator ++ (int);
        bool operator == (const LinkedQueue<T>::Iterator& rhs) const;
        bool operator != (const LinkedQueue<T>::Iterator& rhs) const;
        T& operator *  () const;
        T* operator -> () const;
        friend std::ostream& operator << (std::ostream& outs, const LinkedQueue<T>::Iterator& i) {
          outs << i.str(); //Use the same meaning as the debugging .str() method
          return outs;
        }
        friend Iterator LinkedQueue<T>::begin () const;
        friend Iterator LinkedQueue<T>::end   () const;

      private:
        //If can_erase is false, current indexes the "next" value (must ++ to reach it)
        LN*             prev = nullptr;  //if nullptr, current at front of list
        LN*             current;         //current == prev->next (if prev != nullptr)
        LinkedQueue<T>* ref_queue;
        int             expected_mod_count;
        bool            can_erase = true;

        //Called in friends begin/end
        Iterator(LinkedQueue<T>* iterate_over, LN* initial);
    };


    Iterator begin () const;
    Iterator end   () const;


  private:
    class LN {
      public:
        LN ()                      {}
        LN (const LN& ln)          : value(ln.value), next(ln.next){}
        LN (T v,  LN* n = nullptr) : value(v), next(n){}

        T   value;
        LN* next = nullptr;
    };


    LN* front     =  nullptr;
    LN* rear      =  nullptr;
    int used      =  0;            //Cache the number of values in linked list
    int mod_count =  0;            //For sensing all concurrent modifications

    //Helper methods
    void delete_list(LN*& front);  //Deallocate all LNs, and set front's argument to nullptr;
};


////////////////////////////////////////////////////////////////////////////////
//
//LinkedQueue class and related definitions

//Destructor/Constructors
template<class T>
LinkedQueue<T>::~LinkedQueue() {
    clear();

}


template<class T>
LinkedQueue<T>::LinkedQueue() {
}


template<class T>
LinkedQueue<T>::LinkedQueue(const LinkedQueue<T>& to_copy)
{
    clear();
//    for (auto x : to_copy)
//        enqueue(x);
    *this = to_copy;
}


template<class T>
LinkedQueue<T>::LinkedQueue(const std::initializer_list<T>& il)
{
    for (auto x : il)
        enqueue(x);
}


template<class T>
template<class Iterable>
LinkedQueue<T>::LinkedQueue(const Iterable& i)
        :used(i.size())
{
        for (auto x : i)
            enqueue(x);
}


////////////////////////////////////////////////////////////////////////////////
//
//Queries

template<class T>
bool LinkedQueue<T>::empty() const {
    return used == 0;
}


template<class T>
int LinkedQueue<T>::size() const {
    return used;
}


template<class T>
T& LinkedQueue<T>::peek () const {
    if (this->empty())
        throw EmptyError("LinkedQueue::peek");
    return front->value;
}


template<class T>
std::string LinkedQueue<T>::str() const {
    std::ostringstream answer;
    answer << "LinkedQueue[";
    if (used != 0)
    {
        for (auto f = front; f != rear; f = f->next)
        {
            answer << f->value << "->";
        }
        answer << rear->value;
    }
    answer << "(used=" << used << ",front=" << front << ",rear=" << rear << ",mod_count=" << mod_count << ")";
    return answer.str();
}


////////////////////////////////////////////////////////////////////////////////
//
//Commands

template<class T>
int LinkedQueue<T>::enqueue(const T& element) {
    if (front == nullptr){
        front = rear = new LN(element, nullptr);
    }
    else{
         rear = rear->next = new LN(element, nullptr);
    }
    ++mod_count;
    ++used;
    return 1;
}


template<class T>
    T LinkedQueue<T>::dequeue() {
        if (this->empty())
            throw EmptyError("LinkedQueue::dequeue");
        LN *x = front;
        T answer = front->value;
        front = front->next;
        if (front == nullptr)
        {
            front = rear = nullptr;
        }
        delete x;
        ++mod_count;
        --used;
        return answer;

}


template<class T>
void LinkedQueue<T>::clear() {
    delete_list(front);
}


template<class T>
template<class Iterable>
int LinkedQueue<T>::enqueue_all(const Iterable& i) {
        int count = 0;
        used++;
        for (const T& v : i)
            count += enqueue(v);
        return count;
}


//////////////////////////////////////////////////////////////////////////////

//Operators

template<class T>
LinkedQueue<T>& LinkedQueue<T>::operator = (const LinkedQueue<T>& rhs) {
    if (this == &rhs)
        return *this;
    clear();
    //clears the left hand queue
    LN* f = front;
    LN *p = rhs.front;
    for (; p != nullptr; p = p->next) {
        if (f == nullptr) {
            enqueue(p->value);
        } else {
            f->value = p->value;
            f = f->next;
        }
    }
    ++mod_count;
    return *this;
//    Pattis' ArrayQueue did not decrement the used variable
}


template<class T>
bool LinkedQueue<T>::operator == (const LinkedQueue<T>& rhs) const {
    if (this == &rhs)
        return true;
    int used = this->size();
    if (used != rhs.size()) {
        return false;
    }
    else {
        LN *p = rhs.front;
        for (auto v = front; v != rear; v = v->next, p = p->next) {
            if (v->value != p->value)
                return false;
        }
    }
    return true;
}

template<class T>
bool LinkedQueue<T>::operator != (const LinkedQueue<T>& rhs) const {
    return !(*this == rhs);
}


template<class T>
std::ostream& operator << (std::ostream& outs, const LinkedQueue<T>& q) {
    outs << "queue[";
    if (!q.empty()) {
        for (auto i = q.front; i != q.rear; i = i->next)
        {
            outs << i->value << ",";
        }
        outs << q.rear->value;
    }
    outs << "]:rear";
    return outs;
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors

template<class T>
auto LinkedQueue<T>::begin () const -> LinkedQueue<T>::Iterator {
    return Iterator(const_cast<LinkedQueue<T>*>(this),front);

}

template<class T>
auto LinkedQueue<T>::end () const -> LinkedQueue<T>::Iterator {
    return Iterator(const_cast<LinkedQueue<T>*>(this), rear);

}


////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

template<class T>
void LinkedQueue<T>::delete_list(LN*& front) {
    while (front != nullptr) {
        LN *x = front;
        front = front->next;
        delete x;
        mod_count++;
        used--;
    }
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

template<class T>
LinkedQueue<T>::Iterator::Iterator(LinkedQueue<T>* iterate_over, LN* initial)
: current(initial), ref_queue(iterate_over), expected_mod_count(ref_queue->mod_count)
{
}


template<class T>
LinkedQueue<T>::Iterator::~Iterator()
{}


template<class T>
T LinkedQueue<T>::Iterator::erase() {
    if (expected_mod_count != ref_queue->mod_count)
        throw ConcurrentModificationError("LinkedQueue::Iterator::erase");
    if (!can_erase)
        throw CannotEraseError("LinkedQueue::Iterator::erase Iterator cursor already erased");
    if (current == nullptr)
        throw CannotEraseError("LinkedQueue::Iterator::erase Iterator cursor beyond data structure");

    can_erase = false;
    T to_return  = current->value;
//    if (prev == nullptr)
//    {
//        ref_queue->front = current->next;
//        delete current;
//        current = ref_queue->front;
//    }
//    else {
//        prev->next = current->next;
//        delete current;
//        current = prev->next;
//    }

    expected_mod_count = ref_queue->mod_count;
    return to_return;
}


template<class T>
std::string LinkedQueue<T>::Iterator::str() const {

    std::ostringstream answer;
    answer << ref_queue->str() << "(current=" << current << ",expected_mod_count=" << expected_mod_count << ",can_erase=" << can_erase << ")";
    return answer.str();
}


template<class T>
auto LinkedQueue<T>::Iterator::operator ++ () -> LinkedQueue<T>::Iterator& {
    if (expected_mod_count != ref_queue->mod_count)
        throw ConcurrentModificationError("LinkedQueue::Iterator::operator ++");

    if (current == ref_queue->rear)
        return *this;

    if (can_erase) {
        prev = current;
        current = current->next;
    }
    else
        can_erase = true;

    return *this;
}


template<class T>
auto LinkedQueue<T>::Iterator::operator ++ (int) -> LinkedQueue<T>::Iterator {
    if (expected_mod_count != ref_queue->mod_count)
        throw ConcurrentModificationError("LinkedQueue::Iterator::operator ++(int)");

    if (current == ref_queue->rear)
        return *this;

    Iterator to_return(*this);
    if (can_erase)
    {
        prev = current;
        current = current->next;
    }
    else
        can_erase = true;

    return to_return;
}


template<class T>
bool LinkedQueue<T>::Iterator::operator == (const LinkedQueue<T>::Iterator& rhs) const {

    const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
    if (rhsASI == 0)
        throw IteratorTypeError("LinkedQueue::Iterator::operator ==");
    if (expected_mod_count != ref_queue->mod_count)
        throw ConcurrentModificationError("LinkedQueue::Iterator::operator ==");
    if (ref_queue != rhsASI->ref_queue)
        throw ComparingDifferentIteratorsError("LinkedQueue::Iterator::operator ==");

    return current == rhsASI->current;
}


template<class T>
bool LinkedQueue<T>::Iterator::operator != (const LinkedQueue<T>::Iterator& rhs) const {

    const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
    if (rhsASI == 0)
        throw IteratorTypeError("LinkedQueue::Iterator::operator !=");
    if (expected_mod_count != ref_queue->mod_count)
        throw ConcurrentModificationError("LinkedQueue::Iterator::operator !=");
    if (ref_queue != rhsASI->ref_queue)
        throw ComparingDifferentIteratorsError("LinkedQueue::Iterator::operator !=");

    return current != rhsASI->current;
}


template<class T>
T& LinkedQueue<T>::Iterator::operator *() const {

    if (expected_mod_count != ref_queue->mod_count)
        throw ConcurrentModificationError("LinkedQueue::Iterator::operator *");
    if (current == nullptr || !can_erase) {
        std::ostringstream where;
        where << current
              << " when front = " << ref_queue->front
              << " and rear = " << ref_queue->rear;
        throw IteratorPositionIllegal("LinkedQueue::Iterator::operator * Iterator illegal: "+where.str());
    }
    return current->value;
}


template<class T>
T* LinkedQueue<T>::Iterator::operator ->() const {
    if (expected_mod_count != ref_queue->mod_count)
        throw ConcurrentModificationError("LinkedQueue::Iterator::operator ->");
    if (!can_erase || current== nullptr) {
        std::ostringstream where;
        where << current
              << " when front = " << ref_queue->front
              << " and rear = " << ref_queue->rear;
        throw IteratorPositionIllegal("LinkedQueue::Iterator::operator -> Iterator illegal: "+where.str());
    }
    return &(current->value);
}
}

#endif /* LINKED_QUEUE_HPP_ */
