//
// Created by Kyle Gendreau on 5/30/21.
//

#ifndef SIMPLETRIETEMPLATE_NODE_CPP
#define SIMPLETRIETEMPLATE_NODE_CPP

#include "../include/Node.h"

template<typename K, typename T, uint32_t S>
Node<K, T, S>::Node(Node* parentPtr, K key, std::forward_list<T> value) : parent(parentPtr), key(key), value(value), child(std::vector<std::unique_ptr<Node<K,T,S>>>(S)) {
}

template<typename K, typename T, uint32_t S>
Node<K, T, S>::Node(const Node &rhs) : parent(nullptr), key(rhs.key), value(rhs.value), child(std::vector<std::unique_ptr<Node<K,T,S>>>(S)) {
    for (uint32_t i(0); i < S; ++i) {
        if (rhs.child.at(i).get() != nullptr) {
            child.at(i) = std::make_unique<Node<K, T, S>>(*(rhs.child.at(i)));
            child.at(i)->parent = this;
        }
    }
}

template<typename K, typename T, uint32_t S>
void Node<K, T, S>::swap(Node &rhs) {
    std::swap(parent,rhs.parent);
    std::swap(key,rhs.key);
    std::swap(value,rhs.value);
    std::swap(child,rhs.child);
}

template<typename K, typename T, uint32_t S>
Node<K,T,S> &Node<K, T, S>::operator=(const Node &rhs) {
    if (this != &rhs) {
        Node* ph(new Node<K,T,S>(rhs));
        swap(*ph);
    }
    return *this;
}

template<typename K, typename T, uint32_t S>
bool Node<K, T, S>::operator==(const Node &rhs) {
    if (this == &rhs || ((key == rhs.key) && (value == rhs.value))) {
        for (uint32_t i(0); i < S; ++i) {
            if (child.at(i).get() == nullptr) {
                if (rhs.child.at(i).get() != nullptr)
                    return false;
            }
            else {
                if (rhs.child.at(i).get() == nullptr)
                    return false;

                if (*(child.at(i)) != *(rhs.child.at(i)))
                    return false;
            }
        }
        return true;
    }
    return false;
}

template<typename K, typename T, uint32_t S>
bool Node<K, T, S>::operator!=(const Node &rhs) {
    return !(*this == rhs);
}


#endif // SIMPLETRIETEMPLATE_NODE_CPP