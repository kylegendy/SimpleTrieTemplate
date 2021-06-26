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
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::SimpleTrieTemplate(const SimpleTrieTemplate &&rhs) : numberArticles(rhs.numberArticles), indexer(rhs.indexer), modifier(rhs.modifier), eraser(rhs.eraser) {
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
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser> &SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::operator=(const SimpleTrieTemplate &&rhs) {
    return *this = rhs;
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
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(std::pair<key_type, mapped_type> &p, iterator* ancestor) {
    std::forward_list<mapped_type> val;
    val.push_front(p.second);
    return insert_helper(ancestor,p.first,val);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(std::pair<key_type, mapped_type> &&p, iterator* ancestor) {
    return insert(p,ancestor);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(std::pair<key_type, std::forward_list<mapped_type>> &p, iterator* ancestor) {
    return insert_helper(ancestor,p.first,p.second);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(std::pair<key_type, std::forward_list<mapped_type>> &&p, iterator* ancestor) {
    return insert(p,ancestor);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(key_type article, mapped_type &value, iterator* ancestor) {
    std::forward_list<mapped_type> val;
    val.push_front(value);
    return insert_helper(ancestor,article,val);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(key_type article, mapped_type &&value, iterator* ancestor) {
    return insert(article,value,ancestor);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(key_type article,
                                                                               std::forward_list<mapped_type> &value, iterator* ancestor) {
    return insert_helper(ancestor,article,value);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S> SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert(key_type article,
                                                                                            std::forward_list<mapped_type> &&value, iterator* ancestor) {
    return insert(article,value,ancestor);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::erase(key_type article,iterator* ancestor) {
    auto p(scout(article,ancestor));
    if (p.first) {
        erase(*(p.second),ancestor);
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::erase(iterator &descendant,iterator* ancestor) {
    uint32_t curSize(size());
    if (ancestor == nullptr) {
        iterator it(begin());
        ancestor = &it;
    }
    eraser(*ancestor,descendant,*this);
    numberArticles = --curSize;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::erase(iterator &&descendant,iterator* ancestor) {
    erase(descendant,ancestor);
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::swap(SimpleTrieTemplate &rhs) {
    if (this != &rhs) {
        // numberArticles
        std::swap(numberArticles,rhs.numberArticles);

        // indexer
        std::swap(indexer,rhs.indexer);

        // modifier
        std::swap(modifier,rhs.modifier);

        // eraser
        std::swap(eraser,rhs.eraser);

        // root
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
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::scout(key_type article,iterator* ancestor) {
    if (ancestor == nullptr) {
        iterator it(begin());
        ancestor = &it;
    }
    if (*ancestor == end()) {
        return std::pair<bool, std::unique_ptr<iterator>>(false,nullptr);
    }
    return scout_helper(article, ancestor->get());
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
bool SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::contains(key_type article,iterator* ancestor) {
    return scout(article,ancestor).first;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
bool SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::operator==(const SimpleTrieTemplate &rhs) const {
    if (this == &rhs) // check to see if same address
        return true;

    if (size() == rhs.size()) { // first check size
        // if root holds nullptr, then rhs.root must hold nullptr to be equal
        if (root.get() == nullptr) {
            return rhs.root.get() == nullptr;
        }
        // else root does not hold nullptr, and rhs.root must hold equivalent node as root to be equivalent
        else if (rhs.root.get() != nullptr) {
            return *root == *(rhs.root);
        }
    }
    return false;
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
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert_helper(iterator* ancestor, key_type article, std::forward_list<mapped_type> &value) {
    if (ancestor == nullptr) {
        iterator it(begin());
        ancestor = &it;
    }

    if (!contains(article,ancestor))
        ++numberArticles;

    bool isBegin(*ancestor == begin());
    Node* ptr = (isBegin ? root.get() : ancestor->get());
    auto it = insert_recursive(ptr, K(article), value);

    if (isBegin) {
        if (root.get() == nullptr)
            root.reset(ptr);
    }

    return it;
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
std::pair<bool, std::unique_ptr<Iterator<K,T,S>>>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::scout_helper(key_type key,
                                                                     Node* node) {
    // call indexer and get signal
    int32_t signal(indexer(key,node));

    if (signal == -1) { // check if you found the node!!
        modifier(key,node,-1); // call modifier
        return std::pair<bool,std::unique_ptr<iterator>>(true,std::unique_ptr<iterator>(new iterator(node,-1)));
    }
    else if (signal > -1) { // check if there's a next node in sequence
        valid_childAccess_helper(signal);
        return scout_helper_childAccess(key,node,signal);
    }
    else { // the signal shows we need to return previous node in sequence
        return std::pair<bool,std::unique_ptr<iterator>>(false,std::unique_ptr<iterator>(new iterator(node->parent,-1)));
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
std::pair<bool, std::unique_ptr<Iterator<K,T,S>>>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::scout_helper_childAccess(key_type &key, Node* &node,
                                                                                 int32_t &signal) {

    // if next node is nullptr then target can't be found and this is last node in sequence
    if (node->child.at(signal) == nullptr) {
        // return with an it that's prevIndex points to the last node tried, could be helpful for gather more info
        return std::pair<bool, std::unique_ptr<iterator>>(false, std::unique_ptr<iterator>(new iterator(node, signal)));
    }

    // else modify key/node and recurse
    return scout_helper(modifier(key,node,signal),node->child.at(signal).get());
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
Iterator<K,T,S>
SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::insert_recursive(Node* &curNode, key_type&& key,
                                                                         std::forward_list<mapped_type>& value) {

    // call indexer and get signal
    int32_t signal(indexer(key,curNode));

    if (signal == -1) { // then we have found the last node in sequence for the article

        // call modifier everytime
        modifier(key,curNode,signal);

        // insert values
        handleValueMerge(curNode->value, value);

        return iterator(curNode,signal);
    }
    else if (signal > -1) {
        // basic validation checking
        valid_childAccess_helper(signal);

        // make a copy of a pointer in case it points to a nullptr
        Node* ptr(curNode->child.at(signal).get());

        // recursive call
        auto it = insert_recursive(ptr,modifier(key,curNode,signal),value);

        // if it was equal to nullptr, can now reset
        if (curNode->child.at(signal) == nullptr)
            curNode->child.at(signal).reset(ptr);

        // assign parent
        ptr->parent = curNode;
        return it;
    }
    else {
        return insert_recursive(curNode,modifier(key,curNode,signal),value); // call the exact same value again
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::valid_childAccess_helper(const uint32_t& signal) {
    // do some basic checks
    if (signal >= S) { // check if within range
        throw std::out_of_range("key_indexer signaled for child access greater than S\n");
    }
}

template<typename K, typename T, uint32_t S, typename Indexer, typename Modifier, typename Eraser>
void SimpleTrieTemplate<K, T, S, Indexer, Modifier, Eraser>::handleValueMerge(std::forward_list<mapped_type> &nodeList,
                                                                              std::forward_list<mapped_type> &inputValue) {
    auto inputEnd(inputValue.end()), nodeEnd(nodeList.end());
    bool found(false);
    for (auto inputIt(inputValue.begin()); inputIt != inputEnd; ++inputIt) { // iterate through inputList
        for (auto nodeIt(nodeList.begin()); nodeIt != nodeEnd; ++nodeIt) { // iterator through nodeList
            if (*inputIt == *nodeIt) {
                found = true;
                *nodeIt = *inputIt;
                break;
            }
        }
        if (!found) {
            nodeList.push_front(*inputIt);
            nodeEnd = nodeList.end();
        }
        found = false;
    }
}

#endif // SIMPLETRIETEMPLATE_SIMPLETRIETEMPLATE_CPP