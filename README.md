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
Node<typename K, typename T, uint32_t S> : the node structure used by SimpleTrieTemplate

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
explicit Node(Node* parentPtr = nullptr, K key = K(), std::forward_list<T> value = std::forward_list<T>()); | default constructor with defaulted parameters
explicit Node(const Node& rhs); | copy constructor, copies all descendants, but not ancestor nodes
void swap(Node &rhs); | swap function, swaps all values of *this and rhs
Node& operator=(const Node& rhs); | assignment operator, calls swap on a copy constructor with rhs as a parameter
bool operator==(const Node& rhs); | checks for logical equivalence by comparing all member variables except for parent
bool operator!=(const Node& rhs); | returns the opposite of operator==()



#### Iterator Class Template
##### Description:

##### Member Variables:

##### Member Functions:



#### SimpleTrieTemplate Class Template
##### Description:

##### Member Variables:

##### Member Functions: