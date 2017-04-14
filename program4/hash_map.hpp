// Submitter: brookedl(Ly, Brooke)
// Partner  : jiayueg(Gao, Isley)
// We certify that we worked cooperatively on this programming
//   assignment, according to the rules for pair programming
#ifndef HASH_MAP_HPP_
#define HASH_MAP_HPP_

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
template<class KEY,class T, int (*thash)(const KEY& a) = undefinedhash<KEY>> class HashMap {
  public:
    typedef ics::pair<KEY,T>   Entry;
    typedef int (*hashfunc) (const KEY& a);

    //Destructor/Constructors
    ~HashMap ();

    HashMap          (double the_load_threshold = 1.0, int (*chash)(const KEY& a) = undefinedhash<KEY>);
    explicit HashMap (int initial_bins, double the_load_threshold = 1.0, int (*chash)(const KEY& k) = undefinedhash<KEY>);
    HashMap          (const HashMap<KEY,T,thash>& to_copy, double the_load_threshold = 1.0, int (*chash)(const KEY& a) = undefinedhash<KEY>);
    explicit HashMap (const std::initializer_list<Entry>& il, double the_load_threshold = 1.0, int (*chash)(const KEY& a) = undefinedhash<KEY>);

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    explicit HashMap (const Iterable& i, double the_load_threshold = 1.0, int (*chash)(const KEY& a) = undefinedhash<KEY>);


    //Queries
    bool empty      () const;
    int  size       () const;
    bool has_key    (const KEY& key) const;
    bool has_value  (const T& value) const;
    std::string str () const; //supplies useful debugging information; contrast to operator <<


    //Commands
    T    put   (const KEY& key, const T& value);
    T    erase (const KEY& key);
    void clear ();

    //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
    template <class Iterable>
    int put_all(const Iterable& i);


    //Operators

    T&       operator [] (const KEY&);
    const T& operator [] (const KEY&) const;
    HashMap<KEY,T,thash>& operator = (const HashMap<KEY,T,thash>& rhs);
    bool operator == (const HashMap<KEY,T,thash>& rhs) const;
    bool operator != (const HashMap<KEY,T,thash>& rhs) const;

    template<class KEY2,class T2, int (*hash2)(const KEY2& a)>
    friend std::ostream& operator << (std::ostream& outs, const HashMap<KEY2,T2,hash2>& m);



  private:
    class LN;

  public:
    class Iterator {
      public:
         typedef pair<int,LN*> Cursor;

        //Private constructor called in begin/end, which are friends of HashMap<T>
        ~Iterator();
        Entry       erase();
        std::string str  () const;
        HashMap<KEY,T,thash>::Iterator& operator ++ ();
        HashMap<KEY,T,thash>::Iterator  operator ++ (int);
        bool operator == (const HashMap<KEY,T,thash>::Iterator& rhs) const;
        bool operator != (const HashMap<KEY,T,thash>::Iterator& rhs) const;
        Entry& operator *  () const;
        Entry* operator -> () const;
        friend std::ostream& operator << (std::ostream& outs, const HashMap<KEY,T,thash>::Iterator& i) {
          outs << i.str(); //Use the same meaning as the debugging .str() method
          return outs;
        }
        friend Iterator HashMap<KEY,T,thash>::begin () const;
        friend Iterator HashMap<KEY,T,thash>::end   () const;

      private:
        //If can_erase is false, current indexes the "next" value (must ++ to reach it)
        Cursor                current; //Bin Index and Cursor; stops if LN* == nullptr
        HashMap<KEY,T,thash>* ref_map;
        int                   expected_mod_count;
        bool                  can_erase = true;

        //Helper methods
        void advance_cursors();

        //Called in friends begin/end
        Iterator(HashMap<KEY,T,thash>* iterate_over, bool from_begin);
    };


    Iterator begin () const;
    Iterator end   () const;


  private:
    class LN {
    public:
      LN ()                         : next(nullptr){}
      LN (const LN& ln)             : value(ln.value), next(ln.next){}
      LN (Entry v, LN* n = nullptr) : value(v), next(n){}

      Entry value;
      LN*   next;
  };

  int (*hash)(const KEY& k);  //Hashing function used (from template or constructor)
  LN** map      = nullptr;    //Pointer to array of pointers: each bin stores a list with a trailer node
  double load_threshold;      //used/bins <= load_threshold
  int bins      = 1;          //# bins in array (should start >= 1 so hash_compress doesn't % 0)
  int used      = 0;          //Cache for number of key->value pairs in the hash table
  int mod_count = 0;          //For sensing concurrent modification


  //Helper methods
  int   hash_compress        (const KEY& key)          const;  //hash function ranged to [0,bins-1]
  LN*   find_key             (const KEY& key) const;           //Returns reference to key's node or nullptr
  LN*   copy_list            (LN*   l)                 const;  //Copy the keys/values in a bin (order irrelevant)
  LN**  copy_hash_table      (LN** ht, int bins)       const;  //Copy the bins/keys/values in ht tree (order in bins irrelevant)

  void  ensure_load_threshold(int new_used);                   //Reallocate if load_factor > load_threshold
  void  delete_hash_table    (LN**& ht, int bins);             //Deallocate all LN in ht (and the ht itself; ht == nullptr)
};



