//
// Created by Kyle Gendreau on 5/30/21.
//

#ifndef SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_CPP
#define SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_CPP

#include "../include/SimpleTrieTemplate.h"

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
SimpleTrieTemplate<K, T, S, Indexer, Eraser>::SimpleTrieTemplate() : numberArticles(0), indexer(), eraser() {
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
SimpleTrieTemplate<K, T, S, Indexer, Eraser>::SimpleTrieTemplate(const SimpleTrieTemplate &rhs) : numberArticles(rhs.numberArticles), indexer(rhs.indexer), eraser(rhs.eraser), root(new Node(*rhs.root)) {
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
SimpleTrieTemplate<K, T, S, Indexer, Eraser>::SimpleTrieTemplate(const SimpleTrieTemplate &&rhs) : numberArticles(rhs.numberArticles), indexer(rhs.indexer), eraser(rhs.eraser), root(new Node(*rhs.root)) {
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
SimpleTrieTemplate<K, T, S, Indexer, Eraser>::~SimpleTrieTemplate() {
    delete root;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
SimpleTrieTemplate<K, T, S, Indexer, Eraser> &SimpleTrieTemplate<K, T, S, Indexer, Eraser>::operator=(const SimpleTrieTemplate &rhs) {
    if (this != &rhs) {
        SimpleTrieTemplate ph(rhs);
        swap(ph);
    }
    return *this;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
SimpleTrieTemplate<K, T, S, Indexer, Eraser> &SimpleTrieTemplate<K, T, S, Indexer, Eraser>::operator=(const SimpleTrieTemplate &&rhs) {
    return *this = rhs;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
bool SimpleTrieTemplate<K, T, S, Indexer, Eraser>::empty() const noexcept {
    return size() == 0 && begin() != end(); //todo simplify after finish
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
uint32_t SimpleTrieTemplate<K, T, S, Indexer, Eraser>::size() const noexcept {
    return numberArticles;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Eraser>::clear() noexcept {
    if (!empty()) {
        root->child_.at(0).reset();
        numberArticles = 0;
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Eraser>::insert(std::pair<key_type, mapped_type> &p, Node* ancestor) {
    checkIterPtr_helper(ancestor);
    if (empty()) { //ie if curNode == end().get() && curNode == begin().get()
        assert(ancestor == root);
        Node* ptr(ancestor->child_.at(0).get());
        auto it(insert_helper(ptr,p.first,p.second));
        ancestor->child_.at(0).reset(ptr);
        return it;
    }
    else
        return insert_helper(ancestor->child_.at(0).get(),p.first,p.second);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Eraser>::insert(std::pair<key_type, mapped_type> &&p, Node* ancestor) {
    return insert(p,ancestor);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, Indexer, Eraser>::insert(key_type article, mapped_type &value, Node* ancestor) {
    checkIterPtr_helper(ancestor);
    if (empty()) { //ie if curNode == end().get() && curNode == begin().get()
        assert(ancestor == root);
        Node* ptr(ancestor->child_.at(0).get());
        auto it(insert_helper(ptr,article,value));
        ancestor->child_.at(0).reset(ptr);
        return it;
    }
    else
        return insert_helper(ancestor->child_.at(0).get(),article,value);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, Indexer, Eraser>::insert(key_type article, mapped_type &&value, Node* ancestor) {
    return insert(article,value,ancestor);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Eraser>::erase(key_type article,Node* &ancestor) {
    auto p(scout(article,ancestor));
    if (p.first) {
        erase(p.second.get(),ancestor);
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Eraser>::erase(Node* &descendant,Node* &ancestor) {
    if (!empty()) {
        uint32_t curSize(size());
        checkIterPtr_helper(ancestor);
        eraser(ancestor, descendant, *this);
        numberArticles = --curSize;
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Eraser>::swap(SimpleTrieTemplate &rhs) {
    if (this != &rhs) {
        // numberArticles
        std::swap(numberArticles,rhs.numberArticles);

        // indexer
        std::swap(indexer,rhs.indexer);

        // eraser
        std::swap(eraser,rhs.eraser);

        // root
        std::swap(root,rhs.root);
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Eraser>::find(key_type article) {
    auto couple(scout(article));
    return (couple.first) ? couple.second : end();
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
std::pair<bool, Iterator<K,T,S>>
SimpleTrieTemplate<K, T, S, Indexer, Eraser>::scout(key_type article,Node* ancestor) {
    if (empty()) {
        return std::pair<bool, std::unique_ptr<iterator>>(false,begin());
    }
    checkIterPtr_helper(ancestor);
    return scout_helper(article, ancestor->get());
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
bool SimpleTrieTemplate<K, T, S, Indexer, Eraser>::contains(key_type article,Node* ancestor) {
    return scout(article,ancestor).first;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
bool SimpleTrieTemplate<K, T, S, Indexer, Eraser>::operator==(const SimpleTrieTemplate &rhs) const {
    if (this == &rhs) // check to see if same address
        return true;

    if (size() == rhs.size()) { // first check size
        // if empty, then equal, else check root equivalence
        return empty() ? true : *root->child_.at(0) == *rhs.root->child_.at(0);
    }
    return false;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
bool SimpleTrieTemplate<K, T, S, Indexer, Eraser>::operator!=(const SimpleTrieTemplate &rhs) const {
    return !(*this == rhs);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Eraser>::begin() {
    return ++iterator(*root,-1);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Eraser>::end() {
    return iterator(*root,0);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
std::pair<bool, Iterator<K,T,S>>
SimpleTrieTemplate<K, T, S, Indexer, Eraser>::scout_helper(key_type &key, const Node* &curNode) {
    int16_t index(indexer(key,curNode,*this));
    if (index > -1 && index < S) {
        if (curNode->child_.at(index).get() == nullptr)
            return std::pair<bool,iterator>(true,iterator(curNode,-1));
        else
            return scout_helper(key, *(curNode->child_.at(index)));
    }
    else if (index == -1) {
        return std::pair<bool,iterator>(true,iterator(curNode,-1));
    }
    else if (index == -2) {
        return std::pair<bool,iterator>(false,iterator(curNode,-1));
    }
    // else an invalid index was returned
    throw std::domain_error("indexer returned value outside of [-1," + std::to_string(S) + "]: " + std::to_string(index) + "\n");
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, Indexer, Eraser>::insert_helper(Node* &curNode, key_type &key, mapped_type &value) {
    int16_t index(indexer(key,curNode));
    if (index == -1) {
        *curNode->value_ = value;
        return iterator(curNode, -1);
    }
    //todo this forces all relative modification/indexing to not allow access to parents
    else if (index > -1 && index < S) {
        auto ptr(curNode->child_.at(index).get());
        auto it(insert_helper(ptr, key, value));
        if (curNode->child_.at(index).get() == nullptr) {
            curNode->child_.at(index).reset(ptr);
            curNode->child_.at(index)->parent_ = curNode;
        }
        return it;
    }
    // else an invalid index was returned
    throw std::domain_error("indexer  returned value outside of [-1," + std::to_string(S) + "]: " + std::to_string(index) + "\n");
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Eraser>::checkIterPtr_helper(Node* &ptr) {
    if (ptr == nullptr) {
        ptr = begin().get();
    }
}

#endif // SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_CPP