// Submitter: brookedl(Ly, Brooke)
// Partner  : jiayueg(Gao, Isley)
// We certify that we worked cooperatively on this programming
//   assignment, according to the rules for pair programming

#ifndef LINKED_SET_HPP_
#define LINKED_SET_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"


namespace ics {


template<class T> class LinkedSet {
  public:
    //Destructor/Constructors
    ~LinkedSet();

    LinkedSet          ();
    explicit LinkedSet (int initialLength);
    LinkedSet          (const LinkedSet<T>& to_copy);
    explicit LinkedSet (const std::initializer_list<T>& il);

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    explicit LinkedSet (const Iterable& i);


    //Queries
    bool empty      () const;
    int  size       () const;
    bool contains   (const T& element) const;
    std::string str () const; //supplies useful debugging information; contrast to operator <<

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    bool contains_all (const Iterable& i) const;


    //Commands
    int  insert (const T& element);
    int  erase  (const T& element);
    void clear  ();

    //Iterable class must support "for" loop: .begin()/.end() and prefix ++ on returned result

    template <class Iterable>
    int insert_all(const Iterable& i);

    template <class Iterable>
    int erase_all(const Iterable& i);

    template<class Iterable>
    int retain_all(const Iterable& i);


    //Operators
    LinkedSet<T>& operator = (const LinkedSet<T>& rhs);
    bool operator == (const LinkedSet<T>& rhs) const;
    bool operator != (const LinkedSet<T>& rhs) const;
    bool operator <= (const LinkedSet<T>& rhs) const;
    bool operator <  (const LinkedSet<T>& rhs) const;
    bool operator >= (const LinkedSet<T>& rhs) const;
    bool operator >  (const LinkedSet<T>& rhs) const;

    template<class T2>
    friend std::ostream& operator << (std::ostream& outs, const LinkedSet<T2>& s);



  private:
    class LN;

  public:
    class Iterator {
      public:
        //Private constructor called in begin/end, which are friends of LinkedSet<T>
        ~Iterator();
        T           erase();
        std::string str  () const;
        LinkedSet<T>::Iterator& operator ++ ();
        LinkedSet<T>::Iterator  operator ++ (int);
        bool operator == (const LinkedSet<T>::Iterator& rhs) const;
        bool operator != (const LinkedSet<T>::Iterator& rhs) const;
        T& operator *  () const;
        T* operator -> () const;
        friend std::ostream& operator << (std::ostream& outs, const LinkedSet<T>::Iterator& i) {
          outs << i.str(); //Use the same meaning as the debugging .str() method
          return outs;
        }
        friend Iterator LinkedSet<T>::begin () const;
        friend Iterator LinkedSet<T>::end   () const;

      private:
        //If can_erase is false, current indexes the "next" value (must ++ to reach it)
        LN*           current;  //if can_erase is false, this value is unusable
        LinkedSet<T>* ref_set;
        int           expected_mod_count;
        bool          can_erase = true;

        //Called in friends begin/end
        Iterator(LinkedSet<T>* iterate_over, LN* initial);
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
        LN* next   = nullptr;
    };


    LN* front     = new LN();
    LN* trailer   = front;         //Always point to special trailer LN
    int used      =  0;            //Cache the number of values in linked list
    int mod_count = 0;             //For sensing concurrent modification

    //Helper methods
    int  erase_at   (LN* p);
    void delete_list(LN*& front);  //Deallocate all LNs (but trailer), and set front's argument to trailer;
};

////////////////////////////////////////////////////////////////////////////////
//
//LinkedSet class and related definitions
//Destructor/Constructors

template<class T>
LinkedSet<T>::~LinkedSet() {
    clear();
}


template<class T>
LinkedSet<T>::LinkedSet() {
}


template<class T>
LinkedSet<T>::LinkedSet(const LinkedSet<T>& to_copy) : used(to_copy.used) {
    clear();
    //    for (auto x : to_copy)
//        insert(x);
//    insert_all(to_copy);
    *this= to_copy;
}


template<class T>
LinkedSet<T>::LinkedSet(const std::initializer_list<T>& il) {
    for (auto x: il)
        insert(x);

}


template<class T>
template<class Iterable>
LinkedSet<T>::LinkedSet(const Iterable& i) {
    for (auto x: i)
        insert(x);

}


////////////////////////////////////////////////////////////////////////////////
//
//Queries

template<class T>
bool LinkedSet<T>::empty() const {
    return used == 0;
}


template<class T>
int LinkedSet<T>::size() const {
    return used;
}


template<class T>
bool LinkedSet<T>::contains (const T& element) const {
    for(auto f=front; f!= nullptr; f=f->next){
        if(f->value == element)
            return true;
    }
    return false;
}


template<class T>
std::string LinkedSet<T>::str() const {
    std::ostringstream answer;
    answer << "LinkedSet[";

    if (used != 0) {
        for (auto f = front; f != nullptr; f = f->next)
        {
            answer << f->value << "->";
        }
    }
    answer << "](used=" << used << ",front=" << front << ",mod_count=" << mod_count << ")";
    return answer.str();
}


template<class T>
template<class Iterable>
bool LinkedSet<T>::contains_all (const Iterable& i) const {
    for (auto x : i) {
        if (!contains(x)) {
            return false;
        }
    }
    return true;
}


////////////////////////////////////////////////////////////////////////////////
//
//Commands


template<class T>
int LinkedSet<T>::insert(const T& element) {
    for (auto x = front; ; x = x->next)
    {
        if (x->value == element)
        {
            return 0; //therefore we do not want to input it. Remember this is a set
        }
        else if (x->next == nullptr)  //order does not matter
        {
            x->next = new LN(element, nullptr);
            ++used;
            ++mod_count;
            return 1;
        }
        else {
            continue;
        }

    }
}


template<class T>
int LinkedSet<T>::erase(const T& element) {
    LN  *x = front;
    for (; x->next != nullptr; x = x->next) {
        if (x->next->value == element) {
            return erase_at(x);
        }
    }
    return 0;
}


template<class T>
void LinkedSet<T>::clear() {
    delete_list(front);
}


template<class T>
template<class Iterable>
int LinkedSet<T>::insert_all(const Iterable& i) {
    int count = 0;
    for (const T& v : i)
        count += insert(v);
    return count;
}


template<class T>
template<class Iterable>
int LinkedSet<T>::erase_all(const Iterable& i) {
    int count = 0;
    for (const T& v : i)
        count += erase(v);
    return count;
}


template<class T>
template<class Iterable>
int LinkedSet<T>::retain_all(const Iterable& i) {
    LinkedSet x;
    x.insert_all(i);
    int count = 0;
    for (LN *f = front->next; f != nullptr; f = f->next)
        if (!x.contains(f->value))
        {
            erase(f->value);
            ++count;
        }
    return count;
}


////////////////////////////////////////////////////////////////////////////////
//
//Operators

template<class T>
LinkedSet<T>& LinkedSet<T>::operator = (const LinkedSet<T>& rhs) {
    if (this == &rhs)
        return *this;
    clear();
    //clears the left hand queue
    LN* f = front;
    LN *p = rhs.front;
    for (; p != nullptr; p = p->next) {
        if (f == nullptr) {
            insert(p->value);
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
bool LinkedSet<T>::operator == (const LinkedSet<T>& rhs) const {
    if(this == &rhs)
        return true;
    if(used!= rhs.size())
        return false;
    for (auto x = front; x!=nullptr ; x = x->next){
        if(!rhs.contains(x->value)){
            return false;
        }
    }
    return true;
}


template<class T>
bool LinkedSet<T>::operator != (const LinkedSet<T>& rhs) const {
    return !(*this == rhs);
}


template<class T>
bool LinkedSet<T>::operator <= (const LinkedSet<T>& rhs) const {
    if(this == &rhs)
        return true;
    if(used > rhs.size())
        return false;
    for (auto x = front; x!=nullptr ; x = x->next){
        if(!rhs.contains(x->value)){
            return false;
        }
    }
    return true;
}


template<class T>
bool LinkedSet<T>::operator < (const LinkedSet<T>& rhs) const {
    if(this == &rhs)
        return true;
    if(used >= rhs.size())
        return false;
    for (auto x = front; x!=nullptr ; x = x->next){
        if(!rhs.contains(x->value)){
            return false;
        }
    }
    return true;
}


template<class T>
bool LinkedSet<T>::operator >= (const LinkedSet<T>& rhs) const {
    return rhs <= *this;
}



template<class T>
bool LinkedSet<T>::operator > (const LinkedSet<T>& rhs) const {
    return rhs < *this;
}


template<class T>
std::ostream& operator << (std::ostream& outs, const LinkedSet<T>& s) {
    outs << "set[";
    for (auto i = s.front; i != nullptr; i = i->next) {
        outs << i->value;
    }
    outs << "]";
    return outs;
}

////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors

template<class T>
auto LinkedSet<T>::begin () const -> LinkedSet<T>::Iterator {
    return Iterator(const_cast<LinkedSet<T>*>(this),front->next);

}


template<class T>
auto LinkedSet<T>::end () const -> LinkedSet<T>::Iterator {
    return Iterator(const_cast<LinkedSet<T>*>(this), nullptr);

}


////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

template<class T>
int LinkedSet<T>::erase_at(LN* p) {
    LN *x = p->next;
    p->next = x->next;
    // revert to Pattis' notes on deletion/erasing
    delete x;
    --used;
    ++mod_count;
    return 1;
}


template<class T>
void LinkedSet<T>::delete_list(LN*& front) {
    while (front != nullptr) {
        LN *x = front;
        front = front->next;
        delete x;
        mod_count++;
    }
    used = 0;
    trailer = front = new LN();
}

////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

template<class T>
LinkedSet<T>::Iterator::Iterator(LinkedSet<T>* iterate_over, LN* initial)
        : current(initial), ref_set(iterate_over), expected_mod_count(ref_set->mod_count)
{
}


template<class T>
LinkedSet<T>::Iterator::~Iterator()
{}


template<class T>
T LinkedSet<T>::Iterator::erase() {
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("LinkedSet::Iterator::erase");
    if (!can_erase)
        throw CannotEraseError("LinkedSet::Iterator::erase Iterator cursor already erased");
    if (current == nullptr)
        throw CannotEraseError("LinkedSet::Iterator::erase Iterator cursor beyond data structure");

    can_erase = false;
    T to_return = current->value;

    //LN* node = current;
    current = current->next;
    //ref_set->erase_at(node);
    ref_set->erase(to_return); // erasing the letter at that point
    expected_mod_count = ref_set->mod_count;
    return to_return;
}


template<class T>
std::string LinkedSet<T>::Iterator::str() const {
    std::ostringstream answer;
    answer << ref_set->str() << "(current=" << current << ",expected_mod_count=" << expected_mod_count << ",can_erase=" << can_erase << ")";
    return answer.str();
}


template<class T>
auto LinkedSet<T>::Iterator::operator ++ () -> LinkedSet<T>::Iterator& {
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("LinkedSet::Iterator::operator ++");

    if (current == nullptr)
        return *this;

    if (can_erase)
        current = current->next;
    else
        can_erase = true;  //current already indexes "one beyond" erased value

    return *this;
}


template<class T>
auto LinkedSet<T>::Iterator::operator ++ (int) -> LinkedSet<T>::Iterator {
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("LinkedSet::Iterator::operator ++(int)");

    if (current == nullptr)
        return *this;

    Iterator to_return(*this);
    if (can_erase)
        current = current->next;
    else
        can_erase = true;  //current already indexes "one beyond" erased value

    return to_return;
}


template<class T>
bool LinkedSet<T>::Iterator::operator == (const LinkedSet<T>::Iterator& rhs) const {
    const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
    if (rhsASI == 0)
        throw IteratorTypeError("LinkedSet::Iterator::operator ==");
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("LinkedSet::Iterator::operator ==");
    if (ref_set != rhsASI->ref_set)
        throw ComparingDifferentIteratorsError("LinkedSet::Iterator::operator ==");

    return current == rhsASI->current;
}


template<class T>
bool LinkedSet<T>::Iterator::operator != (const LinkedSet<T>::Iterator& rhs) const {
    const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
    if (rhsASI == 0)
        throw IteratorTypeError("LinkedSet::Iterator::operator !=");
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("LinkedSet::Iterator::operator !=");
    if (ref_set != rhsASI->ref_set)
        throw ComparingDifferentIteratorsError("LinkedSet::Iterator::operator !=");

    return current != rhsASI->current;
}


template<class T>
T& LinkedSet<T>::Iterator::operator *() const {
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("LinkedSet::Iterator::operator *");
    if (!can_erase || current == nullptr) {
        std::ostringstream where;
        where << current << " when size = " << ref_set->size();
        throw IteratorPositionIllegal("LinkedSet::Iterator::operator * Iterator illegal: "+where.str());
    }

    return current->value;
}


template<class T>
T* LinkedSet<T>::Iterator::operator ->() const {
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("LinkedSet::Iterator::operator ->");
    if (!can_erase || current == nullptr) {
        std::ostringstream where;
        where << current << " when size = " << ref_set->size();
        throw IteratorPositionIllegal("LinkedSet::Iterator::operator -> Iterator illegal: "+where.str());
    }

    return &(current->value);
}


}

#endif /* LINKED_SET_HPP_ */
