#include "str_dict.h"

#include <iostream>  // cerr, cout
#include <sstream>
#include <stdexcept>

#include "set.tpp"
#include "utils.tpp"
using namespace std;

//! Optimize Candidate: pre-cache INIT_STORAGE to save overhead.
//! StrDict::clear() assigns INIT_STORAGE to StrDict::storage, instead of
//! recomputing everytime.
// Dict<Code, Bytes> INIT_STORAGE;

StrDict::StrDict(unsigned code_bitsize) {
    if (code_bitsize <= 8) {
        throw runtime_error("Code bit size should be larger than 8");
    }

    capacity = (1 << code_bitsize) - 256 - 1;
    size = 0;
    storage.reserve(1 << code_bitsize);

    for (int i = 0; i < 256; i++) {
        storage.set(Code(i), Bytes(1, (unsigned char)i));
    }
}

void
StrDict::clear() {
    // TODO: should we add another call to storage.reserve() here?

    cout << "Statistics: " << statistics() << endl;

    storage.clear();
    size = 0;
    for (int i = 0; i < 256; i++) {
        storage.set(Code(i), Bytes(1, (unsigned char)i));
    }
}

// inline
bool
StrDict::contains(Code item) {
    return storage.contains(item);
}

Bytes
StrDict::get(Code key) {
    try {
        return storage.get(key);
    } catch (out_of_range e) {
        throw out_of_range("Code not present in StrDict: " + int2str(key));
    }
}

void
StrDict::add_new_str(Bytes item) {
    storage.set(Code(size + 256), item);
    size++;

    if (size == capacity) {
        //! Optimize Candidate: remove duplicate check to accelerate performance
        check_duplicate_str();
        clear();
    }
}

// void
// StrDict::check_duplicate_str() {
//     vector<Bytes> strings = storage.values();
//     Set<Bytes> set;
//     for (unsigned i = 0; i < strings.size(); i++) {
//         Bytes b = strings[i];
//         if (set.contains(b)) {
//             cerr << "Duplicate strings present in StrDict" << endl;
//             throw runtime_error("Duplicate strings present in StrDict");
//         }
//         set.add(strings[i]);
//     }
// }

string
StrDict::str() const {
    return storage.str();
}

//! TODO: visualize stat in histogram
string
StrDict::statistics() const {
    stringstream ss;
    // ss << "Totally " << << " buckets\n";
    // ss << empty << " buckets are empty; ";
    // ss << occupied << " buckets contain elements; ";
    // ss << "Current load factor " << << endl;
    string ret = ss.str();
    return ret;
}
