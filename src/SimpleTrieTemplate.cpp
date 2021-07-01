//
// Created by Kyle Gendreau on 5/30/21.
//

#ifndef SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_CPP
#define SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_CPP

#include "../include/SimpleTrieTemplate.h"

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
SimpleTrieTemplate<K, T, S, indexer_, eraser_>::SimpleTrieTemplate() : numberArticles_(0), indexer_(), eraser_(), root_(new Node) {
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
SimpleTrieTemplate<K, T, S, indexer_, eraser_>::SimpleTrieTemplate(const SimpleTrieTemplate &rhs) : numberArticles_(rhs.numberArticles_), indexer_(rhs.indexer_), eraser_(rhs.eraser_), root_(new Node(*rhs.root_)) {
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
SimpleTrieTemplate<K, T, S, indexer_, eraser_>::SimpleTrieTemplate(const SimpleTrieTemplate &&rhs) : numberArticles_(rhs.numberArticles_), indexer_(rhs.indexer_), eraser_(rhs.eraser_), root_(new Node(*rhs.root_)) {
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
SimpleTrieTemplate<K, T, S, indexer_, eraser_>::~SimpleTrieTemplate() {
    delete root_;
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
SimpleTrieTemplate<K, T, S, indexer_, eraser_> &SimpleTrieTemplate<K, T, S, indexer_, eraser_>::operator=(const SimpleTrieTemplate &rhs) {
    if (this != &rhs) {
        SimpleTrieTemplate ph(rhs);
        swap(ph);
    }
    return *this;
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
SimpleTrieTemplate<K, T, S, indexer_, eraser_> &SimpleTrieTemplate<K, T, S, indexer_, eraser_>::operator=(const SimpleTrieTemplate &&rhs) {
    return *this = rhs;
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
bool SimpleTrieTemplate<K, T, S, indexer_, eraser_>::empty() const noexcept {
    return size() == 0;
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
uint32_t SimpleTrieTemplate<K, T, S, indexer_, eraser_>::size() const noexcept {
    return numberArticles_;
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
void SimpleTrieTemplate<K, T, S, indexer_, eraser_>::clear() noexcept {
    if (!empty()) {
        root_->child_.at(0).reset();
        numberArticles_ = 0;
    }
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, indexer_, eraser_>::insertOrAssign(std::pair<key_type, mapped_type> &p, Node* ancestor) {
    return insertOrAssign(p.first,p.second,ancestor);
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, indexer_, eraser_>::insertOrAssign(std::pair<key_type, mapped_type> &&p, Node* ancestor) {
    return insertOrAssign(p,ancestor);
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, indexer_, eraser_>::insertOrAssign(key_type article, mapped_type &value, Node* ancestor) {

    checkIterPtr_helper(ancestor); // ensure ptr not null, else assign to begin() node
    uint32_t cnt(size()); // obtain original size

    auto it(begin()); // will be assigned later
    Node* ptr(nullptr); // will be assigned later

    // if it's empty, then we insert at root's first child.  There should ALWAYS be a root node, it holds junk values.
    if (empty()) { //ie if curNode == end().get() && curNode == begin().get()
        assert(ancestor == root_);
        it = insert_helper(ptr,article,value,indexer_);

        // CHANGES ARE NOW PERMANENT -- exception safe
        ancestor->child_.at(0).reset(ptr);
        ptr->parent_ = ancestor;
    }

    // else we insert the input directly at the node, not after!
    else {
        if (contains(article)) // if already present then count shouldn't change
            --cnt;
        ptr = new Node(*ancestor);
        it = insert_helper(ptr, article, value, indexer_);

        // CHANGES ARE NOW PERMANENT -- exception safe
        ptr->parent_ = ancestor->parent_;
        ancestor->parent_->child_.at(iterator::findChildsIndex(*ancestor->parent_, *ancestor)).reset(ptr);
        ancestor = ptr;
    }

    numberArticles_ = ++cnt;
    return it;
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, indexer_, eraser_>::insertOrAssign(key_type article, mapped_type &&value, Node* ancestor) {
    return insert(article,value,ancestor);
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
void SimpleTrieTemplate<K, T, S, indexer_, eraser_>::erase(key_type article,Node* ancestor) {
    auto p(scout(article,ancestor));
    if (p.first) {
        erase(p.second.get(),ancestor);
    }
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
void SimpleTrieTemplate<K, T, S, indexer_, eraser_>::erase(Node* &descendant,Node* ancestor) {
    if (!empty()) {
        uint32_t curSize(size());
        checkIterPtr_helper(ancestor);

        // CHANGES ARE NOW PERMANENT -- todo NOT EXCEPTION SAFE
        eraser_(ancestor, descendant, *this);
        numberArticles_ = --curSize;
    }
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
void SimpleTrieTemplate<K, T, S, indexer_, eraser_>::swap(SimpleTrieTemplate &rhs) noexcept {
    if (this != &rhs) {
        // numberArticles_
        std::swap(numberArticles_,rhs.numberArticles_);

        // indexer_
        std::swap(indexer_,rhs.indexer_);

        // eraser_
        std::swap(eraser_,rhs.eraser_);

        // root_
        std::swap(root_,rhs.root_);
    }
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, indexer_, eraser_>::find(key_type article) {
    auto couple(scout(article));
    return (couple.first) ? couple.second : end();
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
std::pair<bool, Iterator<K,T,S>>
SimpleTrieTemplate<K, T, S, indexer_, eraser_>::scout(key_type article,Node* ancestor) {
    if (empty())
        return std::pair<bool, iterator>(false,begin());

    checkIterPtr_helper(ancestor);

    // assume scout operation doesn't edit nodes in trie
    return scout_helper(article, ancestor, indexer_);
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
bool SimpleTrieTemplate<K, T, S, indexer_, eraser_>::contains(key_type article,Node* ancestor) {
    return scout(article,ancestor).first;
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
bool SimpleTrieTemplate<K, T, S, indexer_, eraser_>::operator==(const SimpleTrieTemplate &rhs) const {
    if (this == &rhs) // check to see if same address
        return true;

    if (size() == rhs.size()) { // first check size
        // if empty, then equal, else check root_ equivalence
        return empty() ? true : *root_->child_.at(0) == *rhs.root_->child_.at(0);
    }
    return false;
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
bool SimpleTrieTemplate<K, T, S, indexer_, eraser_>::operator!=(const SimpleTrieTemplate &rhs) const {
    return !(*this == rhs);
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, indexer_, eraser_>::begin() {
    return ++iterator(*root_,-1);
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, indexer_, eraser_>::end() {
    return iterator(*root_,0);
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
std::pair<bool, Iterator<K,T,S>>
SimpleTrieTemplate<K, T, S, indexer_, eraser_>::scout_helper(key_type &key, const Node* curNode, key_indexer& indexer) {

    int32_t index(indexer(key,curNode));
    if (index > -1 && index < S) {
        if (curNode->child_.at(index).get() == nullptr)
            return std::pair<bool,iterator>(false,iterator(const_cast<Node&>(*curNode),-1));
        else
            return scout_helper(key, curNode->child_.at(index).get(), indexer);
    }
    else if (index == -1) {
        return std::pair<bool,iterator>(true,iterator(const_cast<Node&>(*curNode),-1));
    }
    else if (index == -2) {
        return std::pair<bool,iterator>(false,iterator(const_cast<Node&>(*curNode->parent_),-1));
    }
    // else an invalid index was returned
    throw std::domain_error("indexer_ returned value outside of [-1," + std::to_string(S) + "]: " + std::to_string(index) + "\n");
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, indexer_, eraser_>::insert_helper(Node* &curNode, key_type &key, mapped_type &value, key_indexer& indexer) {
    int16_t index(indexer(key,curNode));
    if (index == -1) {
        curNode->value_ = value;
        return iterator(*curNode, -1);
    }
    //todo this forces all relative modification/indexing to not allow access to parents
    else if (index > -1 && index < S) {
        auto ptr(curNode->child_.at(index).get());
        auto it(insert_helper(ptr, key, value, indexer));
        if (curNode->child_.at(index).get() == nullptr) {
            curNode->child_.at(index).reset(ptr);
            curNode->child_.at(index)->parent_ = curNode;
        }
        return it;
    }
    // else an invalid index was returned
    throw std::domain_error("indexer_ returned value outside of [-1," + std::to_string(S) + "]: " + std::to_string(index) + "\n");
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Eraser>
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, Indexer, Eraser>::insert_helper(Node *&&curNode, key_type &article, mapped_type &value, key_indexer& indexer) {
    return insert_helper(curNode, article, value, indexer);
}

template<typename K, typename T, uint32_t S, typename indexer_, typename eraser_>
void SimpleTrieTemplate<K, T, S, indexer_, eraser_>::checkIterPtr_helper(Node* &ptr) {
    if (ptr == nullptr)
        ptr = begin().get();
}

#endif // SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_CPP