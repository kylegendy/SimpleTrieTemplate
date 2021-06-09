//
// Created by Kyle Gendreau on 5/30/21.
//

#ifndef SIMPLETRIETEMPLATE_ITERATOR_CPP
#define SIMPLETRIETEMPLATE_ITERATOR_CPP

#include "../include/Iterator.h"

template<typename K, typename T, uint32_t S>
Iterator<K, T, S>::Iterator(Node<K, T, S> *currentNode, int32_t previousIndex) : curNode(currentNode), prevIndex(previousIndex) {
    basicNode = Node<K,T,S>();
}

template<typename K, typename T, uint32_t S>
Iterator<K, T, S>::Iterator(Node<K, T, S> &currentNode, int32_t previousIndex) : curNode(&currentNode), prevIndex(previousIndex) {
    basicNode = Node<K,T,S>();
}

template<typename K, typename T, uint32_t S>
Iterator<K, T, S>::Iterator(const Iterator &rhs) : curNode((rhs.curNode)), prevIndex(rhs.prevIndex) {
    basicNode = Node<K,T,S>();
}

template<typename K, typename T, uint32_t S>
Node<K, T, S> &Iterator<K, T, S>::operator*() {
    if (curNode == nullptr) {
        return basicNode;
    }
    else {
        return *curNode;
    }
}

template<typename K, typename T, uint32_t S>
Node<K, T, S>* Iterator<K, T, S>::operator->() {
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
void Iterator<K, T, S>::swap(Iterator &rhs) {
    int32_t ph(rhs.prevIndex);
    std::swap(curNode,rhs.curNode);
    rhs.prevIndex = prevIndex;
    prevIndex = ph;
}

template<typename K, typename T, uint32_t S>
Iterator<K,T,S> &Iterator<K, T, S>::operator=(Iterator &rhs) {
    Iterator ph(rhs);
    swap(rhs);
    return *this;
}

template<typename K, typename T, uint32_t S>
Iterator<K,T,S> &Iterator<K, T, S>::operator=(Iterator &&rhs) {
    return *this = rhs;
}

template<typename K, typename T, uint32_t S>
Iterator<K,T,S> &Iterator<K, T, S>::moveUp() {
    // todo deal with root
    int32_t index = findChildsIndex(*(curNode->parent), *curNode);
    assert(index > -1 && index < S);
    Iterator ph(curNode->parent, index);
    swap(ph);
    return *this;
}

template<typename K, typename T, uint32_t S>
Iterator<K,T,S> &Iterator<K, T, S>::operator++() {

    // if node* points to null
    if (curNode == nullptr) {
        ++prevIndex;
        return *this;
    }

    // instantiate common iterator
    Iterator nextIter;
    // find next valid child to go to
    int nextIndex = findValidSucceedingChildIndex(*curNode,prevIndex);

    // if valid child
    if (nextIndex < S) {
        nextIter = Iterator(curNode->child.at(nextIndex).get(), -1);
    }
    // at this point: no valid child to go to next

    // if parent not null, go up to get to next node
    else if (curNode->parent != nullptr) {
        // find curNode's index as child in parent's children
        int parentsPrevIndex = findChildsIndex(*(curNode->parent), *curNode);
        nextIter = Iterator(curNode->parent, parentsPrevIndex);
        ++nextIter;
    }

    // else return end iterator
    else {
        nextIter = Iterator(curNode, S-1);
    }

    swap(nextIter);
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
    // if root holds nullptr, then rhs.root must hold nullptr to be equal
    if (curNode == nullptr) {
        return rhs.curNode == nullptr;
    }
        // else root does not hold nullptr, and rhs.root must hold equivalent node as root to be equivalent
    else if (rhs.curNode != nullptr) {
        if (curNode->parent == nullptr && rhs.curNode->parent == nullptr)
            return *curNode == *(rhs.curNode) && prevIndex == rhs.prevIndex;
        return *curNode == *(rhs.curNode);
    }
    return false;
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
        if (parent.child.at(i).get() == &child)
            return i;
    }
    return -1;
}

#endif // SIMPLETRIETEMPLATE_ITERATOR_CPP