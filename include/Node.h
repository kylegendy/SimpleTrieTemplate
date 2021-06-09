//
// Created by Kyle Gendreau on 5/30/21.
//

#ifndef SIMPLETRIETEMPLATE_NODE_H
#define SIMPLETRIETEMPLATE_NODE_H

#include <forward_list>
#include <vector>

template<typename K, typename T, uint32_t S>
struct Node {

    explicit Node(Node* parentPtr = nullptr, K key = K(), std::forward_list<T> value = std::forward_list<T>());

    explicit Node(const Node& rhs);

    ~Node() = default;

    void swap(Node &rhs);

    Node& operator=(const Node& rhs);

    bool operator==(const Node& rhs);

    bool operator!=(const Node& rhs);

    Node* parent;

    K key;

    std::forward_list<T> value;

    std::vector<std::unique_ptr<Node<K,T,S>>> child;

};

#include "../src/Node.cpp"

#endif //SIMPLETRIETEMPLATE_NODE_H
