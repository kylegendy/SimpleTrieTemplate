//
// Created by Kyle Gendreau on 5/30/21.
//

#include <iostream>
#include "./include/SimpleTrieTemplate.h"

void run_tests();

void print_message(const std::string& s);

//region //FUNCTION DECLARATION
void constructor_default_tests();
    std::string constructor_default_empty();
    std::string constructor_default_beginIsEnd();
    std::string constructor_default_equalInstants();

void constructor_copy_tests();
    std::string constructor_copy_equal(SimpleTrieTemplate<int,char,2>& og, uint32_t count);
    std::string constructor_copy_diffOGs();
    std::string constructor_copy_notByRef();

void assignment_tests();
    std::string assign_diffOnesAndDiffOthers();
    std::string assign_oneEqualOther(SimpleTrieTemplate<int,char,2>& one, SimpleTrieTemplate<int,char,2>& other, int32_t cnt);
    std::string assign_notByRef();

void empty_tests();
    std::string empty_checksValid_cases();
    std::string empty_checksValid(SimpleTrieTemplate<int,char> &trie, bool isEmpty,int32_t cnt);

void size_tests();
    std::string size_checksValid_cases();
    std::string size_checkValid(SimpleTrieTemplate<int,char> &trie, uint32_t correctCnt, uint32_t cnt);

void clear_tests();
    std::string clear_diffCases();
    std::string clear_properOut(SimpleTrieTemplate<int,char> &trie, uint32_t cnt);
    std::string clear_multiple();
    std::string clear_empty_empty();
    std::string clear_filled_empty();
    std::string clear_empty_filled();
    std::string clear_filled_filled();

void insert_tests();

void erase_tests();

void swap_tests();
    std::string swap_oneAndOther(SimpleTrieTemplate<int,char> &one, SimpleTrieTemplate<int,char> &other, int32_t cnt);

void find_tests();
    std::string find_keyInTrie(SimpleTrieTemplate<int,char> &trie, int key, bool valid, int32_t cnt);

void scout_tests();
    std::string scout_keyInTrie(SimpleTrieTemplate<int,char> &trie, int key, bool valid, int32_t cnt);

void contains_tests();
    std::string contains_keyInTrie(SimpleTrieTemplate<int,char> &trie, int key, bool valid, int32_t cnt);

void iterator_tests();
//endregion

int main() {
    run_tests();
    return 0;
}

//region //FUNCTION DEFINITIONS
void run_tests() {
    constructor_default_tests();
    constructor_copy_tests();
    assignment_tests();
    empty_tests();
    size_tests();
    clear_tests();
//    insert_tests();
//    erase_tests();
    swap_tests();
    find_tests();
    scout_tests();
    contains_tests();
//    iterator_tests();
}

void print_message(const std::string& s) {
    if (s.empty()) {
        std::cout << "PASS" << std::endl;
    }
    else {
        std::cout << "FAIL" << std::endl << s;
    }
}

