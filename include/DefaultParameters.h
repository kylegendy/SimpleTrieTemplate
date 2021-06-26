//
// Created by Kyle Gendreau on 6/20/21.
//

#ifndef SIMPLETRIETEMPLATE_DEFAULTPARAMETERS_H
#define SIMPLETRIETEMPLATE_DEFAULTPARAMETERS_H

#include "SimpleTrieTemplate.h"

// assume all upper-case letters
class RadixIndexer {
public:
    int32_t operator()(const std::string& input, const Node<std::string,bool,26>* node) {
        // if node points to nullptr, then a new node must be created
        if (node == nullptr)
            return -2;

        // cannot insert empty string
        if (input.length() == 0)
            throw std::logic_error("\ninserting an empty string\n");

        // find where strings deviate
        int16_t dif(findDeviation(input,node->key));

        if (dif == input.length())
            return (dif == node->key.length()) ? -1 : -2; // checks for equivalence
        else if (dif == node->key.length())
            return findChildIndex(input.at(dif)); // move on to next node
        else if (dif == 0) {
            if (node->key.length() == 0) {
                // if this is the root, then move on to next node
                if (node->parent == nullptr)
                    return findChildIndex(input.at(dif));

                // this should never happen otherwise!!!
                throw std::logic_error("\na non-root node holds empty string");
            }
            // else it is most likely a trie that holds a letter as the starting node, and must be modified
            return -3;
        }

        // else dif is somewhere in the middle of BOTH strings, and thus the node's key and children must be modded
        return -2;
    }

    // returns the index at deviation
    static int16_t findDeviation(const std::string& input, const std::string& nodeString) {
        for (uint32_t i(0); i < input.length(); ++i) {
            // if input length longer than nodeString length or strings not equal to eachother at i
            if (i >= nodeString.length() || input.at(i) != nodeString.at(i))
                return i;
        }
        return input.length();
    }

    static int16_t findChildIndex(const char16_t& letter) {
        return letter - 'A';
    }
};

class RadixModifier {
public:
    std::string operator()(std::string& input, Node<std::string,bool,26>* &curNode, const int32_t &signal) {
        if (signal > -1) {
            // else it signals a child location, and thus part of input string fully overlaps node string
            int16_t dif(RadixIndexer::findDeviation(input,curNode->key));
            return input.substr(dif);
        }
        else if (signal == -2) {
            // treat for nullptr
            if (curNode == nullptr) {
                curNode = new Node<std::string,bool,26>(nullptr,input);
                return input;
            }
            else {
                // A) input string partially overlaps node
                // B) part of input string partially overlaps node
                int16_t dif(RadixIndexer::findDeviation(input,curNode->key));

                Node<std::string,bool,26>* duplicate(new Node<std::string,bool,26>(*curNode));
                duplicate->key = duplicate->key.substr(dif);
                curNode->key = curNode->key.substr(0,dif);
                curNode->value.clear();
                for (uint32_t i(0); i<26; ++i) {
                    curNode->child.at(i).reset();
                }
                int16_t childIndex(RadixIndexer::findChildIndex(duplicate->key.at(0)));
                curNode->child[childIndex].reset(duplicate);
                duplicate->parent = curNode;

                return input;
            }
        }
        else if ( signal == -1) {
            return "";
        }
        else {
            handleThree(curNode);
            return input;
        }
    }

    static void handleThree(Node<std::string,bool,26>* &curNode) {
        assert(curNode->parent == nullptr);

        // create new root node with empty string
        Node<std::string,bool,26>* prevNode(new Node<std::string,bool,26>(*curNode));
        curNode->key = "";
        curNode->value.clear();
        for (uint32_t i(0); i<26; ++i) {
            curNode->child.at(i).reset();
        }
        curNode->child.at(RadixIndexer::findChildIndex(prevNode->key.at(0))).reset(prevNode);
        prevNode->parent = curNode;
    }
};

class RadixEraser {
public:
    void operator()(Iterator<std::string,bool,26> &ancestor, Iterator<std::string,bool,26> &descendant, SimpleTrieTemplate<std::string,bool,26,RadixIndexer,RadixModifier,RadixEraser> &trie) {
        if (ancestor->child.empty() || descendant == ancestor) {
            return;
        }
        else {
            descendant->value.clear();
            uint16_t numChild(getNumChild(descendant));
            // if child empty
            if (numChild == 0) {
                handleAncestors(descendant.moveUp(), trie);
            }
            // if only one child
            else if (numChild == 1) {
                handleCompact(descendant);
                handleCompact(descendant.moveUp());
            }
            // if multiple children
            else {
                handleCompact(descendant.moveUp());
            }
        }
    }

    static uint16_t getNumChild(Iterator<std::string,bool,26> &descendant) {
        uint16_t num(0);
        for (uint16_t i(0); i < 26; ++i) {
            if (descendant->child.at(i).get() != nullptr)
                ++num;
        }
        return num;
    }

    static void handleAncestors(Iterator<std::string,bool,26> &it,SimpleTrieTemplate<std::string,bool,26,RadixIndexer,RadixModifier,RadixEraser> &trie) { // todo this shit is fucked and breaks on erase tests
        if (it->parent == nullptr) {
            trie.clear();
        }
        else {
            it.moveUp();
            it->child.at(it.getIndex()).reset();
            if (it->value.empty()) {
                if (getNumChild(it) == 0) {
                    handleAncestors(it,trie);
                }
            }
        }
    }

    static void handleCompact(Iterator<std::string,bool,26> &ancestor) {
        int16_t index(-1);
        for (uint16_t i(0); i < 26; ++i) {
            if (ancestor->child.at(i).get() != nullptr) {
                if (index != -1)
                    return;
                else
                    index = i;
            }
        }
        std::string prevStr(ancestor->key);
        Node<std::string,bool,26>* prevParent(ancestor->parent);
        *ancestor = *(ancestor->child.at(index).get());
        ancestor->key = prevStr + ancestor->key;
        ancestor->parent = prevParent;
    }
};

#endif //SIMPLETRIETEMPLATE_DEFAULTPARAMETERS_H
