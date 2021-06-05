//
// Created by Kyle Gendreau on 5/30/21.
//

#ifndef SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_CPP
#define SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_CPP

#include "../include/SimpleTrieTemplate.h"

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::SimpleTrieTemplate() : numberArticles(0), root(nullptr), indexer(), modifier(), eraser() {
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::SimpleTrieTemplate(const SimpleTrieTemplate &rhs) : numberArticles(rhs.numberArticles), indexer(rhs.indexer), modifier(rhs.modifier), eraser(rhs.eraser) {
    if (!rhs.empty()) {
        root.reset(new Node(*(rhs.root)));
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser> &SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::operator=(const SimpleTrieTemplate &rhs) {
    if (this != &rhs) {
        SimpleTrieTemplate ph(rhs);
        swap(ph);
    }
    return *this;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
bool SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::empty() const noexcept {
    return size() == 0;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
uint32_t SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::size() const noexcept {
    return numberArticles;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::clear() noexcept {
    if (!empty()) {
        root.reset();
        numberArticles = 0;
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(std::pair<key_type, mapped_type> &p) {
    return insert(p.first,p.second);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(std::pair<key_type, mapped_type> &&p) {
    return insert(p.first,p.second);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(key_type article, mapped_type &&value) {
    std::forward_list<mapped_type> val;
    val.push_front(value);

    if (!contains(article))
        ++numberArticles;

    Node* ptr(root.get());
    auto it = insert_recursive(ptr, nullptr, article, val);
    if (root.get() == nullptr)
        root.reset(ptr);

    return it;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(key_type article,
                                                                                            std::forward_list<mapped_type> &&value) {
    if (!contains(article))
        ++numberArticles;
    return insert_recursive(root.get(), nullptr, article, value);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(key_type article, mapped_type &value) {
    std::forward_list<mapped_type> val;
    val.push_front(value);
    if (!contains(article))
        ++numberArticles;

    Node* ptr(root.get());
    auto it = insert_recursive(ptr, nullptr, article, val);
    if (root.get() == nullptr)
        root.reset(ptr);

    return it;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(key_type article,
                                                                               std::forward_list<mapped_type> &value) {
    if (!contains(article))
        ++numberArticles;

    Node* ptr(root.get());
    auto it = insert_recursive(ptr, nullptr, article, value);
    if (root.get() == nullptr)
        root.reset(ptr);

    return it;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
bool SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::erase(key_type article) {
    auto iter(find(article));
    if (iter != end()) {
        erase(iter);
        return true;
    }
    return false;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::erase(SimpleTrieTemplate::iterator& pos) {
    eraser(pos);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::erase(SimpleTrieTemplate::iterator &&pos) {
    eraser(pos);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::swap(SimpleTrieTemplate &rhs) {
    if (this != &rhs) {
        // numberArticles
        uint32_t ph_int(rhs.numberArticles);
        rhs.numberArticles = numberArticles;
        numberArticles = ph_int;

        // indexer
        key_indexer ph_dex(rhs.indexer);
        rhs.indexer = indexer;
        indexer = ph_dex;

        // modifier
        key_modifier ph_mod(rhs.modifier);
        rhs.modifier = modifier;
        modifier = ph_mod;

        // eraser
        key_eraser ph_rase(rhs.eraser);
        rhs.eraser = eraser;
        eraser = ph_rase;

        root.swap(rhs.root);
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::find(key_type article) {
    auto couple = scout(article);
    return (couple.first) ? *(couple.second) : end();
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
std::pair<bool, std::unique_ptr<Iterator<K,T,S>>>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::scout(const key_type &article) {
    key_type duplicate(article);
    if (root.get() == nullptr) {
        return std::pair<bool, std::unique_ptr<iterator>>(false,nullptr);
    }
    return scout_helper(duplicate, root.get());
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
bool SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::contains(key_type article) {
    return scout(article).first;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
bool SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::operator==(const SimpleTrieTemplate &rhs) const {
    if (this == &rhs)
        return true;

    bool first = numberArticles == rhs.numberArticles;
    if (root.get() == nullptr) {
        return rhs.root.get() == nullptr;
    }
    else {
        if (rhs.root.get() == nullptr)
            return false;
        bool sec = *root == *(rhs.root);
        return first && sec;
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
bool SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::operator!=(const SimpleTrieTemplate &rhs) const {
    return !(*this == rhs);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::begin() {
    return iterator(*root,-1);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::end() {
    return iterator(*root,S-1);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
std::pair<bool, std::unique_ptr<Iterator<K,T,S>>>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::scout_helper(key_type &key,
                                                                      const Node *node) {
    // call indexer and get signal
    int32_t signal(indexer(key,node));

    if (signal == -1) { // check if you found the node!!
        return scout_helper_foundTarget(key,const_cast<Node*>(node));
    }
    else if (signal > -1) { // check if there's a next node in sequence
        return scout_helper_childAccess(key,const_cast<Node*>(node),signal);
    }
    else { // the signal shows we need to return previous node in sequence
        if (node->parent == nullptr) // check if previous node is nullptr
            return std::pair<bool, std::unique_ptr<iterator>>(false,nullptr);

        // else return iterator at previous node in sequence
        return std::pair<bool, std::unique_ptr<iterator>>(false,std::unique_ptr<iterator>(new iterator(const_cast<Node*>(node),-1)));
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
std::pair<bool, std::unique_ptr<Iterator<K,T,S>>>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::scout_helper_foundTarget(key_type &key,
                                                                                         Node *node) {
    // call modifier everytime
    modifier(key,node,node->parent,-1);
    return std::pair<bool,std::unique_ptr<iterator>>(true,std::unique_ptr<iterator>(new iterator(node,-1)));
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
std::pair<bool, std::unique_ptr<Iterator<K,T,S>>>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::scout_helper_childAccess(key_type &key, Node *node,
                                                                                         uint32_t signal) {
    // basic validation checking
    valid_childAccess_helper(*node,signal);

    modifier(key,node,node->parent,signal);

    // if next node is nullptr then target can't be found and this is last node in sequence
    if (node->child.at(signal) == nullptr) {
        // return with an it that's prevIndex points to the last node tried, could be helpful for gather more info
        return std::pair<bool, std::unique_ptr<iterator>>(false, std::unique_ptr<iterator>(new iterator(node, signal)));
    }

    // else modify key/node and recurse
    return scout_helper(key,node->child.at(signal).get());
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert_recursive(Node* &curNode, Node* parentNode, key_type& key,
                                                                         std::forward_list<mapped_type>& value) {

    // call indexer and get signal
    int32_t signal(indexer(key,curNode));

    if (signal == -1) { // then we have found the last node in sequence for the article

        // call modifier everytime
        modifier(key,curNode,parentNode,signal);

        // insert values
        curNode->value.merge(value);

        return Iterator<K,T,S>(*curNode,int32_t(signal - 1));
    }
    else if (signal > -1) {
        // basic validation checking
        valid_childAccess_helper(*curNode,signal);

        // call modifier everytime
        modifier(key,curNode,parentNode,signal);

        // recursive call
        Node* ptr(curNode->child.at(signal).get());
        auto it = insert_recursive(ptr,curNode,key,value);
        if (curNode->child.at(signal).get() == nullptr)
            curNode->child.at(signal).reset(ptr);
        ptr->parent = curNode;
        return it;
    }
    else {
        modifier(key,curNode,parentNode,signal); // should fix error
        return insert_recursive(curNode,parentNode,key,value); // call the exact same value again
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::valid_childAccess_helper(const Node &node, const uint32_t& signal) {
    // do some basic checks
    if (signal >= S) { // check if within range
        throw std::out_of_range("key_indexer signaled for child access greater than S\n");
    }
}

#endif // SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_CPP