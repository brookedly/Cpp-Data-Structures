// Submitter: brookedl(Ly, Brooke)
// Partner  : jiayueg(Gao, Isley)
// We certify that we worked cooperatively on this programming
//   assignment, according to the rules for pair programming
#ifndef LINKED_PRIORITY_QUEUE_HPP_
#define LINKED_PRIORITY_QUEUE_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
#include "array_stack.hpp"      //See operator <<


namespace ics {

#ifndef undefinedgtdefined
#define undefinedgtdefined
    template<class T>
    bool undefinedgt (const T& a, const T& b) {return false;}
#endif /* undefinedgtdefined */

//Instantiate the templated class supplying tgt(a,b): true, iff a has higher priority than b.
//If tgt is defaulted to undefinedgt in the template, then a constructor must supply cgt.
//If both tgt and cgt are supplied, then they must be the same (by ==) function.
//If neither is supplied, or both are supplied but different, TemplateFunctionError is raised.
//The (unique) non-undefinedgt value supplied by tgt/cgt is stored in the instance variable gt.
    template<class T, bool (*tgt)(const T& a, const T& b) = undefinedgt<T>> class LinkedPriorityQueue {
    public:
        //Destructor/Constructors
        ~LinkedPriorityQueue();

        LinkedPriorityQueue          (bool (*cgt)(const T& a, const T& b) = undefinedgt<T>);
        LinkedPriorityQueue          (const LinkedPriorityQueue<T,tgt>& to_copy, bool (*cgt)(const T& a, const T& b) = undefinedgt<T>);
        explicit LinkedPriorityQueue (const std::initializer_list<T>& il, bool (*cgt)(const T& a, const T& b) = undefinedgt<T>);

        //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
        template <class Iterable>
        explicit LinkedPriorityQueue (const Iterable& i, bool (*cgt)(const T& a, const T& b) = undefinedgt<T>);


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
        LinkedPriorityQueue<T,tgt>& operator = (const LinkedPriorityQueue<T,tgt>& rhs);
        bool operator == (const LinkedPriorityQueue<T,tgt>& rhs) const;
        bool operator != (const LinkedPriorityQueue<T,tgt>& rhs) const;

        template<class T2, bool (*gt2)(const T2& a, const T2& b)>
        friend std::ostream& operator << (std::ostream& outs, const LinkedPriorityQueue<T2,gt2>& pq);



    private:
        class LN;

    public:
        class Iterator {
        public:
            //Private constructor called in begin/end, which are friends of LinkedPriorityQueue<T,tgt>
            ~Iterator();
            T           erase();
            std::string str  () const;
            LinkedPriorityQueue<T,tgt>::Iterator& operator ++ ();
            LinkedPriorityQueue<T,tgt>::Iterator  operator ++ (int);
            bool operator == (const LinkedPriorityQueue<T,tgt>::Iterator& rhs) const;
            bool operator != (const LinkedPriorityQueue<T,tgt>::Iterator& rhs) const;
            T& operator *  () const;
            T* operator -> () const;
            friend std::ostream& operator << (std::ostream& outs, const LinkedPriorityQueue<T,tgt>::Iterator& i) {
                outs << i.str(); //Use the same meaning as the debugging .str() method
                return outs;
            }
            friend Iterator LinkedPriorityQueue<T,tgt>::begin () const;
            friend Iterator LinkedPriorityQueue<T,tgt>::end   () const;

        private:
            //If can_erase is false, current indexes the "next" value (must ++ to reach it)
            LN*             prev;            //prev should be initalized to the header
            LN*             current;         //current == prev->next
            LinkedPriorityQueue<T,tgt>* ref_pq;
            int             expected_mod_count;
            bool            can_erase = true;

            //Called in friends begin/end
            Iterator(LinkedPriorityQueue<T,tgt>* iterate_over, LN* initial);
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


        bool (*gt) (const T& a, const T& b); // The gt used by enqueue (from template or constructor)
        LN* front     =  new LN();
        int used      =  0;                  //Cache the number of values in linked list
        int mod_count =  0;                  //For sensing concurrent modification

        //Helper methods
        void delete_list(LN*& front);        //Deallocate all LNs, and set front's argument to nullptr;
    };





////////////////////////////////////////////////////////////////////////////////
//
//LinkedPriorityQueue class and related definitions

//Destructor/Constructors

    template<class T, bool (*tgt)(const T& a, const T& b)>
    LinkedPriorityQueue<T,tgt>::~LinkedPriorityQueue() {
        clear();
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    LinkedPriorityQueue<T,tgt>::LinkedPriorityQueue(bool (*cgt)(const T& a, const T& b))
//            : gt(tgt != nullptr ? tgt : cgt)
    {
//        if (gt == nullptr)
//            throw TemplateFunctionError("LinkedPriorityQueue::default constructor: neither specified");
//        if (tgt != nullptr && cgt != nullptr && tgt != cgt)
//            throw TemplateFunctionError("LinkedPriorityQueue::default constructor: both specified and different");
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    LinkedPriorityQueue<T,tgt>::LinkedPriorityQueue(const LinkedPriorityQueue<T,tgt>& to_copy, bool (*cgt)(const T& a, const T& b))
    {
//        clear();
//        *this = to_copy;
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    LinkedPriorityQueue<T,tgt>::LinkedPriorityQueue(const std::initializer_list<T>& il, bool (*cgt)(const T& a, const T& b))
//            : gt(tgt != nullptr ? tgt : cgt)
    {
//        if (gt == nullptr)
//            throw TemplateFunctionError("LinkedPriorityQueue::initializer_list constructor: neither specified");
//        if (tgt != nullptr && cgt != nullptr && tgt != cgt)
//            throw TemplateFunctionError("LinkedPriorityQueue::initializer_list constructor: both specified and different");
//        for (auto x : il)
//            enqueue(x);
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    template<class Iterable>
    LinkedPriorityQueue<T,tgt>::LinkedPriorityQueue(const Iterable& i, bool (*cgt)(const T& a, const T& b))
    {
//        if (gt == nullptr)
//            throw TemplateFunctionError("LinkedPriorityQueue::Iterable constructor: neither specified");
//        if (tgt != nullptr && cgt != nullptr && tgt != cgt)
//            throw TemplateFunctionError("LinkedPriorityQueue::Iterable constructor: both specified and different");
//        for (auto x : i)
//            enqueue(x);
    }


////////////////////////////////////////////////////////////////////////////////
//
//Queries

    template<class T, bool (*tgt)(const T& a, const T& b)>
    bool LinkedPriorityQueue<T,tgt>::empty() const {
        return used == 0;
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    int LinkedPriorityQueue<T,tgt>::size() const {
        return used;
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    T& LinkedPriorityQueue<T,tgt>::peek () const {
        if (this->empty())
            throw EmptyError("ArrayPriorityQueue::peek");
        return front->next->value;
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    std::string LinkedPriorityQueue<T,tgt>::str() const {
        std::ostringstream answer;
        answer << "PriorityLinkedQueue[";
        if (used != 0)
        {
            for (auto f = front->next; f != nullptr; f = f->next)
            {
                answer << "->" << f->value;
            }
        }
        answer << "](used=" << used << ",front=" << front << ",mod_count=" << mod_count << ")";
        return answer.str();
    }


////////////////////////////////////////////////////////////////////////////////
//
//Commands

    template<class T, bool (*tgt)(const T& a, const T& b)>
    int LinkedPriorityQueue<T,tgt>::enqueue(const T& element) {
        LN *add = front;
        if (front == nullptr)
            front->next = new LN(element, front->next);

        for (;; add = add->next) {
            if (add->next == nullptr) {
                add->next = new LN(element, add->next);
                break;

            } else if (tgt(element, add->next->value)) {
                add->next = new LN(element, add->next);
                break;
            }
            else {
                continue;
            }
        }
        mod_count++;
        used++;
        return 1;
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    T LinkedPriorityQueue<T,tgt>::dequeue() {
        if (this->empty())
            throw EmptyError("LinkedPriorityQueue::dequeue");
        LN *x = front->next;
        T answer = front->next->value;
        front->next = front->next->next;
        delete x;
        ++mod_count;
        --used;
        return answer;
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    void LinkedPriorityQueue<T,tgt>::clear() {
       delete_list(front);
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    template <class Iterable>
    int LinkedPriorityQueue<T,tgt>::enqueue_all (const Iterable& i) {
        int count = 0;
        for (const T& v : i)
            count += enqueue(v);
        return count;
    }


////////////////////////////////////////////////////////////////////////////////
//
//Operators

    template<class T, bool (*tgt)(const T& a, const T& b)>
    LinkedPriorityQueue<T,tgt>& LinkedPriorityQueue<T,tgt>::operator = (const LinkedPriorityQueue<T,tgt>& rhs) {

    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    bool LinkedPriorityQueue<T,tgt>::operator == (const LinkedPriorityQueue<T,tgt>& rhs) const {
        if (this == &rhs)
        return true;
        //int used = this->size();
        if (gt != rhs.gt) {

            return false;
        }
        if (used != rhs.size()) {
            return false;
        }
        else {
            LN *p = rhs.front;
            for (auto v = front; v != nullptr; v = v->next, p = p->next) {
                if (v->value != p->value)
                    return false;
            }
        }
    return true;
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    bool LinkedPriorityQueue<T,tgt>::operator != (const LinkedPriorityQueue<T,tgt>& rhs) const {
        return !(*this == rhs);
    }

    template<class T, bool (*tgt)(const T& a, const T& b)>
    std::ostream& operator << (std::ostream& outs, const LinkedPriorityQueue<T,tgt>& pq) {
//        ics::ArrayStack<T> letters;
//        for (auto x : pq)
//        {
//            letters.push(x);
//        }
//        outs << "priority_queue[";
//        for (ics::ArrayStack<std::string>::Iterator i = letters.begin(); i != letters.end(); ++i)
//        {
//            outs << *i << ",";
//
//        }
//        outs<<"]:highest";
//        return outs;

        }



////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors


    template<class T, bool (*tgt)(const T& a, const T& b)>
    auto LinkedPriorityQueue<T,tgt>::begin () const -> LinkedPriorityQueue<T,tgt>::Iterator {
        return Iterator(const_cast<LinkedPriorityQueue<T,tgt>*>(this),front->next);
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    auto LinkedPriorityQueue<T,tgt>::end () const -> LinkedPriorityQueue<T,tgt>::Iterator {
        return Iterator(const_cast<LinkedPriorityQueue<T,tgt>*>(this), nullptr);
    }


////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

    template<class T, bool (*tgt)(const T& a, const T& b)>
    void LinkedPriorityQueue<T,tgt>::delete_list(LN*& front) {
        while (front != nullptr)
        {
            LN* x = front;
            front = front->next;
            delete x;
            mod_count++;
//            used--;
        }
        used = 0; //used needs be updated to 0 otherwise it may
        // cause trouble with a used variable that holds random int value
        front = new LN();
//        used--;
    }




////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

    template<class T, bool (*tgt)(const T& a, const T& b)>
    LinkedPriorityQueue<T,tgt>::Iterator::Iterator(LinkedPriorityQueue<T,tgt>* iterate_over, LN* initial)
            : current(initial), ref_pq(iterate_over), expected_mod_count(ref_pq->mod_count)
    {
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    LinkedPriorityQueue<T,tgt>::Iterator::~Iterator()
    {}


    template<class T, bool (*tgt)(const T& a, const T& b)>
    T LinkedPriorityQueue<T,tgt>::Iterator::erase() {
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    std::string LinkedPriorityQueue<T,tgt>::Iterator::str() const {
        std::ostringstream answer;
        answer << ref_pq->str() << "/current=" << current << "/expected_mod_count=" << expected_mod_count << "/can_erase=" << can_erase;
        return answer.str();
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    auto LinkedPriorityQueue<T,tgt>::Iterator::operator ++ () -> LinkedPriorityQueue<T,tgt>::Iterator& {
        if (expected_mod_count != ref_pq->mod_count)
            throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ++");

        if (current == nullptr)
            return *this;

        if (can_erase) {
            prev = current;
            current = current->next;
        }
        else {
            can_erase = true;
        }
        return *this;
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    auto LinkedPriorityQueue<T,tgt>::Iterator::operator ++ (int) -> LinkedPriorityQueue<T,tgt>::Iterator {
        if (expected_mod_count != ref_pq->mod_count)
            throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ++(int)");

        if (current == nullptr)
            return *this;

        Iterator to_return(*this);
        if (can_erase) {
            prev = current;
            current = current->next;
        }
        else {
            can_erase = true;
        }
        return to_return;
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    bool LinkedPriorityQueue<T,tgt>::Iterator::operator == (const LinkedPriorityQueue<T,tgt>::Iterator& rhs) const {const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
        if (rhsASI == 0)
            throw IteratorTypeError("LinkedPriorityQueue::Iterator::operator ==");
        if (expected_mod_count != ref_pq->mod_count)
            throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ==");
        if (ref_pq != rhsASI->ref_pq)
            throw ComparingDifferentIteratorsError("LinkedPriorityQueue::Iterator::operator ==");

        return current == rhsASI->current;
    }


    template<class T, bool (*tgt)(const T& a, const T& b)>
    bool LinkedPriorityQueue<T,tgt>::Iterator::operator != (const LinkedPriorityQueue<T,tgt>::Iterator& rhs) const {
        const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
        if (rhsASI == 0)
            throw IteratorTypeError("LinkedPriorityQueue::Iterator::operator !=");
        if (expected_mod_count != ref_pq->mod_count)
            throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator !=");
        if (ref_pq != rhsASI->ref_pq)
            throw ComparingDifferentIteratorsError("LinkedPriorityQueue::Iterator::operator !=");

        return current != rhsASI->current;
    }

    template<class T, bool (*tgt)(const T& a, const T& b)>
    T& LinkedPriorityQueue<T,tgt>::Iterator::operator *() const {
        if (expected_mod_count != ref_pq->mod_count)
            throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator *");
        if (!can_erase || current == nullptr) {
            std::ostringstream where;
            where << current << " when size = " << ref_pq->size();
            throw IteratorPositionIllegal("LinkedPriorityQueue::Iterator::operator * Iterator illegal: "+where.str());
        }

        return current->value;
    }

    template<class T, bool (*tgt)(const T& a, const T& b)>
    T* LinkedPriorityQueue<T,tgt>::Iterator::operator ->() const {
        if (expected_mod_count !=  ref_pq->mod_count)
            throw ConcurrentModificationError("LinkedPriorityQueue::Iterator::operator ->");
        if (!can_erase || current == nullptr) {
            std::ostringstream where;
            where << current << " when size = " << ref_pq->size();
            throw IteratorPositionIllegal("LinkedPriorityQueue::Iterator::operator -> Iterator illegal: "+where.str());
        }

        return &(current->value);
    }


}

#endif /* LINKED_PRIORITY_QUEUE_HPP_ */
