// Submitter: brookedl(Ly, Brooke)
// Partner  : jiayueg(Gao, Isley)
// We certify that we worked cooperatively on this programming
//   assignment, according to the rules for pair programming
#ifndef HASH_SET_HPP_
#define HASH_SET_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
#include "pair.hpp"

namespace ics {


#ifndef undefinedhashdefined
#define undefinedhashdefined
template<class T>
int undefinedhash (const T& a) {return 0;}
#endif /* undefinedhashdefined */

//Instantiate the templated class supplying thash(a): produces a hash value for a.
//If thash is defaulted to undefinedhash in the template, then a constructor must supply chash.
//If both thash and chash are supplied, then they must be the same (by ==) function.
//If neither is supplied, or both are supplied but different, TemplateFunctionError is raised.
//The (unique) non-undefinedhash value supplied by thash/chash is stored in the instance variable hash.
template<class T, int (*thash)(const T& a) = undefinedhash<T>> class HashSet {
  public:
    typedef int (*hashfunc) (const T& a);

    //Destructor/Constructors
    ~HashSet ();

    HashSet (double the_load_threshold = 1.0, int (*chash)(const T& a) = undefinedhash<T>);
    explicit HashSet (int initial_bins, double the_load_threshold = 1.0, int (*chash)(const T& k) = undefinedhash<T>);
    HashSet (const HashSet<T,thash>& to_copy, double the_load_threshold = 1.0, int (*chash)(const T& a) = undefinedhash<T>);
    explicit HashSet (const std::initializer_list<T>& il, double the_load_threshold = 1.0, int (*chash)(const T& a) = undefinedhash<T>);

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    explicit HashSet (const Iterable& i, double the_load_threshold = 1.0, int (*chash)(const T& a) = undefinedhash<T>);


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
    HashSet<T,thash>& operator = (const HashSet<T,thash>& rhs);
    bool operator == (const HashSet<T,thash>& rhs) const;
    bool operator != (const HashSet<T,thash>& rhs) const;
    bool operator <= (const HashSet<T,thash>& rhs) const;
    bool operator <  (const HashSet<T,thash>& rhs) const;
    bool operator >= (const HashSet<T,thash>& rhs) const;
    bool operator >  (const HashSet<T,thash>& rhs) const;

    template<class T2, int (*hash2)(const T2& a)>
    friend std::ostream& operator << (std::ostream& outs, const HashSet<T2,hash2>& s);



  private:
    class LN;

  public:
    class Iterator {
      public:
        typedef pair<int,LN*> Cursor;

        //Private constructor called in begin/end, which are friends of HashSet<T,thash>
        ~Iterator();
        T           erase();
        std::string str  () const;
        HashSet<T,thash>::Iterator& operator ++ ();
        HashSet<T,thash>::Iterator  operator ++ (int);
        bool operator == (const HashSet<T,thash>::Iterator& rhs) const;
        bool operator != (const HashSet<T,thash>::Iterator& rhs) const;
        T& operator *  () const;
        T* operator -> () const;
        friend std::ostream& operator << (std::ostream& outs, const HashSet<T,thash>::Iterator& i) {
          outs << i.str(); //Use the same meaning as the debugging .str() method
          return outs;
        }
        friend Iterator HashSet<T,thash>::begin () const;
        friend Iterator HashSet<T,thash>::end   () const;

      private:
        //If can_erase is false, current indexes the "next" value (must ++ to reach it)
        Cursor              current; //Bin Index and Cursor; stops if LN* == nullptr
        HashSet<T,thash>*   ref_set;
        int                 expected_mod_count;
        bool                can_erase = true;

        //Helper methods
        void advance_cursors();

        //Called in friends begin/end
        Iterator(HashSet<T,thash>* iterate_over, bool from_begin);
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

public:
  int (*hash)(const T& k);   //Hashing function used (from template or constructor)
private:
  LN** set      = nullptr;   //Pointer to array of pointers: each bin stores a list with a trailer node
  double load_threshold;     //used/bins <= load_threshold
  int bins      = 1;         //# bins in array (should start >= 1 so hash_compress doesn't % 0)
  int used      = 0;         //Cache for number of key->value pairs in the hash table
  int mod_count = 0;         //For sensing concurrent modification


  //Helper methods
  int   hash_compress        (const T& key)              const;  //hash function ranged to [0,bins-1]
  LN*   find_element         (const T& element)          const;  //Returns reference to element's node or nullptr
  LN*   copy_list            (LN*   l)                   const;  //Copy the elements in a bin (order irrelevant)
  LN**  copy_hash_table      (LN** ht, int bins)         const;  //Copy the bins/keys/values in ht tree (order in bins irrelevant)

  void  ensure_load_threshold(int new_used);                     //Reallocate if load_threshold > load_threshold
  void  delete_hash_table    (LN**& ht, int bins);               //Deallocate all LN in ht (and the ht itself; ht == nullptr)
};





//HashSet class and related definitions

////////////////////////////////////////////////////////////////////////////////
//
//Destructor/Constructors

template<class T, int (*thash)(const T& a)>
HashSet<T,thash>::~HashSet() {
    delete_hash_table(set,bins);
}


template<class T, int (*thash)(const T& a)>
HashSet<T,thash>::HashSet(double the_load_threshold, int (*chash)(const T& element))
        : hash( thash != (hashfunc)undefinedhash<T> ? thash : chash), load_threshold(the_load_threshold)
{
    if (hash == (hashfunc)undefinedhash<T>)
        throw TemplateFunctionError("HashSet::default constructor nothing specified");
    if (thash != (hashfunc)undefinedhash<T> && chash != (hashfunc)undefinedhash<T> &&  thash != chash)
        throw TemplateFunctionError("HashSet::default constructor both specified and different");
    set = new LN*[bins];
    for (int x=0; x<bins; ++x)
        set[x] = new LN();
}


template<class T, int (*thash)(const T& a)>
HashSet<T,thash>::HashSet(int initial_bins, double the_load_threshold, int (*chash)(const T& element))
        : hash( thash != (hashfunc)undefinedhash<T> ? thash : chash), load_threshold(the_load_threshold)
{

    if (hash == (hashfunc)undefinedhash<T>)
        throw TemplateFunctionError("HashSet::default constructor nothing specified");
    if (thash != nullptr && chash != nullptr &&  thash != chash)
        throw TemplateFunctionError("HashSet::default constructor both specified and different");
    set = new LN*[initial_bins];
    for (int i = 0 ; i < initial_bins; i++)
        set[i] = new LN();
}


template<class T, int (*thash)(const T& a)>
HashSet<T,thash>::HashSet(const HashSet<T,thash>& to_copy, double the_load_threshold, int (*chash)(const T& element))
        : hash(to_copy.hash), load_threshold(to_copy.load_threshold), bins(to_copy.bins), used(to_copy.used)
{
    if (hash == (hashfunc)undefinedhash<T>)
        throw TemplateFunctionError("HashSet::copy constructor: neither specified");
    if (thash != (hashfunc)undefinedhash<T> && chash != (hashfunc)undefinedhash<T> && thash != chash)
        throw TemplateFunctionError("HashSet::copy constructor: both specified and different");
    set = copy_hash_table(to_copy.set, bins);
}


template<class T, int (*thash)(const T& a)>
HashSet<T,thash>::HashSet(const std::initializer_list<T>& il, double the_load_threshold, int (*chash)(const T& element))
        : hash( thash != (hashfunc)undefinedhash<T> ? thash : chash), load_threshold(the_load_threshold)
{
    if (hash == (hashfunc)undefinedhash<T>)
        throw TemplateFunctionError("HashSet::initializer_list constructor: neither specified");
    if (thash != (hashfunc)undefinedhash<T> && chash != (hashfunc)undefinedhash<T> && thash != chash)
        throw TemplateFunctionError("HashSet::initializer_list constructor: both specified and different");
    set = new LN*[bins];
    for (int i =0 ; i< bins; i++)
        set[i] = new LN();
    for (auto x : il)
        insert(x);
}


template<class T, int (*thash)(const T& a)>
template<class Iterable>
HashSet<T,thash>::HashSet(const Iterable& i, double the_load_threshold, int (*chash)(const T& a))
        : hash( thash != (hashfunc)undefinedhash<T> ? thash : chash), load_threshold(the_load_threshold)
{
    if (hash == nullptr)
        throw TemplateFunctionError("HashSet::Iterable constructor: neither specified");
    if (thash != (hashfunc)undefinedhash<T> && chash != (hashfunc)undefinedhash<T> && thash != chash)
        throw TemplateFunctionError("HashSet::Iterable constructor: both specified and different");
    set = new LN*[bins];
    for (int i =0 ; i< bins; i++)
        set[i] = new LN();
    for (auto x : i)
        insert(x);
}


////////////////////////////////////////////////////////////////////////////////
//
//Queries

template<class T, int (*thash)(const T& a)>
bool HashSet<T,thash>::empty() const {
    return used == 0;
}


template<class T, int (*thash)(const T& a)>
int HashSet<T,thash>::size() const {
    return used;
}


template<class T, int (*thash)(const T& a)>
bool HashSet<T,thash>::contains (const T& element) const {
    for (int x = 0; x < bins; x++ ){
        for (LN* front = set[x]; front->next != nullptr; front = front->next){
            if (element == front->value)
                return true;
        }
    }
    return false;

}


template<class T, int (*thash)(const T& a)>
std::string HashSet<T,thash>::str() const {
    std::ostringstream answer;
    answer << "HashSet[";


    answer << "](used=" << used << ",mod_count=" << mod_count << ")";
    return answer.str();
}


template<class T, int (*thash)(const T& a)>
template <class Iterable>
bool HashSet<T,thash>::contains_all(const Iterable& i) const {
    for (auto v : i)
        if (!contains(v))
            return false;

    return true;
}


////////////////////////////////////////////////////////////////////////////////
//
//Commands

template<class T, int (*thash)(const T& a)>
int HashSet<T,thash>::insert(const T& element) {
    int binnum = hash_compress(element);
    for (LN *front = set[binnum]; front->next != nullptr; front = front->next){
        if (front->value == element)
        {
            return 0;
            //element already in set
        }
    }
    ensure_load_threshold(++used);
    ++mod_count;
    set[binnum] = new LN(element, set[binnum]);
    return 1;
}


template<class T, int (*thash)(const T& a)>
int HashSet<T,thash>::erase(const T& element) {
    int binnum = hash_compress(element);
    for (LN *front = set[binnum]; front->next != nullptr; front = front->next){
        if (front->value == element)
        {
            --used;
            ++mod_count;
            LN* to_delete = front->next;
            front->value = to_delete->value;
            front->next = to_delete->next;
            delete to_delete;
            return 1;
        }
    }
    return 0;
}


template<class T, int (*thash)(const T& a)>
void HashSet<T,thash>::clear() {
    for (int x = 0; x <bins; x++)
    {
        for (LN* front = set[x]; front->next != nullptr; front = set[x])
        {
            --used;
            set[x] = set[x]->next;
            delete front;
        }
    }
}


template<class T, int (*thash)(const T& a)>
template<class Iterable>
int HashSet<T,thash>::insert_all(const Iterable& i) {
    int count=0;
    for(auto x : i)
        count += insert(x);
    return count;
}


template<class T, int (*thash)(const T& a)>
template<class Iterable>
int HashSet<T,thash>::erase_all(const Iterable& i) {
    int count=0;
    for(auto x : i)
        count += erase(x);
    return count;
}


template<class T, int (*thash)(const T& a)>
template<class Iterable>
int HashSet<T,thash>::retain_all(const Iterable& i) {
    HashSet s(i);
    int count = 0;
    for (int i=0; i<bins; i++) {
        LN *front = set[i];
        while (front->next != nullptr) {
            if (!s.contains(front->value)) {
                erase(front->value);
            } else {
                front = front->next;
            }
        }
    }
    return count;
}


////////////////////////////////////////////////////////////////////////////////
//
//Operators

template<class T, int (*thash)(const T& a)>
HashSet<T,thash>& HashSet<T,thash>::operator = (const HashSet<T,thash>& rhs) {
    if (this == &rhs)
        return *this;

    clear();
    set = copy_hash_table(rhs.set,rhs.bins);
    used = rhs.used;
    ++mod_count;
    return *this;
}


template<class T, int (*thash)(const T& a)>
bool HashSet<T,thash>::operator == (const HashSet<T,thash>& rhs) const {
    if (this == &rhs)
        return true;
    if (used != rhs.size())
        return false;
    for (int x = 0; x < bins; x++)
    {
        for (LN* front = set[x]; front->next != nullptr; front = front->next)
        {
            if (!rhs.contains(front->value))
                return false;
        }
    }
    return true;
}



template<class T, int (*thash)(const T& a)>
bool HashSet<T,thash>::operator != (const HashSet<T,thash>& rhs) const {
    return !(*this == rhs);
}


template<class T, int (*thash)(const T& a)>
bool HashSet<T,thash>::operator <= (const HashSet<T,thash>& rhs) const {
    if (this == &rhs)
        return true;
    if (used > rhs.size())
        return false;
    for (int x = 0; x < bins; x++)
    {
        for (LN* front = set[x]; front->next != nullptr; front = front->next)
        {
            if (!rhs.contains(front->value))
                return false;
        }
    }
    return true;
}

template<class T, int (*thash)(const T& a)>
bool HashSet<T,thash>::operator < (const HashSet<T,thash>& rhs) const {
    if (this == &rhs)
        return true;
    if (used >= rhs.size())
        return false;
    for (int x = 0; x < bins; x++)
    {
        for (LN* front = set[x]; front->next != nullptr; front = front->next)
        {
            if (!rhs.contains(front->value))
                return false;
        }
    }
    return true;
}


template<class T, int (*thash)(const T& a)>
bool HashSet<T,thash>::operator >= (const HashSet<T,thash>& rhs) const {
    return rhs <= *this;
}


template<class T, int (*thash)(const T& a)>
bool HashSet<T,thash>::operator > (const HashSet<T,thash>& rhs) const {
    return rhs < *this;
}


template<class T, int (*thash)(const T& a)>
std::ostream& operator << (std::ostream& outs, const HashSet<T,thash>& s) {
    if (s.empty())
        outs << "set[]";
    else {
        outs << "set[";
        for (int x = 0; x < s.bins; x++){
            auto f = s.set[x];
            outs << f->value ;
            for (auto front = f->next; front->next != nullptr; front= front->next){
                outs << "," << front->value;
            }
            outs << "]";
        }
    }
    return outs;
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors

template<class T, int (*thash)(const T& a)>
auto HashSet<T,thash>::begin () const -> HashSet<T,thash>::Iterator {
    return Iterator(const_cast<HashSet<T,thash>*>(this),true);
}


template<class T, int (*thash)(const T& a)>
auto HashSet<T,thash>::end () const -> HashSet<T,thash>::Iterator {
    return Iterator(const_cast<HashSet<T,thash>*>(this),false);
}


////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

template<class T, int (*thash)(const T& a)>
int HashSet<T,thash>::hash_compress (const T& element) const {
    return std::abs(hash(element)) % bins;
}


template<class T, int (*thash)(const T& a)>
typename HashSet<T,thash>::LN* HashSet<T,thash>::find_element (const T& element) const {

    for (int x = 0; x < bins; x++)
        for (LN* front = set[x]; front->next!=nullptr; front = front->next)
            if (element == front->value)
                return front;

    return nullptr;
}

template<class T, int (*thash)(const T& a)>
typename HashSet<T,thash>::LN* HashSet<T,thash>::copy_list (LN* l) const {
    if (l == nullptr)
        return nullptr;
    else
        return new LN(l->value, copy_list(l->next));
}


template<class T, int (*thash)(const T& a)>
typename HashSet<T,thash>::LN** HashSet<T,thash>::copy_hash_table (LN** ht, int bins) const {
    LN** copied = new LN*[bins];
    for(int x = 0; x < bins; x++)
    {
        copied[x] = copy_list(ht[x]);
    }
    return copied;
}


template<class T, int (*thash)(const T& a)>
void HashSet<T,thash>::ensure_load_threshold(int new_used) {
    double load_factor = new_used/bins;
    int newbins = bins*2;
    if(load_factor <= load_threshold){
        return;
    }
    if (load_factor > load_threshold){
        //Create a new hash table with twice the number of bins
        //and puts all the values in the original hash table into the new one
        LN** setcopy = set;
        set = new LN*[newbins];
        for (int i =0 ; i< newbins; i++){
            set[i] = new LN();
        }
        //for each key in setcopy
        for (int x = 0; x < bins; x++) {
            for (LN *c = setcopy[x]; c->next != nullptr; c = c->next) {

                int binnum = hash_compress(c->value);
                ++mod_count;
                set[binnum] = new LN(c->value, set[binnum]);
            }
        }
    }
}


template<class T, int (*thash)(const T& a)>
void HashSet<T,thash>::delete_hash_table (LN**& ht, int bins) {

}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

template<class T, int (*thash)(const T& a)>
void HashSet<T,thash>::Iterator::advance_cursors() {
//    Advance cursor = cursor.next; if it becomes nullptr, loop to advance the
//    index to later bins, stopping at the first non-nullptr bin
//    Succeed: set index to bin number, cursor to the first LN in the bin
//    Fail   : set cursor to nullptr
    if (current.second != nullptr && current.second->next != nullptr && current.second->next->next != nullptr)
    {
        //if this condition fails that means it is pointing to a Trailer Node
        current.second = current.second->next;
        return;
    }
    else {
        for (++current.first; current.first < ref_set->bins; ++current.first)
            if (ref_set->set[current.first]->next != nullptr) {
                current = ics::pair<int, LN*>(current.first, ref_set->set[current.first]);
                return;
            }
    }
    current = ics::pair<int, LN*>(-1, nullptr);
}


template<class T, int (*thash)(const T& a)>
HashSet<T,thash>::Iterator::Iterator(HashSet<T,thash>* iterate_over, bool begin)
:ref_set(iterate_over), expected_mod_count(ref_set->mod_count)
{
    current = ics::pair<int,LN*>(-1,nullptr);
    if (begin)
        advance_cursors();
}


template<class T, int (*thash)(const T& a)>
HashSet<T,thash>::Iterator::~Iterator()
{}


template<class T, int (*thash)(const T& a)>
T HashSet<T,thash>::Iterator::erase() {
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("HashSet::Iterator::erase");
    if (!can_erase)
        throw CannotEraseError("HashSet::Iterator::erase Iterator cursor already erased");
    if (current.second == nullptr)
        throw CannotEraseError("HashSet::Iterator::erase Iterator cursor beyond data structure");

    can_erase = false;
    T to_return = current.second->value;
    ref_set->erase(to_return);
    expected_mod_count = ref_set->mod_count;
    return to_return;
}


template<class T, int (*thash)(const T& a)>
std::string HashSet<T,thash>::Iterator::str() const {
    std::ostringstream answer;
    answer << ref_set->str() << "(expected_mod_count=" << expected_mod_count << ",can_erase=" << can_erase << ")";
    return answer.str();
}


template<class T, int (*thash)(const T& a)>
auto  HashSet<T,thash>::Iterator::operator ++ () -> HashSet<T,thash>::Iterator& {
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("HashSet::Iterator::operator ++");

    if (current.second != nullptr)
        if (can_erase|| current.second->next == nullptr)
            advance_cursors();
    else {
        can_erase = true;
    }
    return *this;
}


template<class T, int (*thash)(const T& a)>
auto  HashSet<T,thash>::Iterator::operator ++ (int) -> HashSet<T,thash>::Iterator {
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("HashSet::Iterator::operator ++(int)");

    Iterator to_return(*this);
    if (current.second != nullptr )
        advance_cursors();
    else {
        can_erase = true;
    }
    return to_return;
}


template<class T, int (*thash)(const T& a)>
bool HashSet<T,thash>::Iterator::operator == (const HashSet<T,thash>::Iterator& rhs) const {
    const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
    if (rhsASI == 0)
        throw IteratorTypeError("HashSet::Iterator::operator ==");
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("HashSet::Iterator::operator ==");
    if (ref_set != rhsASI->ref_set)
        throw ComparingDifferentIteratorsError("HashSet::Iterator::operator ==");

    return current.second == rhsASI->current.second;
}


template<class T, int (*thash)(const T& a)>
bool HashSet<T,thash>::Iterator::operator != (const HashSet<T,thash>::Iterator& rhs) const {
    const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
    if (rhsASI == 0)
        throw IteratorTypeError("HashSet::Iterator::operator !=");
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("HashSet::Iterator::operator !=");
    if (ref_set != rhsASI->ref_set)
        throw ComparingDifferentIteratorsError("HashSet::Iterator::operator !=");

    return current.second != rhsASI->current.second;
}

template<class T, int (*thash)(const T& a)>
T& HashSet<T,thash>::Iterator::operator *() const {
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("HashSet::Iterator::operator *");
    if (!can_erase || current.second == nullptr)
        throw IteratorPositionIllegal("HashSet::Iterator::operator * Iterator illegal: exhausted");

    return current.second->value;
}

template<class T, int (*thash)(const T& a)>
T* HashSet<T,thash>::Iterator::operator ->() const {
    if (expected_mod_count != ref_set->mod_count)
        throw ConcurrentModificationError("HashSet::Iterator::operator *");
    if (!can_erase || current.second == nullptr)
        throw IteratorPositionIllegal("HashSet::Iterator::operator * Iterator illegal: exhausted");

    return &(current.second->value);
}

}

#endif /* HASH_SET_HPP_ */
