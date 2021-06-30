//
// Created by Kyle Gendreau on 5/30/21.
//

#ifndef SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_H
#define SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_H

#include <algorithm>
#include <string>
#include "Iterator.h"

/**
 * -1 if cur node's key is equivalent to input,
 *      ie you have found the end of the node-sequence, and are now at the node that will hold the value for the article
 *
 * 0 through S-1 if new node to check,
 *      ie the return is the index for the next child to move onto
 *
 * -2 if not found
 */
template <typename K, typename T, uint32_t S>
class Awful_Indexer {
public:
    // called when not inserting
    int32_t operator()(K& input, const Node<K,T,S>* &node) {
        if (input == node->key) {
            return (node->value_) ? -1 : -2;
        }
        return (input < node->key) ? 0 : 1;
    }

    // called when inserting
    int32_t operator()(K& input, Node<K,T,S>* &node) {

        if (node == nullptr) {
            node = new Node<K, T, S>(input);
            return -1;
        }
        else if (input == node->key)
            return -1;
        else
            return (input < node->key) ? 0 : 1;
    }
};

// forward declaration
template <typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
class SimpleTrieTemplate;

/**
 *
 */
template<typename K, typename T, uint32_t S, typename Indexer>
class Awful_Eraser {
public:
    void operator()(Iterator<K,T,S> &ancestor, Iterator<K,T,S> &descendant, SimpleTrieTemplate<K,T,S,Indexer,Awful_Eraser<K,T,S,Indexer>> &trie) {
        if ()
    }
};

template <typename K, typename T, uint32_t S = 2, typename Indexer = Awful_Indexer<K,T,S>, typename Eraser = Awful_Eraser<K,T,S,Indexer>>
class SimpleTrieTemplate {
public:

    typedef K                                       key_type;
    typedef T                                       mapped_type;
    typedef Indexer                                 key_indexer;
    typedef Eraser                                  key_eraser;
    typedef Iterator<key_type,mapped_type,S>        iterator;
    typedef Node<key_type,mapped_type,S>            Node;

    //default ctor
    explicit SimpleTrieTemplate();

    // copy ctor
    explicit SimpleTrieTemplate(const SimpleTrieTemplate& rhs);
    explicit SimpleTrieTemplate(const SimpleTrieTemplate&& rhs);

    //DTOR
    ~SimpleTrieTemplate();

    //assignment op
    //region //operator=()
    SimpleTrieTemplate &operator=(const SimpleTrieTemplate& rhs);
    SimpleTrieTemplate &operator=(const SimpleTrieTemplate&& rhs);
    //endregion

//////////////////////////////////////////////////////
//// CAPACITY

    /**
     * checks whether the container is empty
     * @return - true if empty, false otherwise
     */
    bool empty() const noexcept;

    /**
     * returns the number of articles in trie
     * @return - unsigned int for number of elements in trie
     */
    uint32_t size() const noexcept;

//////////////////////////////////////////////////////
//// MODIFIERS

    /**
     * clears the contents, ie if not already empty, deletes the contents of the trie
     */
    void clear() noexcept;

    /**
     * inserts article if not already present, and adds value at end of article's sequence
     * @param article - the key used to find value's target
     * @param value - the value being stored at element
     * @return - an iterator to the node that holds value
     */
     //region //insert();
    iterator insert(std::pair<key_type,mapped_type>& p, Node* ancestor = nullptr);
    iterator insert(std::pair<key_type,mapped_type>&& p, Node* ancestor = nullptr);

    iterator insert(key_type article, mapped_type& value, Node* ancestor = nullptr);
    iterator insert(key_type article, mapped_type&& value, Node* ancestor = nullptr);
    //endregion

    /**
     * erases articles within trie
     * @param article - the key being deleted
     */
     //region //erase();
    void erase(key_type article,Node* &ancestor = nullptr);

     // erase article with descendant as end of its node sequence, must stop at ancestor
    void erase(Node* &descendant, Node* &ancestor = nullptr);
     //endregion

    /**
     * swaps the contents
     * @param rhs - the container to exchange the contents with
     */
    void swap(SimpleTrieTemplate& rhs);

//////////////////////////////////////////////////////
//// LOOKUPS

    /**
     * finds the node that holds the values for the article
     * @param article - the article being searched for
     * @return - an iterator at the last node in sequence for the article, else returns .end()
     */
    iterator find(key_type article);

    /**
     * finds the last viable node in sequence for the article. The returned pair contains, first, a boolean which holds
     * the same value as the contains() function returns, and second, an iterator at the last viable node in sequence.
     *
     * Example: given 'LOOKUP' was inserted in a BTrie, scout('LOOK') will return pair<false,node('K')>, and
     * scout('LOOKUP') will return pair<true,node('P')> (where node(N) represents the node at N letter in the node
     * sequence)
     *
     * @param article - the article being searched for
     * @return - returns a pair of a boolean and an iterator at the last viable node for the article's node sequence
     */
    std::pair<bool,iterator> scout(key_type article,Node* ancestor = nullptr);

    /**
     * checks if the container contains element
     * @param element - the key being searched for
     * @return - true if in container, else false
     */
    bool contains(key_type article,Node* ancestor = nullptr);

//////////////////////////////////////////////////////
//// COMPARERS

    bool operator==(const SimpleTrieTemplate& rhs) const;
    bool operator!=(const SimpleTrieTemplate& rhs) const;

//////////////////////////////////////////////////////
//// ITERATORS

    iterator begin();

    iterator end();

private:

//////////////////////////////////////////////////////
//// VARIABLES

    uint32_t numberArticles;

    key_indexer indexer;

    key_eraser eraser;

    Node* root;

//////////////////////////////////////////////////////
//// PRIVATE HELPER METHODS

    std::pair<bool, iterator> scout_helper(key_type& key, const Node* &curNode);

    iterator insert_helper(Node* &curNode, key_type& article, mapped_type& value);

    void checkIterPtr_helper(Node* &ptr);

};

#include "../src/SimpleTrieTemplate.cpp"

#endif //SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_H
