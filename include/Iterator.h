//
// Created by Kyle Gendreau on 5/30/21.
//

#ifndef SIMPLETRIETEMPLATE_ITERATOR_H
#define SIMPLETRIETEMPLATE_ITERATOR_H

#include "Node.h"
#include <iterator>
#include <list>

/**
 * @class Iterator
 * @brief bi-directional Iterator class, iterates through the nodes of a trie
 *
 * @tparam K - the key_type of the trie
 * @tparam T - the mapped_type of the trie
 * @tparam S - the number of child nodes for composites
 */
template <typename K, typename T, uint32_t S>
class Iterator  {
public:

    // default ctor is no more
    Iterator(Node<K,T,S> *currentNode = nullptr, int32_t previousIndex = -1);

    // alt ctor
    Iterator(Node<K,T,S> &currentNode, int32_t previousIndex);

    // copy ctor
    Iterator(const Iterator &rhs);

    // dtor
    ~Iterator() = default;


    /////////////////////////////////////////////////////////////////////////
    /// ACCESSORS

    /**
     * @return the node being pointed to by this
     */
    Node<K,T,S> &operator*();

    /**
     * @return a pointer to the node pointed to by this
     */
    Node<K,T,S> *operator->();

    /**
     * @return the key
     */
    K &first();

    /**
     * @return the list of values
     */
    std::forward_list<T> &second();

    /**
     * @return true if an end to an article, false otherwise
     */
    bool isArticleEnd();


    /////////////////////////////////////////////////////////////////////////
    /// MODIFIERS

    // assignment op
    Iterator &operator=(Iterator &rhs);

    /**
     * turns curNode into its parent, and prevNode into curNode
     * @return modified Iterator
     */
    Iterator &moveUp();

    /**
     * prefix increments Iterator to next value
     * @return this after increment
     */
    Iterator &operator++();

    /**
     * postfix increments Iterator to next value
     * @param rhs - the Iterator being on the right side of the ++ operator
     * @return the Iterator prior to the increment
     */
    Iterator operator++(int);

    /**
     * prefix decrements the Iterator
     * @return the decremented Iterator
     */
    // Iterator &operator--();

    /**
     * postfix decrements the Iterator
     * @param rhs - the Iterator being on the right side of the -- operator
     * @return
     */
    // Iterator operator--(int);


    /////////////////////////////////////////////////////////////////////////
    /// COMPARERS

    /**
     * compares equivalence
     * @param rhs - the other Iterator being compared to this
     * @return
     */
    bool operator==(const Iterator &rhs) const;

    /**
     * compares equivalence
     * @param rhs - the other Iterator being compared to this
     * @return
     */
    bool operator!=(const Iterator &rhs) const;

private:
    // current node being looked at
    Node<K,T,S>* curNode;

    // current node being looked at
    int32_t prevIndex;

    /**
     * looks for the next valid child after prevIndex and returns its index number, if none available then returns S
     * @return index of next valid child after prev, else S if none
     */
    static int32_t findValidSucceedingChildIndex(const Node<K,T,S> &parent, int32_t previousIndex);

    /**
     * looks for child node within parent's vector of children and returns the child's index.  if not found returns -1
     * @param parent - the parent whos vector of children is being searched through
     * @param child - the child that's being searched for
     * @return - the index of the child or -1 if not found
     */
    static int32_t findChildsIndex(const Node<K,T,S> &parent, const Node<K,T,S> &child);
};

#include "../src/Iterator.cpp"

#endif //SIMPLETRIETEMPLATE_ITERATOR_H