////////////////////////////////////////////////////////////////////////////////
//
//HashMap class and related definitions

//Destructor/Constructors

template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::~HashMap() {
    clear();
}


template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::HashMap(double the_load_threshold, int (*chash)(const KEY& k))
        : hash( thash != (hashfunc)undefinedhash<KEY> ? thash : chash), load_threshold(the_load_threshold)
{
    if (hash == (hashfunc)undefinedhash<KEY>)
        throw TemplateFunctionError("HashMap::default constructor: neither specified");
    if (thash != (hashfunc)undefinedhash<KEY> && chash != (hashfunc)undefinedhash<KEY> && thash != chash)
        throw TemplateFunctionError("HashMap::default constructor: both specified and different");
    map = new LN*[bins];
    for (int i =0 ; i< bins; i++)
        map[i] = new LN();
}


template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::HashMap(int initial_bins, double the_load_threshold, int (*chash)(const KEY& k))
        : hash( thash != (hashfunc)undefinedhash<KEY> ? thash : chash), load_threshold(the_load_threshold)
{
    if (hash == (hashfunc)undefinedhash<KEY>)
        throw TemplateFunctionError("HashMap::default constructor: neither specified");
    if (thash != (hashfunc)undefinedhash<KEY> && chash != (hashfunc)undefinedhash<KEY> && thash != chash)
        throw TemplateFunctionError("HashMap::default constructor: both specified and different");
    map = new LN*[initial_bins];
    for (int x = 0; x < initial_bins; x++){
        map[x] = new LN();
    }

}


template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::HashMap(const HashMap<KEY,T,thash>& to_copy, double the_load_threshold, int (*chash)(const KEY& a))
        : hash(to_copy.hash), load_threshold(to_copy.load_threshold), bins(to_copy.bins), used(to_copy.used)
{
    if (hash == (hashfunc)undefinedhash<KEY>)
        throw TemplateFunctionError("HashMap::copy constructor: neither specified");
    if (thash != (hashfunc)undefinedhash<KEY> && chash != (hashfunc)undefinedhash<KEY> && thash != chash)
        throw TemplateFunctionError("HashMap::copy constructor: both specified and different");
    map = copy_hash_table(to_copy.map, bins);
}


template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::HashMap(const std::initializer_list<Entry>& il, double the_load_threshold, int (*chash)(const KEY& k))
        : hash( thash != (hashfunc)undefinedhash<KEY> ? thash : chash), load_threshold(the_load_threshold)

{
    if (hash == (hashfunc)undefinedhash<KEY> )
        throw TemplateFunctionError("HashMap::initializer_list constructor: neither specified");
    if (thash != (hashfunc)undefinedhash<KEY> && chash != (hashfunc)undefinedhash<KEY> && thash != chash)
        throw TemplateFunctionError("HashMap::initializer_list constructor: both specified and different");
    map = new LN*[bins];
    for (int i =0 ; i< bins; i++)
        map[i] = new LN();
    for (auto x : il)
        put(x.first, x.second);
}


template<class KEY,class T, int (*thash)(const KEY& a)>
template <class Iterable>
HashMap<KEY,T,thash>::HashMap(const Iterable& i, double the_load_threshold, int (*chash)(const KEY& k))
        : hash( thash != (hashfunc)undefinedhash<KEY> ? thash : chash), load_threshold(the_load_threshold)
{
    if (hash == (hashfunc)undefinedhash<KEY> )
        throw TemplateFunctionError("HashMap::Iterable constructor: neither specified");
    if (thash != (hashfunc)undefinedhash<KEY> && chash != (hashfunc)undefinedhash<KEY> && thash != chash)
        throw TemplateFunctionError("HashMap::Iterable constructor: both specified and different");
    map = new LN*[bins];
    for (int i =0 ; i< bins; i++)
        map[i] = new LN();
    for (auto x : i)
        put(x.first, x.second);
}


