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

void insert_tests();

void erase_tests();

void swap_tests();

void find_tests();

void scout_tests();

void contains_tests();

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
//    clear_tests();
//    insert_tests();
//    erase_tests();
//    swap_tests();
//    find_tests();
//    scout_tests();
//    contains_tests();
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

    // when empty

    // when not empty

    // called multiple times

    print_message(s);
}

void insert_tests() {
    std::cout << "insert test(s): ";
    std::string s;

    // insert duplicate values at same key
    // affects size appropriately

    ///// ARTICLES
    // insert non-const rvalues into empty
    // insert non-const rvalues into non-empty

    // insert non-const lvalues into empty
    // insert non-const lvalues into non-empty

    // insert const rvalues into empty
    // insert const rvalues into non-empty

    // insert const lvalues into empty
    // insert const lvalues into non-empty

    // insert duplicate keys into empty
    // insert duplicate keys into non-empty


    ///// VALUES
    // insert non-const rvalues

    // insert non-const lvalues

    // insert const rvalues

    // insert const lvalues

    // insert non-list value

    // insert list value

    print_message(s);
}

void erase_tests() {
    std::cout << "erase test(s): ";
    std::string s;

    print_message(s);
}

void swap_tests() {
    std::cout << "swap test(s): ";
    std::string s;

    print_message(s);
}

void find_tests() {
    std::cout << "find test(s): ";
    std::string s;

    print_message(s);
}

void scout_tests() {
    std::cout << "scout test(s): ";
    std::string s;

    print_message(s);
}

void contains_tests() {
    std::cout << "contains test(s): ";
    std::string s;

    print_message(s);
}

void iterator_tests() {
    std::cout << "iterator test(s): ";
    std::string s;

    print_message(s);
}
//endregion