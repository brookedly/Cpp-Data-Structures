// Submitter: brookedl(Ly, Brooke)
// Partner  : jiayueg(Gao, Isley)
// We certify that we worked cooperatively on this programming
//   assignment, according to the rules for pair programming
#ifndef BST_MAP_HPP_
#define BST_MAP_HPP_

#include <string>
#include <iostream>
#include <sstream>
#include <initializer_list>
#include "ics_exceptions.hpp"
#include "pair.hpp"
#include "array_queue.hpp"   //For traversal


namespace ics {


#ifndef undefinedltdefined
#define undefinedltdefined
    template<class T>
    bool undefinedlt (const T& a, const T& b) {return false;}
#endif /* undefinedltdefined */

//Instantiate the templated class supplying tgt(a,b): true, iff a has higher priority than b.
//If tgt is defaulted to undefinedlt in the template, then a constructor must supply cgt.
//If both tlt and clt are supplied, then they must be the same (by ==) function.
//If neither is supplied, or both are supplied but different, TemplateFunctionError is raised.
//The (unique) non-undefinedlt value supplied by tlt/clt is stored in the instance variable gt.
    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b) = undefinedlt<KEY>> class BSTMap {
    public:
        typedef pair<KEY,T> Entry;
        typedef bool (*ltfunc) (const KEY& a, const KEY& b);

        //Destructor/Constructors
        ~BSTMap();

        BSTMap          (bool (*clt)(const KEY& a, const KEY& b) = undefinedlt<KEY>);
        BSTMap          (const BSTMap<KEY,T,tlt>& to_copy, bool (*clt)(const KEY& a, const KEY& b) = undefinedlt<KEY>);
        explicit BSTMap (const std::initializer_list<Entry>& il, bool (*clt)(const KEY& a, const KEY& b) = undefinedlt<KEY>);

        //Iterable class must support "for-each" loop: .begin()/.end() and prefix ++ on returned result
        template <class Iterable>
        explicit BSTMap (const Iterable& i, bool (*clt)(const KEY& a, const KEY& b) = undefinedlt<KEY>);


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
        BSTMap<KEY,T,tlt>& operator = (const BSTMap<KEY,T,tlt>& rhs);
        bool operator == (const BSTMap<KEY,T,tlt>& rhs) const;
        bool operator != (const BSTMap<KEY,T,tlt>& rhs) const;

        template<class KEY2,class T2, bool (*lt2)(const KEY2& a, const KEY2& b)>
        friend std::ostream& operator << (std::ostream& outs, const BSTMap<KEY2,T2,lt2>& m);



        class Iterator {
        public:
            //Private constructor called in begin/end, which are friends of BSTMap<T>
            ~Iterator();
            Entry       erase();
            std::string str  () const;
            BSTMap<KEY,T,tlt>::Iterator& operator ++ ();
            BSTMap<KEY,T,tlt>::Iterator  operator ++ (int);
            bool operator == (const BSTMap<KEY,T,tlt>::Iterator& rhs) const;
            bool operator != (const BSTMap<KEY,T,tlt>::Iterator& rhs) const;
            Entry& operator *  () const;
            Entry* operator -> () const;
            friend std::ostream& operator << (std::ostream& outs, const BSTMap<KEY,T,tlt>::Iterator& i) {
                outs << i.str(); //Use the same meaning as the debugging .str() method
                return outs;
            }
            friend Iterator BSTMap<KEY,T,tlt>::begin () const;
            friend Iterator BSTMap<KEY,T,tlt>::end   () const;

        private:
            //If can_erase is false, the value has been removed from "it" (++ does nothing)
            ArrayQueue<Entry> it;                 //Queue for all associations (from begin); use it as iterator via dequeue
            BSTMap<KEY,T,tlt>* ref_map;
            int               expected_mod_count;
            bool              can_erase = true;

            //Called in friends begin/end
            Iterator(BSTMap<KEY,T,tlt>* iterate_over, bool from_begin);
        };


        Iterator begin () const;
        Iterator end   () const;


    private:
        class TN {
        public:
            TN ()                     : left(nullptr), right(nullptr){}
            TN (const TN& tn)         : value(tn.value), left(tn.left), right(tn.right){}
            TN (Entry v, TN* l = nullptr,
                TN* r = nullptr) : value(v), left(l), right(r){}

            Entry value;
            TN*   left;
            TN*   right;
        };

        bool (*lt) (const KEY& a, const KEY& b); // The lt used for searching BST (from template or constructor)
        TN* map       = nullptr;
        int used      = 0;                       //Cache for number of key->value pairs in the BST
        int mod_count = 0;                       //For sensing concurrent modification

        //Helper methods (find_key written iteratively, the rest recursively)
        TN*   find_key            (TN*  root, const KEY& key)                 const; //Returns reference to key's node or nullptr
        bool  has_value           (TN*  root, const T& value)                 const; //Returns whether value is is root's tree
        TN*   copy                (TN*  root)                                 const; //Copy the keys/values in root's tree (identical structure)
        void  copy_to_queue       (TN* root, ArrayQueue<Entry>& q)            const; //Fill queue with root's tree value
        bool  equals              (TN*  root, const BSTMap<KEY,T,tlt>& other) const; //Returns whether root's keys/value are all in other
        std::string string_rotated(TN* root, std::string indent)              const; //Returns string representing root's tree

        T     insert              (TN*& root, const KEY& key, const T& value);       //Put key->value, returning key's old value (or new one's, if key absent)
        T&    find_addempty       (TN*& root, const KEY& key);                       //Return reference to key's value (adding key->T() first, if key absent)
        Entry remove_closest      (TN*& root);                                       //Helper for remove
        T     remove              (TN*& root, const KEY& key);                       //Remove key->value from root's tree
        void  delete_BST          (TN*& root);                                       //Deallocate all TN in tree; root == nullptr
    };





