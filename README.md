# SimpleTrieTemplate
## Table of Contents

1. [Objective](#objective)
2. [Outline](#outline)
    1. [General](#general)
    2. [Object Details](#object-details)
        1. [Node Class Template](#node-class-template)
        2. [Iterator Class Template](#iterator-class-template)
        4. [SimpleTrieTemplate Class Template](#simpletrietemplate-class-template)

## Objective
Create a trie that templatizes the key_types, value_types, number of children, and abstracts its mapping/erasing functions.

## Outline

### General
This structure finds inserted values, ie T variables, or value_types, by searching through the trie with respect to a key, ie K variables, or key_types.  The methodology of traversing through the trie is orchestrated by the indexer and modifier classes.
The indexer and modifier is part of the template parameters, the indexer returns an index for the next child to visit relative to the current node.  The modifier modifies the key and node given the index returned from the indexer.

### Object Details

#### Node Class Template
##### Description:
Node\<typename K, typename T, uint32_t S\> : the node structure used by SimpleTrieTemplate

##### Member Variables:
variable | description
---------|---------
Node* parent; | a raw pointer to the parent Node of *this
K key; | the key held at *this
std::forward_list\<T\> value; | a list of mapped value types, T
std::vector\<std::unique_ptr\<Node\>\> child; | a vector of Node type smart pointers that holds S number of children

##### Member Functions:
signature | description
----------|-----------
explicit Node(Node* parentPtr = nullptr, K key = K(), std::forward_list\<T\> value = std::forward_list\<T\>()); | default constructor with defaulted parameters
explicit Node(const Node& rhs); | copy constructor, copies all descendants, but not ancestor nodes
void swap(Node &rhs); | swap function, swaps all values of *this and rhs
Node& operator=(const Node& rhs); | assignment operator, calls swap on a copy constructor with rhs as a parameter
bool operator==(const Node& rhs); | checks for logical equivalence by comparing all member variables except for parent
bool operator!=(const Node& rhs); | returns the opposite of operator==()



#### Iterator Class Template
##### Description:
Iterator\<typename K, typename T, uin32_t S\> : a forwards iterator used to iterate through the nodes of the trie

##### Member Variables:
Nothing public, but describing the private variables might prove useful.
First, a node pointer to the node currently at.  Second, an integer value valled prevIndex, which serves the purpose of navigating through nodes with varying S values.  prevIndex tells where the iterator comes from, if its -1 its comes from the parent, if greater then it comes from a child at the index of prevIndex.

##### Member Functions:
signature | description
----------|-----------
Iterator(Node\<K,T,S\>* currentNode = nullptr, int32_t previousIndex = -1); | default constructor, paramaters hold default values
Iterator(Node\<K,T,S\> &currentNode, int32_t previousIndex); | alternative constructor, the same as the default except that it takes a node by reference instead of address
Iterator(const Iterator &rhs); | copy constructor
Node\<K,T,S\> &operator*(); | dereference operator, returns the node pointed to by the iterator
Node\<K,T,S\>* operator->(); | accessor operator, returns the address of the node pointed to by the iterator
K &first(); | returns the key held by the node
std::forward_list\<T\> &second(); | returns the list of values held by the node
bool isArticleEnd(); | checks if the node is the end of an article, ie if its list is not empty
static int32_t findChildsIndex(const Node\<K,T,S\> &parent, const Node\<K,T,S\> &child); | returns the index of child within the children of parent, else -1 if not found
void swap(Iterator& rhs); | swaps the values of *this and rhs
Iterator &operator=(Iterator &rhs); | assignment operator by reference, makes logically equivalent from itself onwards through the descendents, but does not assign parents
Iterator &operator=(Iterator &&rhs); | assignment operator by move, does the exact same as the assignment operator by reference
Iterator &moveUp(); | moves iterator up to its parent
Iterator &operator++(); | prefix incrementor, increments *this forwards onto the next node in order, and then returns *this
Iterator operator++(int); | postfix incrementor, increments *this forwards onto the next node in order, and returns a copy of *this prior to incrementing
bool operator==(const Iterator &rhs) const; | compares *this to rhs by logical equivalence by checking the nodes logical equivalence and then the prevIndexes: true if equivalent, false otherwise
bool operator!=(const Iterator &rhs) const; | returns the opposite of operator==()



#### SimpleTrieTemplate Class Template
##### Description:

##### Member Variables:

##### Member Functions: