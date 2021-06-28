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
    int32_t operator()(const K& input, const Node<K,Value,S>* node) {
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
    K operator()(K& input, Node<K,Value,S>* &curNode, const int32_t &signal) {
        if (signal == -2)
            curNode = new Node<K,Value,S>( ((curNode == nullptr) ? nullptr : curNode->parent) ,input);
        return input;
    }

    K operator()(K& input, const Node<K,Value,S>* const &curNode, const int32_t &signal) {
        return input;
    }
};

// forward declarations
template<typename K, typename Value, uint32_t S, typename Indexer, typename Modifier>
class Awful_Eraser;

template <typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
class SimpleTrieTemplate;

/**
 *
 */
template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier>
class Awful_Eraser {
public:
    void operator()(Iterator<K,T,S> &ancestor, Iterator<K,T,S> &descendant, SimpleTrieTemplate<K,T,S,Indexer,Modifier,Awful_Eraser<K,T,S,Indexer,Modifier>> &trie) {
        // create pointer that points to merged children
        Node<K,T,S>* mergedChild;
        std::forward_list<std::pair<K,std::forward_list<T>>> list, other;

        list.merge(makeList(descendant->child.at(0).get()));
        list.merge(makeList(descendant->child.at(1).get()));

        if (descendant->parent == nullptr) {
            trie.clear();

            while (!list.empty()) {
                trie.insert(list.front());
                list.pop_front();
            }
        }
        else {
            // find index of descendant in parent's child
            int index = Iterator<K,T,S>::findChildsIndex(*(descendant->parent),*descendant);

            descendant.moveUp();
            // reset child at index with new merged pointer
            descendant->child.at(index).reset();

            while (!list.empty()) {
                trie.insert(list.front(),&descendant);
                list.pop_front();
            }
        }


    }

    std::forward_list<std::pair<K,std::forward_list<T>>> makeList(Node<K,T,S>* node) {
        std::forward_list<std::pair<K,std::forward_list<T>>> lst;
        if (node == nullptr) {
            return lst;
        }

        lst.merge(makeList(node->child.at(0).get()));
        lst.merge(makeList(node->child.at(1).get()));

        lst.push_front(std::pair<K,std::forward_list<T>>(node->key,node->value));
        return lst;
    }
};

template <typename K, typename T, uint32_t S = 2, typename Indexer = Awful_Indexer<K,T,S>, typename Modifier =
Awful_KeyModif<K,T,S>, typename Eraser = Awful_Eraser<K,T,S,Indexer,Modifier>>
class SimpleTrieTemplate {
public:

    typedef K                                       key_type;
    typedef T                                       mapped_type;
    typedef Indexer                                 key_indexer;
    typedef Modifier                                key_modifier;
    typedef Eraser                                  key_eraser;
    typedef Iterator<key_type,mapped_type,S>        iterator;
    typedef Node<key_type,mapped_type,S>            Node;

    //default ctor
    explicit SimpleTrieTemplate();

    // copy ctor
    explicit SimpleTrieTemplate(const SimpleTrieTemplate& rhs);

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
    iterator insert(std::pair<key_type,mapped_type>& p, iterator* ancestor = nullptr);
    iterator insert(std::pair<key_type,mapped_type>&& p, iterator* ancestor = nullptr);

    iterator insert(key_type article, mapped_type& value, iterator* ancestor = nullptr);
    iterator insert(key_type article, mapped_type&& value, iterator* ancestor = nullptr);
    //endregion

    /**
     * erases articles within trie
     * @param article - the key being deleted
     */
     //region //erase();
    void erase(key_type article,iterator* ancestor = nullptr);

     // erase article with descendant as end of its node sequence, must stop at ancestor
    void erase(iterator& descendant, iterator* ancestor = nullptr);
    void erase(iterator&& descendant, iterator* ancestor = nullptr);
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
    std::pair<bool,std::unique_ptr<iterator>> scout(key_type article,iterator* ancestor = nullptr);

    /**
     * checks if the container contains element
     * @param element - the key being searched for
     * @return - true if in container, else false
     */
    bool contains(key_type article,iterator* ancestor = nullptr);

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

    key_modifier modifier;

    key_eraser eraser;

    Node* root;

//////////////////////////////////////////////////////
//// PRIVATE HELPER METHODS


};

#include "../src/SimpleTrieTemplate.cpp"

#endif //SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_H