////////////////////////////////////////////////////////////////////////////////
//
//BSTMap class and related definitions

//Destructor/Constructors

    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    BSTMap<KEY,T,tlt>::~BSTMap() {
        clear();
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    BSTMap<KEY,T,tlt>::BSTMap(bool (*clt)(const KEY& a, const KEY& b))
            : lt( tlt != (ltfunc)undefinedlt<KEY> ? tlt : clt)
    {
        if (lt == nullptr)
            throw TemplateFunctionError("BSTMap::default constructor: neither specified");
        if (tlt != (ltfunc)undefinedlt<KEY> && clt != (ltfunc)undefinedlt<KEY> && tlt != clt)
            throw TemplateFunctionError("BSTMap::default constructor: both specified and different");
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    BSTMap<KEY,T,tlt>::BSTMap(const BSTMap<KEY,T,tlt>& to_copy, bool (*clt)(const KEY& a, const KEY& b))
            : lt(to_copy.lt), used(to_copy.used)
    {
        if (lt == (ltfunc)undefinedlt<KEY>) {
            throw TemplateFunctionError("BSTMap::copy constructor: neither specified");
        }
        if (tlt != (ltfunc)undefinedlt<KEY> && clt != (ltfunc)undefinedlt<KEY> && tlt != clt) {
            throw TemplateFunctionError("BSTMap::copy constructor: both specified");
        }
        *this = to_copy;
//        copy(to_copy.map);

    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    BSTMap<KEY,T,tlt>::BSTMap(const std::initializer_list<Entry>& il, bool (*clt)(const KEY& a, const KEY& b))
            : lt(tlt != (ltfunc)undefinedlt<KEY> ? tlt : clt)
    {
        if (lt == (ltfunc)undefinedlt<KEY>) {
            throw TemplateFunctionError("BSTMap::initializer_list constructor: neither specified");
        }
        if (tlt != (ltfunc)undefinedlt<KEY> && clt != (ltfunc)undefinedlt<KEY> && tlt != clt) {
            throw TemplateFunctionError("BSTMap::initializer_list constructor: both specified");
        }

        for (const Entry& kv : il)
            put(kv.first,kv.second);
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    template <class Iterable>
    BSTMap<KEY,T,tlt>::BSTMap(const Iterable& i, bool (*clt)(const KEY& a, const KEY& b))
            : lt(tlt != (ltfunc)undefinedlt<KEY> ? tlt : clt)
    {
        if (lt == (ltfunc)undefinedlt<KEY>) {
            throw TemplateFunctionError("BSTMap::initializer_list constructor: neither specified");
        }
        if (tlt != (ltfunc)undefinedlt<KEY> && clt != (ltfunc)undefinedlt<KEY> && tlt != clt) {
            throw TemplateFunctionError("BSTMap::initializer_list constructor: both specified");
        }

        for (const Entry& kv : i)
            put(kv.first,kv.second);

    }


////////////////////////////////////////////////////////////////////////////////
//
//Queries

    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    bool BSTMap<KEY,T,tlt>::empty() const {
        return used == 0;
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    int BSTMap<KEY,T,tlt>::size() const {
        return used;
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    bool BSTMap<KEY,T,tlt>::has_key (const KEY& key) const {
        if (map == nullptr){
            return false;
        }
        else if(map->value.first == key){
            return true;
        }
        else if(map->value.first != key){
            return false;
        }
        else{
            if(key < map->value.first){
                has_key(map->left->value.first);
            }
            else {
                has_key(map->right->value.first);
            }
        }

    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    bool BSTMap<KEY,T,tlt>::has_value (const T& value) const {
        return has_value(map, value);
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    std::string BSTMap<KEY,T,tlt>::str() const {
        std::ostringstream answer;
        answer << "BSTMap[";

        answer << "]" << ",used=" << used << ",mod_count=" << mod_count << ")";
        return answer.str();
    }


////////////////////////////////////////////////////////////////////////////////
//
//Commands

    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    T BSTMap<KEY,T,tlt>::put(const KEY& key, const T& value) {
        ++mod_count;
        return insert(map, key, value);
    }



    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    T BSTMap<KEY,T,tlt>::erase(const KEY& key) {
        T to_remove = remove(map, key);
        ++mod_count;
        --used;
        return to_remove;
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    void BSTMap<KEY,T,tlt>::clear() {
        delete_BST(map);
        used = 0;
        ++mod_count;
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    template<class Iterable>
    int BSTMap<KEY,T,tlt>::put_all(const Iterable& i) {
        int count = 0;
//        std::cout << "before " << std::endl;

        for (auto kv: i){
            ++count;
//            std::cout << "HI IM HERE " << std::endl;
            put(kv.first, kv.second);
        }
        return count;
    }


////////////////////////////////////////////////////////////////////////////////
//
//Operators

    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    T& BSTMap<KEY,T,tlt>::operator [] (const KEY& key) {
        return find_addempty(map, key);

    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    const T& BSTMap<KEY,T,tlt>::operator [] (const KEY& key) const {
        TN *k = find_key(map, key);
        if (k != nullptr)
            //therefore it exist
            return k->value.second;
        std::ostringstream answer;
        answer << "BST_Map::operator []: key(" << key << ") not in Map";
        throw KeyError(answer.str());
//        return T();

    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    BSTMap<KEY,T,tlt>& BSTMap<KEY,T,tlt>::operator = (const BSTMap<KEY,T,tlt>& rhs) {
        if (this == &rhs)
            return *this;
        clear();
        used = rhs.used;
//        if (map == nullptr)
//            insert(rhs.map, rhs.map->value.first, rhs.map->value.second);
        map = copy(rhs.map);
        ++mod_count;
        return *this;
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    bool BSTMap<KEY,T,tlt>::operator == (const BSTMap<KEY,T,tlt>& rhs) const {
        if (this == &rhs)
            return true;
        if (used != rhs.size())
            return false;
        else {
            return equals(map, rhs);
        }
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    bool BSTMap<KEY,T,tlt>::operator != (const BSTMap<KEY,T,tlt>& rhs) const {
        return !(*this == rhs);
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    std::ostream& operator << (std::ostream& outs, const BSTMap<KEY,T,tlt>& m) {
        outs << "map[";
        if (!m.empty()) {
            outs << m.map->value.first << "->" << m.map->value.second;
        }
        outs << "]";
        return outs;
    }


////////////////////////////////////////////////////////////////////////////////
//
//Iterator constructors

    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    auto BSTMap<KEY,T,tlt>::begin () const -> BSTMap<KEY,T,tlt>::Iterator {
        return Iterator(const_cast<BSTMap<KEY, T, tlt>*>(this),true);

    }

    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    auto BSTMap<KEY,T,tlt>::end () const -> BSTMap<KEY,T,tlt>::Iterator {
        return Iterator(const_cast<BSTMap<KEY, T, tlt>*>(this),false);

    }

////////////////////////////////////////////////////////////////////////////////
//
//Private helper methods

    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    typename BSTMap<KEY,T,tlt>::TN* BSTMap<KEY,T,tlt>::find_key (TN* root, const KEY& key) const {
        if(root==nullptr || root->value.first == key ){
            return root;
        }

        else{
            if (lt(key, root->value.first)) {
                return find_key(root->left, key);
            }
            else {
                return find_key(root->right, key);
            }
        }
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    bool BSTMap<KEY,T,tlt>::has_value (TN* root, const T& value) const {
        if (root == nullptr)
            return false;
        else if (root->value.second == value)
            return true;
        else if (root->value.second != value)
            return false;
        else {
            return has_value(root->left, value) || has_value(root->right, value);
        }
    }

    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    typename BSTMap<KEY,T,tlt>::TN* BSTMap<KEY,T,tlt>::copy (TN* root) const {
        if (root == nullptr)
            return nullptr;
        else
            return new TN(root->value, copy(root->left), copy(root->right));
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    void BSTMap<KEY,T,tlt>::copy_to_queue (TN* root, ArrayQueue<Entry>& q) const {
       if (root == nullptr)
           return;

       else {
           //Depth first (preorder) traversing
           q.enqueue(root->value);
           copy_to_queue(root->left, q);
           copy_to_queue(root->right, q);

       }
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    bool BSTMap<KEY,T,tlt>::equals (TN* root, const BSTMap<KEY,T,tlt>& other) const {
        if (root == other.map){
            return true;
        }
        if(root == nullptr || other.map == nullptr)
        {
            return true;
        }
        if(other[root->value.first] != root->value.second)
            //if the value at root's key isn't the same with other return false
        {
            return false;
        }
        if(other[root->value.first] == root->value.second)
        {
            return equals(root->left, other) && equals(root->right, other);
            //we need to check the other left and right TNs
        }
        else
        {
            if (equals(root->left, other) && equals(root->right, other))
                return true;
            else {
                return false;
            }
        }
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    std::string BSTMap<KEY,T,tlt>::string_rotated(TN* root, std::string indent) const {
    }

    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
     T BSTMap<KEY,T,tlt>::insert (TN*& root, const KEY& key, const T& value) { 
        if(root==nullptr){
            root = new TN(Entry{key, value});
            used++;
            return value;
        }else{
            if(key < root->value.first){
                return insert(root->left, key, value);
            }
            else if (key > root->value.first){
                return insert(root->right, key, value);
            }
            else{
                T getrid = root->value.second;
                root->value.second = value;
                return getrid;
            }
        }
    }



    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    T& BSTMap<KEY,T,tlt>::find_addempty (TN*& root, const KEY& key) {
        if(root==nullptr){
            root = new TN(Entry(key,T()));
            ++used;
            return root->value.second;
        }else{
            if(key < root->value.first){
                return find_addempty(root->left, key);
            }
            else if (key > root->value.first){
                return find_addempty(root->right, key);
            }
            else {
                return root->value.second;
            }
        }
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    pair<KEY,T> BSTMap<KEY,T,tlt>::remove_closest(TN*& root) {
        if (root->right != nullptr)
            return remove_closest(root->right);
        else{
            Entry to_return = root->value;
            TN* to_delete = root;
            root = root->left;
            delete to_delete;
            return to_return;
        }
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    T BSTMap<KEY,T,tlt>::remove (TN*& root, const KEY& key) {
        if (root == nullptr) {
            std::ostringstream answer;
            answer << "BSTMap::erase: key(" << key << ") not in Map";
            throw KeyError(answer.str());
        }else
        if (key == root->value.first) {
            T to_return = root->value.second;
            if (root->left == nullptr) {
                TN* to_delete = root;
                root = root->right;
                delete to_delete;
            }else if (root->right == nullptr) {
                TN* to_delete = root;
                root = root->left;
                delete to_delete;
            }else
                root->value = remove_closest(root->left);
            return to_return;
        }else
            return remove( (lt(key,root->value.first) ? root->left : root->right), key);
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    void BSTMap<KEY,T,tlt>::delete_BST (TN*& root) {
        while (root != nullptr) 
            remove_closest(root);
    }



////////////////////////////////////////////////////////////////////////////////
//
//Iterator class definitions

    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    BSTMap<KEY,T,tlt>::Iterator::Iterator(BSTMap<KEY,T,tlt>* iterate_over, bool from_begin)
            :it(), ref_map(iterate_over), expected_mod_count(ref_map->mod_count)
    {
        if (from_begin == true)
            ref_map->copy_to_queue(ref_map->map, it);
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    BSTMap<KEY,T,tlt>::Iterator::~Iterator()
    {}


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    auto BSTMap<KEY,T,tlt>::Iterator::erase() -> Entry {
        if (expected_mod_count != ref_map->mod_count)
            throw ConcurrentModificationError("BSTMap::Iterator::erase");
        if (!can_erase)
            throw CannotEraseError("BSTMap::Iterator::erase Iterator cursor already erased");
        if (it.empty())
            throw CannotEraseError("BSTMap::Iterator::erase Iterator cursor beyond data structure");

        can_erase = false;
//        Entry to_return = it.peek();  //doesn't work because you have to get rid of the pair
        Entry to_return = it.dequeue(); //and dequeue does that
        ref_map->erase(to_return.first);
        expected_mod_count = ref_map->mod_count;
        return to_return;
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    std::string BSTMap<KEY,T,tlt>::Iterator::str() const {
        std::ostringstream answer;
        answer << ref_map->str() << "(it=" << it << ",expected_mod_count=" << expected_mod_count << ",can_erase=" << can_erase << ")";
        return answer.str();
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    auto  BSTMap<KEY,T,tlt>::Iterator::operator ++ () -> BSTMap<KEY,T,tlt>::Iterator& {
        if (expected_mod_count != ref_map->mod_count)
            throw ConcurrentModificationError("BSTMap::Iterator::operator ++");

        if (it.empty())
            return *this;

        if (can_erase)
//            ++current;
            it.dequeue();
        else
            can_erase = true;

        return *this;
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    auto BSTMap<KEY,T,tlt>::Iterator::operator ++ (int) -> BSTMap<KEY,T,tlt>::Iterator {
        if (expected_mod_count != ref_map->mod_count)
            throw ConcurrentModificationError("BSTMap::Iterator::operator ++(int)");

        if (it.empty())
            return *this;

        Iterator to_return(*this);
        if (can_erase)
//            ++current;
            it.dequeue();
        else
            can_erase = true;

        return to_return;
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    bool BSTMap<KEY,T,tlt>::Iterator::operator == (const BSTMap<KEY,T,tlt>::Iterator& rhs) const {
        const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
        if (rhsASI == 0)
            throw IteratorTypeError("BSTMap::Iterator::operator ==");
        if (expected_mod_count != ref_map->mod_count)
            throw ConcurrentModificationError("BSTMap::Iterator::operator ==");
        if (ref_map != rhsASI->ref_map)
            throw ComparingDifferentIteratorsError("BSTMap::Iterator::operator ==");

        return it == rhsASI->it;
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    bool BSTMap<KEY,T,tlt>::Iterator::operator != (const BSTMap<KEY,T,tlt>::Iterator& rhs) const {
        const Iterator* rhsASI = dynamic_cast<const Iterator*>(&rhs);
        if (rhsASI == 0)
            throw IteratorTypeError("BSTMap::Iterator::operator !=");
        if (expected_mod_count != ref_map->mod_count)
            throw ConcurrentModificationError("BSTMap::Iterator::operator !=");
        if (ref_map != rhsASI->ref_map)
            throw ComparingDifferentIteratorsError("BSTMap::Iterator::operator !=");

        return it != rhsASI->it;
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    pair<KEY,T>& BSTMap<KEY,T,tlt>::Iterator::operator *() const {
        if (expected_mod_count != ref_map->mod_count)
            throw ConcurrentModificationError("BSTMap::Iterator::operator *");
        if (!can_erase || it.empty()) {

            throw IteratorPositionIllegal("BSTMap::Iterator::operator * Iterator illegal: ");
        }

        return it.peek();
    }


    template<class KEY,class T, bool (*tlt)(const KEY& a, const KEY& b)>
    pair<KEY,T>* BSTMap<KEY,T,tlt>::Iterator::operator ->() const {
        if (expected_mod_count != ref_map->mod_count)
            throw ConcurrentModificationError("BSTMap::Iterator::operator ->");
        if (!can_erase || it.empty()) {

            throw IteratorPositionIllegal("BSTMap::Iterator::operator -> Iterator illegal: ");
        }

        return &(it.peek());

    }


}

#endif /* BST_MAP_HPP_ */
