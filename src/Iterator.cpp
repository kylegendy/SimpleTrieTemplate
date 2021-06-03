//
// Created by Kyle Gendreau on 5/30/21.
//

#ifndef SIMPLETRIETEMPLATE_ITERATOR_CPP
#define SIMPLETRIETEMPLATE_ITERATOR_CPP

#include "../include/Iterator.h"

template<typename K, typename T, uint32_t S>
Iterator<K, T, S>::Iterator(Node<K, T, S> *currentNode, int32_t previousIndex) : curNode(currentNode), prevIndex(previousIndex) {
}

template<typename K, typename T, uint32_t S>
Iterator<K, T, S>::Iterator(Node<K, T, S> &currentNode, int32_t previousIndex) : curNode(&currentNode), prevIndex(previousIndex) {
}

template<typename K, typename T, uint32_t S>
Iterator<K, T, S>::Iterator(const Iterator &rhs) : curNode((rhs.curNode)), prevIndex(rhs.prevIndex) {
}

template<typename K, typename T, uint32_t S>
Node<K, T, S> &Iterator<K, T, S>::operator*() {
    return (curNode == nullptr) ? nullptr : *curNode;
}

template<typename K, typename T, uint32_t S>
Node<K, T, S> *Iterator<K, T, S>::operator->() {
    return curNode;
}

template<typename K, typename T, uint32_t S>
K &Iterator<K, T, S>::first() {
    return curNode->key;
}

template<typename K, typename T, uint32_t S>
std::forward_list<T> &Iterator<K, T, S>::second() {
    return curNode->value;
}

template<typename K, typename T, uint32_t S>
bool Iterator<K, T, S>::isArticleEnd() {
    return curNode == nullptr;
}

template<typename K, typename T, uint32_t S>
Iterator<K,T,S> &Iterator<K, T, S>::operator=(Iterator &rhs) {
    Iterator ph(rhs);
    std::swap(*this,rhs);
    return *this;
}

template<typename K, typename T, uint32_t S>
Iterator<K,T,S> &Iterator<K, T, S>::moveUp() {
    // todo deal with root
    int32_t index = findChildsIndex(curNode->getParent(), *curNode);
    assert(index > -1 && index < S);
    Iterator ph(curNode->getParent(), index);
    std::swap(*this, ph);
    return *this;
}

template<typename K, typename T, uint32_t S>
Iterator<K,T,S> &Iterator<K, T, S>::operator++() {

    Iterator nextIter;

    // now check the node-type, if its a component check for next child
    int nextIndex = findValidSucceedingChildIndex(*curNode,prevIndex);
    if (nextIndex < S) {
        nextIter = iterator(curNode->child.at(nextIndex), -1, (curNode->child.at(nextIndex)).begin());
        swap(*this, nextIter);
        return *this;
    }

    // at this point:
    // either node is leaf or has no valid child to go to next

    // check to see if curNode is not root node, ie its parent will not be nullptr
    if (curNode->getParent() != nullptr) {
        // find curNode's index as child in parent's children
        int parentsPrevIndex = findChildsIndex(curNode->getParent(), *curNode);
        nextIter = iterator(curNode->getParent(), parentsPrevIndex);
        ++nextIter; // increment for next node
    }
        // else its the root node, its parent is nullptr
    else {
        nextIter = iterator(curNode, S-1);
    }

    swap(*this, nextIter);

    // ensure your at an article
    if (!isArticleEnd())
        ++(*this);

    return *this;
}

template<typename K, typename T, uint32_t S>
Iterator<K,T,S> Iterator<K, T, S>::operator++(int) {
    Iterator ph(*this);
    ++(*this);
    return ph;
}

template<typename K, typename T, uint32_t S>
bool Iterator<K, T, S>::operator==(const Iterator &rhs) const {
    return curNode == rhs.curNode;
}

template<typename K, typename T, uint32_t S>
bool Iterator<K, T, S>::operator!=(const Iterator &rhs) const {
    return !(*this == rhs);
}

template<typename K, typename T, uint32_t S>
int32_t Iterator<K, T, S>::findValidSucceedingChildIndex(const Node<K, T, S> &parent, int32_t previousIndex) {
    int32_t wlkr(previousIndex + 1);
    while (wlkr < S) {
        if (parent.child.at(wlkr) != nullptr)
            break;
        ++wlkr;
    }
    return wlkr;
}

template<typename K, typename T, uint32_t S>
int32_t Iterator<K, T, S>::findChildsIndex(const Node<K, T, S> &parent, const Node<K, T, S> &child) {
    for (uint32_t i(0); i < S; ++i) {
        if (parent.child.at(i) == &child)
            return i;
    }
    return -1;
}

#endif // SIMPLETRIETEMPLATE_ITERATOR_CPP