void constructor_default_tests() {
    std::cout << "default constructor test(s): ";
    std::string s;

    s += constructor_default_empty();
    s += constructor_default_beginIsEnd();
    s += constructor_default_equalInstants();

    print_message(s);
}
//region //DEF CONSTRUCTOR
std::string constructor_default_empty() {
    std::string out("\t- is empty: ");
    try {
        SimpleTrieTemplate<int,char,2> trie;
        return (!trie.empty()) ? out + "fail\n" : "";
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

std::string constructor_default_beginIsEnd() {
    std::string out("\t- begin()==end(): ");
    try {
        SimpleTrieTemplate<int,char,2> trie;
        return (trie.begin() != trie.end()) ? out + "fail\n" : "";
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

std::string constructor_default_equalInstants() {
    std::string out("\t- def()==def(): ");
    try {
        SimpleTrieTemplate<int,char,2> firstTrie;
        SimpleTrieTemplate<int,char,2> secondTrie;
        out = (firstTrie != secondTrie) ? out + "fail -- != " : "";
        out = (firstTrie == secondTrie) ? "" : out + "fail -- == ";
        return (out.empty()) ? out : out + "\n";
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

//endregion

void constructor_copy_tests() {
    std::cout << "copy constructor test(s): ";
    std::string s;

    s += constructor_copy_diffOGs();
    s += constructor_copy_notByRef();

    print_message(s);
}
//region //COPY CONSTRUCTOR
std::string constructor_copy_equal(SimpleTrieTemplate<int,char,2>& og, uint32_t count) {
    std::string out("\t- is equal(" + std::to_string(count) + "): ");
    try {
        SimpleTrieTemplate<int,char,2> trie(og);
        return (trie == og) ? "" : out + "fail\n";
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

std::string constructor_copy_diffOGs() {
    SimpleTrieTemplate<int,char,2> tr_one; // empty
    SimpleTrieTemplate<int,char,2> tr_two; // one item
    SimpleTrieTemplate<int,char,2> tr_three; // three items

    std::forward_list<char> lst;
    lst.push_front('d');
    lst.push_front('e');
    lst.push_front('f');

    char val('z');

    tr_two.insert(3,'c');
    tr_three.insert(3,'c');
    tr_three.insert(5,lst);
    tr_three.insert(8,val);

    std::string out;

    out += constructor_copy_equal(tr_one, 1);
    out += constructor_copy_equal(tr_two, 2);
    out += constructor_copy_equal(tr_three, 3);

    return out;
}

std::string constructor_copy_notByRef() {
    std::string out("\t- copy by value: ");
    try {
        SimpleTrieTemplate<int,char,2> trie_one;
        SimpleTrieTemplate<int,char,2> trie_two(trie_one);
        trie_one.insert(1,'a');
        trie_one.insert(2,'b');
        trie_one.insert(3,'d');

        SimpleTrieTemplate<int,char,2> trie_three;
        trie_three.insert(1,'a');
        trie_three.insert(2,'b');
        trie_three.insert(3,'d');
        SimpleTrieTemplate<int,char,2> trie_four(trie_one);

        std::string moreOut;

        moreOut += ((trie_one != trie_two) ? "" : "fail -- inserted after copy; ");
        moreOut +=((trie_three == trie_four) ? "" : "fail -- inserted before copy");

        return (moreOut.empty() ? "" : out + moreOut + "\n");
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

//endregion

void assignment_tests() {
    std::cout << "assignment operator test(s): ";
    std::string s;

    s += assign_diffOnesAndDiffOthers();
    s += assign_notByRef();

    print_message(s);
}
//region //ASSIGNMENT
std::string assign_diffOnesAndDiffOthers() {
    SimpleTrieTemplate<int,char> trie_one, trie_two, trie_three, trie_four,
    trie_five, trie_six, trie_seven, trie_eight, trie_nine, trie_ten;

    std::string out;

    // empty to empty
    out += assign_oneEqualOther(trie_one,trie_two,1);

    // filled to empty
    trie_one.insert(3,'a');
    trie_one.insert(2,'b');
    trie_one.insert(5,'c');
    out += assign_oneEqualOther(trie_one,trie_two,2);

    // empty to filled
    trie_three.insert(3,'a');
    trie_three.insert(2,'b');
    trie_three.insert(5,'c');
    out += assign_oneEqualOther(trie_four,trie_three,3);

    // filled to filled (same size)
    trie_six.insert(3,'a');
    trie_six.insert(2,'b');
    trie_six.insert(5,'c');
    trie_five.insert(6,'a');
    trie_five.insert(3,'b');
    trie_five.insert(7,'c');
    out += assign_oneEqualOther(trie_six,trie_five,4);

    // filled to filled (less to large -- ie 2 to 3)
    trie_seven.insert(3,'a');
    trie_seven.insert(2,'b');
    trie_eight.insert(6,'a');
    trie_eight.insert(3,'b');
    trie_eight.insert(7,'c');
    out += assign_oneEqualOther(trie_seven,trie_eight,5);

    // filled to filled (large to less -- ie 3 to 2)
    trie_nine.insert(3,'a');
    trie_nine.insert(2,'b');
    trie_ten.insert(6,'a');
    trie_ten.insert(3,'b');
    trie_ten.insert(7,'c');
    out += assign_oneEqualOther(trie_ten,trie_nine,6);

    return out;
}

std::string assign_oneEqualOther(SimpleTrieTemplate<int,char,2>& one, SimpleTrieTemplate<int,char>& other, int32_t cnt) {
    std::string out("\t- one assigned to other(" + std::to_string(cnt) + "): ");
    try {
        one = other;
        return (one == other) ? "" : out + "fail\n";
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

std::string assign_notByRef() {
    std::string out("\t- copied by value: ");
    try {
        SimpleTrieTemplate<int,char,2> trie_one;
        SimpleTrieTemplate<int,char,2> trie_two;
        trie_two = trie_one;
        trie_one.insert(1,'a');
        trie_one.insert(2,'b');
        trie_one.insert(3,'d');

        SimpleTrieTemplate<int,char,2> trie_three;
        SimpleTrieTemplate<int,char,2> trie_four;
        trie_three.insert(1,'a');
        trie_three.insert(2,'b');
        trie_three.insert(3,'d');
        trie_four = trie_three;

        std::string moreOut;

        moreOut += ((trie_one != trie_two) ? "" : "fail -- inserted after assignment; ");
        moreOut +=((trie_three == trie_four) ? "" : "fail -- inserted before assignment");

        return (moreOut.empty() ? "" : out + moreOut + "\n");
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

//endregion

void empty_tests() {
    std::cout << "empty test(s): ";
    std::string s;

    s += empty_checksValid_cases();

    print_message(s);
}
//region //EMPTY
std::string empty_checksValid_cases() {
    SimpleTrieTemplate<int,char> trie_one, trie_two;
    std::string out;

    // proper return when actually empty
    out += empty_checksValid(trie_one,true,1);

    // proper return when not empty (contains one item)
    trie_two.insert(3,'a');
    out += empty_checksValid(trie_two,false,2);

    // proper return when not empty (contains two item)
    trie_two.insert(4,'b');
    out += empty_checksValid(trie_two,false,2);

    return out;
}

std::string empty_checksValid(SimpleTrieTemplate<int,char> &trie, bool isEmpty, int32_t cnt) {
    std::string out("\t- proper return(" + std::to_string(cnt) + "): ");
    try {
        return (trie.empty() == isEmpty) ? "" : out + "fail\n";
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

//endregion

void size_tests() {
    std::cout << "size test(s): ";
    std::string s;

    s += size_checksValid_cases();

    print_message(s);
}
//region //SIZE
std::string size_checksValid_cases() {
    SimpleTrieTemplate<int,char> trie_one, trie_two;
    std::string out;

    SimpleTrieTemplate<int,char> trie;

    int32_t input;
    for (uint32_t i(0); i < 10; ++i) {
        size_checkValid(trie,i,i+1);
        input = (i % 2 == 0) ? 5 - i : 5 + i;
        trie.insert(input,char('a' + i));
    }

    return out;
}

std::string size_checkValid(SimpleTrieTemplate<int,char> &trie, uint32_t correctCnt, uint32_t cnt) {
    std::string out("\t- proper return(" + std::to_string(cnt) + "): ");
    try {
        return (trie.size() == correctCnt) ? "" : out + "fail\n";
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

//endregion

void clear_tests() {
    std::cout << "clear test(s): ";
    std::string s;

    // when empty and not empty
    s += clear_diffCases();

    // called multiple times
    s += clear_multiple();

    print_message(s);
}
//region //EMPTY
std::string  clear_diffCases() {
    SimpleTrieTemplate<int,char> trie_one, trie_two, trie_three;
    std::string out;

    // when empty
    out += clear_properOut(trie_one, 1);

    // filled with one
    trie_two.insert(3,'a');
    out += clear_properOut(trie_two, 2);

    // filled with more than one
    trie_three.insert(3,'a');
    trie_three.insert(1,'b');
    trie_three.insert(8,'c');
    trie_three.insert(8,'u');
    trie_three.insert(8,'n');
    trie_three.insert(8,'t');
    out += clear_properOut(trie_three,3);

    return out;
}

std::string clear_properOut(SimpleTrieTemplate<int,char> &trie, uint32_t cnt) {
    std::string out("\t- proper return(" + std::to_string(cnt) + "): ");
    try {
        trie.clear();
        return (trie.empty() && trie.size() == 0) ? "" : out + "fail\n";
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

std::string clear_multiple() {

    std::string out;

    out += clear_empty_empty();
    out += clear_filled_empty();
    out += clear_empty_filled();
    out += clear_filled_filled();

    return out;
}

std::string clear_empty_empty() {
    SimpleTrieTemplate<int,char> trie_empty;
    std::string out;

    out += clear_properOut(trie_empty, 4);
    out += clear_properOut(trie_empty, 5);

    return out;
}

std::string clear_filled_empty() {
    SimpleTrieTemplate<int,char> trie;
    std::string out;

    trie.insert(3,'a');

    out += clear_properOut(trie, 6);
    out += clear_properOut(trie,7);

    return out;
}

std::string clear_empty_filled() {
    SimpleTrieTemplate<int,char> trie;
    std::string out;

    out += clear_properOut(trie,8);
    trie.insert(3,'a');
    out += clear_properOut(trie,9);

    return out;
}

std::string clear_filled_filled() {
    SimpleTrieTemplate<int,char> trie;
    std::string out;

    trie.insert(3,'a');
    out += clear_properOut(trie,10);
    trie.insert(4,'b');
    out += clear_properOut(trie,11);

    return out;
}

//endregion

void insert_tests() {
    std::cout << "insert test(s): ";
    std::string s;

    // pair&

    // pair&&

    // mapped_type&

    // mapped_type&&

    // forward_list&

    // forward_list&&

    print_message(s);
}

void erase_tests() {
    std::cout << "erase test(s): ";
    std::string s;

    // called on empty trie
        // changes number appropriately

    // non-existent key
        // changes number appropriately

    // actual key

    print_message(s);
}

void swap_tests() {
    std::cout << "swap test(s): ";
    std::string s;

    // empty to empty
    SimpleTrieTemplate<int,char> trie_one, trie_two;
    s += swap_oneAndOther(trie_one,trie_two,1);

    // empty to filled
    SimpleTrieTemplate<int,char> trie_three, trie_four;
    trie_four.insert(3,'a');
    trie_four.insert(2,'b');
    trie_four.insert(5,'c');
    s += swap_oneAndOther(trie_three,trie_four,2);

    // filled to empty
    SimpleTrieTemplate<int,char> trie_five, trie_six;
    trie_five.insert(3,'a');
    trie_five.insert(2,'b');
    trie_five.insert(5,'c');
    s += swap_oneAndOther(trie_five,trie_six,3);

    // filled to filled
    SimpleTrieTemplate<int,char> trie_seven, trie_eight;
    trie_seven.insert(3,'a');
    trie_seven.insert(2,'b');
    trie_seven.insert(5,'c');
    trie_eight.insert(3,'a');
    trie_eight.insert(2,'b');
    trie_eight.insert(5,'c');
    trie_eight.insert(6,'d');
    s += swap_oneAndOther(trie_seven,trie_eight,4);

    print_message(s);
}
//region //SWAP
std::string swap_oneAndOther(SimpleTrieTemplate<int,char> &one, SimpleTrieTemplate<int,char> &other, int32_t cnt) {
    std::string out("\t- proper swap(" + std::to_string(cnt) + "): ");
    try {
        SimpleTrieTemplate<int,char> copy_one(one), copy_other(other);
        one.swap(other);
        return (one == copy_other && other == copy_one) ? "" : out + "fail\n";
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

//endregion

void find_tests() {
    std::cout << "find test(s): ";
    std::string s;

    // empty trie
    SimpleTrieTemplate<int,char> empty;
    s += find_keyInTrie(empty,3,false,1);

    // filled trie, invalid key
    SimpleTrieTemplate<int,char> filled;
    filled.insert(3,'a');
    filled.insert(2,'b');
    filled.insert(5,'c');
    s += find_keyInTrie(filled,7,false,2);

    // filled trie, valid key
    s += find_keyInTrie(filled,5,true,2);

    print_message(s);
}
//region //FIND
std::string find_keyInTrie(SimpleTrieTemplate<int,char> &trie, int key, bool valid, int32_t cnt) {
    std::string out("\t- proper find(" + std::to_string(cnt) + "): ");
    try {
        auto it = trie.find(key);
        bool found(true);
        if (it != trie.end()) {
            found = it.first() == key;
        }
        return ((it != trie.end()) == valid && found) ? "" : out + "fail\n";
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

//endregion

void scout_tests() {
    std::cout << "scout test(s): ";
    std::string s;

    // empty trie
    SimpleTrieTemplate<int,char> empty;
    s += scout_keyInTrie(empty,3,false,1);

    // filled trie, invalid key
    SimpleTrieTemplate<int,char> trie;
    trie.insert(3,'a');
    trie.insert(2,'b');
    trie.insert(5,'c');
    s += scout_keyInTrie(trie,8,false,2);

    // filled key, valid key
    s += scout_keyInTrie(trie,5,true,3);

    print_message(s);
}
//region //SCOUT
std::string scout_keyInTrie(SimpleTrieTemplate<int,char> &trie, int key, bool valid, int32_t cnt) {
    std::string out("\t- proper scout(" + std::to_string(cnt) + "): ");
    try {
        auto it = trie.scout(key);
        bool found(true);
        if (it.first) {
            found = it.second->first() == key;
        }
        return (it.first == valid && found) ? "" : out + "fail\n";
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

//endregion

void contains_tests() {
    std::cout << "contains test(s): ";
    std::string s;

    // empty trie
    SimpleTrieTemplate<int,char> empty;
    s += contains_keyInTrie(empty,3,false,1);

    // filled trie, invalid key
    SimpleTrieTemplate<int,char> trie;
    trie.insert(3,'a');
    trie.insert(2,'b');
    trie.insert(5,'c');
    s += contains_keyInTrie(trie,8,false,2);

    // filled key, valid key
    s += contains_keyInTrie(trie,5,true,3);

    print_message(s);
}
//region //CONTAINS
std::string contains_keyInTrie(SimpleTrieTemplate<int,char> &trie, int key, bool valid, int32_t cnt) {
    std::string out("\t- proper contains(" + std::to_string(cnt) + "): ");
    try {
        return (trie.contains(key) == valid) ? "" : out + "fail\n";
    }
    catch(...) {
        return out + "UNKNOWN ERROR\n";
    }
}

//endregion

void iterator_tests() {
    std::cout << "iterator test(s): ";
    std::string s;

    print_message(s);
}
//endregion