////////////////////////////////////////////////////////////////////////////////
//
//Queries

template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::empty() const {
    return used == 0;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
int HashMap<KEY,T,thash>::size() const {

    return used;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::has_key (const KEY& key) const {
    return find_key(key) != nullptr;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::has_value (const T& value) const {
    if (map == nullptr)
        return false;
    for (int x = 0; x < bins; x++) {
        for (LN *c = map[x]; c->next != nullptr; c = c->next) {
            if (c->value.second == value) {
                return true;
            }
        }
    }
    return false;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
std::string HashMap<KEY,T,thash>::str() const {
    std::ostringstream answer;
    answer << "HashMap[";
//    if (used != 0)
    answer  << "](used=" << used << ",mod_count=" << mod_count << ")";
    return answer.str();
}


////////////////////////////////////////////////////////////////////////////////
//
//Commands

template<class KEY,class T, int (*thash)(const KEY& a)>
T HashMap<KEY,T,thash>::put(const KEY& key, const T& value) {
    int binnum = hash_compress(key);
    for (LN *front = map[binnum]; front->next != nullptr; front = front->next){
        if (front->value.first == key)
        {
            T to_return = front->value.second;
            front->value.second = value;
            return to_return;
        }
    }
    ensure_load_threshold(++used);
    ++mod_count;
    map[binnum] = new LN(Entry(key, value), map[binnum]);
    return map[binnum]->value.second;

}

template<class KEY,class T, int (*thash)(const KEY& a)>
T HashMap<KEY,T,thash>::erase(const KEY& key) {
    int binnum = hash_compress(key);
    if(!(has_key(key))){
        std::ostringstream answer;
        answer << "HashMap::erase: key(" << key << ") not in Map";
        throw KeyError(answer.str());
    }
    else {
        for (LN *front = map[binnum]; front->next != nullptr; front = front->next) {
            if (front->value.first == key) {
                --used;
                ++mod_count;
                T to_return = front->value.second;
                LN *to_delete = front->next;
                front->value = to_delete->value;
                front->next = to_delete->next;
                delete to_delete;
                return to_return;
            }
        }
    }
}


template<class KEY,class T, int (*thash)(const KEY& a)>
void HashMap<KEY,T,thash>::clear() {
    for (int x = 0; x <bins; x++)
    {
        for (LN* front = map[x]; front->next != nullptr; front = map[x])
        {
            --used;
            map[x] = map[x]->next;
            delete front;
        }
    }

}


template<class KEY,class T, int (*thash)(const KEY& a)>
template<class Iterable>
int HashMap<KEY,T,thash>::put_all(const Iterable& i) {

    int count = 0;
    for (const Entry& m_entry : i) {
        ++count;
        put(m_entry.first, m_entry.second);
    }
    return count;
}


////////////////////////////////////////////////////////////////////////////////
//
//Operators

template<class KEY,class T, int (*thash)(const KEY& a)>
T& HashMap<KEY,T,thash>::operator [] (const KEY& key) {
    int binnum = hash_compress(key);
    for (LN* front = map[binnum]; front->next != nullptr; front = front->next){
        if (front->value.first == key){
            return front->value.second;
        }
    }
    ensure_load_threshold(++used);
    ++mod_count;
    map[binnum] = new LN(Entry(key, T()), map[binnum]);
    return map[binnum]->value.second;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
const T& HashMap<KEY,T,thash>::operator [] (const KEY& key) const {
    LN* c = find_key(key);
    if (c != nullptr)
        return c->value.second;
    std::ostringstream answer;
    answer << "HashMap::operator []: key(" << key << ") not in Map";
    throw KeyError(answer.str());
}


template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>& HashMap<KEY,T,thash>::operator = (const HashMap<KEY,T,thash>& rhs) {
    if (this == &rhs)
        return *this;

    clear();
    map = copy_hash_table(rhs.map,rhs.bins);
    used = rhs.used;
    ++mod_count;
    return *this;
}

template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::operator == (const HashMap<KEY,T,thash>& rhs) const {
    if (this == &rhs)
        return true;
    if (used != rhs.size())
        return false;
    for (int x = 0; x < bins; x++)
    {
        for (LN* front = map[x]; front->next != nullptr; front = front->next)
        {
            if (front->value.second != rhs[front->value.first])
                return false;
        }
    }
    return true;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::operator != (const HashMap<KEY,T,thash>& rhs) const {
    return !(*this == rhs);
}


template<class KEY,class T, int (*thash)(const KEY& a)>
std::ostream& operator << (std::ostream& outs, const HashMap<KEY,T,thash>& m) {
    if (m.empty())
        outs << "map[]";
    else {
        outs << "map[";
        for (int x = 0; x < m.bins; x++){
            auto f = m.map[x];
            outs << f->value.first << "->" << f->value.second ;
            for (auto front = f->next; front->next != nullptr; front= front->next){
                outs << "," << front->value.first << "->" << front->value.second ;
            }
            outs << "]";
        }
    }
    return outs;
}


////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors

template<class KEY,class T, int (*thash)(const KEY& a)>
auto HashMap<KEY,T,thash>::begin () const -> HashMap<KEY,T,thash>::Iterator {
    return Iterator(const_cast<HashMap<KEY,T,thash>*>(this), true);
}


template<class KEY,class T, int (*thash)(const KEY& a)>
auto HashMap<KEY,T,thash>::end () const -> HashMap<KEY,T,thash>::Iterator {
    return Iterator(const_cast<HashMap<KEY,T,thash>*>(this),false);
}


////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

template<class KEY,class T, int (*thash)(const KEY& a)>
int HashMap<KEY,T,thash>::hash_compress (const KEY& key) const {
    return std::abs(hash(key)) % bins;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
typename HashMap<KEY,T,thash>::LN* HashMap<KEY,T,thash>::find_key (const KEY& key) const {
    if (map == nullptr)
        return nullptr;

    for (int x = 0; x < bins; x++) {
        for (LN *c = map[x]; c->next != nullptr; c = c->next) {
            if (c->value.first == key) {
                return c;
            }
        }
    }
    return nullptr;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
typename HashMap<KEY,T,thash>::LN* HashMap<KEY,T,thash>::copy_list (LN* l) const {
    if (l == nullptr)
        return nullptr;
    else
        return new LN(l->value, copy_list(l->next));

}


template<class KEY,class T, int (*thash)(const KEY& a)>
typename HashMap<KEY,T,thash>::LN** HashMap<KEY,T,thash>::copy_hash_table (LN** ht, int bins) const {
    LN** copied = new LN*[bins];
    for(int x = 0; x < bins; x++)
    {
        copied[x] = copy_list(ht[x]);
    }
    return copied;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
void HashMap<KEY,T,thash>::ensure_load_threshold(int new_used) {
    //Reallocate if load_factor > load_threshold
    //Load factor = ratio of values it contains divided by the number of bins in the hash table.
    double load_factor = new_used/bins;
    int newbins = bins*2;
    if(load_factor <= load_threshold){
        return;
    }
    if (load_factor > load_threshold){
        //Create a new hash table with twice the number of bins
        //and puts all the values in the original hash table into the new one
        LN** mapcopy = map;
        map = new LN*[newbins];
        for (int i =0 ; i< newbins; i++){
            map[i] = new LN();
        }
        //for each key in mapcopy
        for (int x = 0; x < bins; x++) {
            for (LN *c = mapcopy[x]; c->next != nullptr; c = c->next) {

                int binnum = hash_compress(c->value.first);
                ++mod_count;
                map[binnum] = new LN(Entry(c->value.first, c->value.second), map[binnum]);
            }
        }
    }
}


template<class KEY,class T, int (*thash)(const KEY& a)>
void HashMap<KEY,T,thash>::delete_hash_table (LN**& ht, int bins) {

}

////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

template<class KEY,class T, int (*thash)(const KEY& a)>
void HashMap<KEY,T,thash>::Iterator::advance_cursors(){
//    Advance cursor = cursor.next; if it becomes nullptr, loop to advance the
//    index to later bins, stopping at the first non-nullptr bin
//    Succeed: set index to bin number, cursor to the first LN in the bin
//    Fail   : set cursor to nullptr
    if (current.second != nullptr && current.second->next != nullptr && current.second->next->next != nullptr)
    {
        current.second = current.second->next;
        return;
    }
    else {
        for (++current.first; current.first < ref_map->bins; ++current.first)
            if (ref_map->map[current.first]->next != nullptr) {
                current = ics::pair<int, LN*>(current.first, ref_map->map[current.first]);
                return;
            }
    }
    current = ics::pair<int, LN*>(-1, nullptr);

}


template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::Iterator::Iterator(HashMap<KEY,T,thash>* iterate_over, bool from_begin)
: ref_map(iterate_over), expected_mod_count(ref_map->mod_count) {
    current = ics::pair<int,LN*>(-1,nullptr);
    if (from_begin)
        advance_cursors();
}


template<class KEY,class T, int (*thash)(const KEY& a)>
HashMap<KEY,T,thash>::Iterator::~Iterator()
{}


template<class KEY,class T, int (*thash)(const KEY& a)>
auto HashMap<KEY,T,thash>::Iterator::erase() -> Entry {
    if (expected_mod_count != ref_map->mod_count)
        throw ConcurrentModificationError("HashMap::Iterator::erase");
    if (!can_erase)
        throw CannotEraseError("HashMap::Iterator::erase Iterator cursor already erased");
    if (current.second == nullptr)
        throw CannotEraseError("HashMap::Iterator::erase Iterator cursor beyond data structure");

    can_erase = false;
    Entry to_return = current.second->value;
    ref_map->erase(current.second->value.first);
    expected_mod_count = ref_map->mod_count;
    return to_return;

}


template<class KEY,class T, int (*thash)(const KEY& a)>
std::string HashMap<KEY,T,thash>::Iterator::str() const {
    std::ostringstream answer;
    answer << ref_map->str() << "(expected_mod_count=" << expected_mod_count << ",can_erase=" << can_erase << ")";
    return answer.str();
}

template<class KEY,class T, int (*thash)(const KEY& a)>
auto  HashMap<KEY,T,thash>::Iterator::operator ++ () -> HashMap<KEY,T,thash>::Iterator& {

    if (expected_mod_count != ref_map->mod_count)
        throw ConcurrentModificationError("HashMap::Iterator::operator ++");
    if (current.second != nullptr )
        if (can_erase|| current.second->next == nullptr)
                 advance_cursors();
    else {
        can_erase = true;
    }
    return *this;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
auto  HashMap<KEY,T,thash>::Iterator::operator ++ (int) -> HashMap<KEY,T,thash>::Iterator {
    if (expected_mod_count != ref_map->mod_count)
        throw ConcurrentModificationError("HashMap::Iterator::operator ++(int)");

    Iterator to_return(*this);
    if (current.second != nullptr)
        advance_cursors();
    else {
        can_erase = true;
    }
    return to_return;

}


template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::Iterator::operator == (const HashMap<KEY,T,thash>::Iterator& rhs) const {
    const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
    if (rhsASI == 0)
        throw IteratorTypeError("HashMap::Iterator::operator ==");
    if (expected_mod_count != ref_map->mod_count)
        throw ConcurrentModificationError("HashMap::Iterator::operator ==");
    if (ref_map != rhs.ref_map)
        throw ComparingDifferentIteratorsError("HashMap::Iterator::operator ==");

    return current.second == rhsASI->current.second;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
bool HashMap<KEY,T,thash>::Iterator::operator != (const HashMap<KEY,T,thash>::Iterator& rhs) const {
    const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
    if (rhsASI == 0)
        throw IteratorTypeError("HashMap::Iterator::operator !=");
    if (expected_mod_count != ref_map->mod_count)
        throw ConcurrentModificationError("HashMap::Iterator::operator !=");
    if (ref_map != rhs.ref_map)
        throw ComparingDifferentIteratorsError("HashMap::Iterator::operator !=");

    return current.second != rhsASI->current.second;
}


template<class KEY,class T, int (*thash)(const KEY& a)>
pair<KEY,T>& HashMap<KEY,T,thash>::Iterator::operator *() const {
    if (expected_mod_count != ref_map->mod_count)
        throw ConcurrentModificationError("HashMap::Iterator::operator *");
    if (!can_erase || current.second == nullptr)
        throw IteratorPositionIllegal("HashMap::Iterator::operator * Iterator illegal");

    return current.second->value;

}


template<class KEY,class T, int (*thash)(const KEY& a)>
pair<KEY,T>* HashMap<KEY,T,thash>::Iterator::operator ->() const {
    if (expected_mod_count !=ref_map->mod_count)
        throw ConcurrentModificationError("HashMap::Iterator::operator ->");
    if (!can_erase || current.second == nullptr)
        throw IteratorPositionIllegal("HashMap::Iterator::operator -> Iterator illegal");

    return &(current.second->value);

}
}

#endif /* HASH_MAP_HPP_ */
