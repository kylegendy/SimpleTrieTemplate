//
// Created by Kyle Gendreau on 5/30/21.
//

#ifndef SIMPLETRIETEMPLATE_NODE_H
#define SIMPLETRIETEMPLATE_NODE_H

#include <vector>
#include <optional>

template<typename K, typename T, uint32_t S>
struct Node {

    explicit Node(Node* parent = nullptr);

    explicit Node(K key, Node* parent = nullptr);

    explicit Node(const Node& rhs);

    ~Node() = default;

    void swap(Node &rhs) noexcept;

    Node& operator=(const Node& rhs);
    Node& operator=(const Node&& rhs);

    bool operator==(const Node& rhs);

    bool operator!=(const Node& rhs);

    K key_;

    Node* parent_;

    std::optional<T> value_;

    std::vector<std::unique_ptr<Node<K,T,S>>> child_;

};

#include "../src/Node.cpp"

#endif //SIMPLETRIETEMPLATE_NODE_H
