//
// Created by Kyle Gendreau on 5/30/21.
//

#ifndef SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_H
#define SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_H

#include <algorithm>
#include "Iterator.h"

/**
 * Although not all returns must be used or assumed to be encountered, these are the suggested values to handle
 *
 * -1 if cur node's key is equivalent to input,
 *      ie you have found the end of the node-sequence, and are now at the node that will hold the value for the article
 *
 * 0 through S-1 if new node to check,
 *      ie the return is the index for the next child to move onto
 *
 * -2 if target but not equivalent and curNode and/or its children must be modded
 *      common example: curNode points to nullptr
 *      radix trie ex: curNode's key is too big for input key (ie input key is "ai" and curNode's key is "ail")
 *
 * -3 if curNode key doesn't allow for -2 through S,
 *      ie something must be modded between curNode and its parent
 *      radix trie ex: the root node holds the letter "a", but you want to add "bee"
 */
template <typename K, typename Value, uint32_t S>
class Awful_Indexer {
public:
    int32_t operator()(K& input, const Node<K,Value,S>* node) {
        if (node != nullptr) {
            if (input == node->key)
                return -1;
            else if (input < node->key)
                return 0;
            else
                return 1;
        }
        return -2; // points to nullptr
    }
};

/**
 * Modified the key as it goes through the trie, can be dependant on the node and signal (the signal is the
 * output from the Indexer)
 */
template <typename K, typename Value, uint32_t S>
class Awful_KeyModif {
public:
    K operator()(K& input, Node<K,Value,S>* &curNode, Node<K,Value,S>* &parentNode, int32_t signal) {
        if (signal == -2)
            curNode = new Node<K,Value,S>(parentNode,input);
        return input;
    }

    K operator()(K& input, const Node<K,Value,S>* const &curNode, const Node<K,Value,S>* const &parentNode, int32_t signal) {
        return input;
    }
};

/**
 *
 */
template <typename K, typename Value, uint32_t S>
class Awful_Eraser {
public:
    void operator()(Iterator<K,Value,S>) {
    }
};

template <typename K, typename T, uint32_t S = 2, typename Indexer = Awful_Indexer<K,T,S>, typename Modifier =
Awful_KeyModif<K,T,S>, typename Eraser = Awful_Eraser<K,T,S>>
class SimpleTrieTemplate {
public:

    typedef K                                       key_type;
    typedef T                                       mapped_type;
    typedef Indexer                                 key_indexer;
    typedef Modifier                                key_modifier;
    typedef Eraser                                  key_eraser;
    typedef Iterator<key_type,mapped_type,S>        iterator;
    typedef Node<key_type,mapped_type,S>            Node;

    // default ctor
    explicit SimpleTrieTemplate();

    // copy ctor
    explicit SimpleTrieTemplate(const SimpleTrieTemplate& rhs);

    // dtor
    ~SimpleTrieTemplate() = default;

    // assignment op
    SimpleTrieTemplate &operator=(const SimpleTrieTemplate& rhs);

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
    iterator insert(key_type article, mapped_type&& value);
    iterator insert(key_type article, std::forward_list<mapped_type>&& value);
    iterator insert(key_type article, mapped_type& value);
    iterator insert(key_type article, std::forward_list<mapped_type>& value);

    /**
     * erases articles within trie
     * @param article - the key being deleted
     * @return - true if an element was deleted, false if not
     */
    bool erase(key_type article);

    /**
     * calles eraser on iterator at the end of the article's node sequence
     * @param pos - the iterator at the end of the article's node sequence
     */
    void erase(iterator pos);

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
    iterator find(const key_type& article);

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
    std::pair<bool,std::unique_ptr<iterator>> scout(const key_type& article);

    /**
     * checks if the container contains element
     * @param element - the key being searched for
     * @return - true if in container, else false
     */
    bool contains(const key_type& article);

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

    std::unique_ptr<Node> root;

    key_indexer indexer;

    key_modifier modifier;

    key_eraser eraser;

//////////////////////////////////////////////////////
//// PRIVATE HELPER METHODS

    std::pair<bool,std::unique_ptr<iterator>> scout_helper(key_type& key, const Node* node);

    std::pair<bool,std::unique_ptr<iterator>> scout_helper_foundTarget(key_type& key, Node* node);

    std::pair<bool,std::unique_ptr<iterator>> scout_helper_childAccess(key_type& key, Node* node, uint32_t signal);

    iterator insert_recursive(Node* &curNode,Node* parentNode,key_type& key, std::forward_list<mapped_type>& value);

    void valid_childAccess_helper(const Node& node, const uint32_t& signal);

};

#include "../src/SimpleTrieTemplate.cpp"

#endif //SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_H
