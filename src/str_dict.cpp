#include "str_dict.h"

#include <iostream>  // cerr, cout
#include <set>
#include <sstream>
#include <stdexcept>

#include "utils.tpp"

using namespace std;

//! Optimize Candidate: pre-cache INIT_STORAGE to save overhead.
//! StrDict::clear() assigns INIT_STORAGE to StrDict::storage, instead of
//! recomputing everytime.
// unordered_map<Code, Bytes> INIT_STORAGE;

StrDict::StrDict(unsigned code_bitsize) {
    if (code_bitsize <= 8) {
        throw runtime_error("Code bit size should be larger than 8");
    }

    capacity = (1 << code_bitsize) - 256 - 1;
    size = 0;
    storage.reserve(1 << code_bitsize);

    for (int i = 0; i < 256; i++) {
        storage[Code(i)] = Bytes(1, (unsigned char)i);
    }
}

void
StrDict::clear() {
    // cout << "Statistics: " << statistics() << endl;

    storage.clear();
    // Reserve after clear. Standard doesn't provide guarantee that calling
    // clear() doesn't change capacity. Ref:
    // https://stackoverflow.com/questions/18467624/what-does-the-standard-say-about-how-calling-clear-on-a-vector-changes-the-capac
    storage.reserve(capacity + 256 + 1);
    size = 0;
    for (int i = 0; i < 256; i++) {
        storage[Code(i)] = Bytes(1, (unsigned char)i);
    }
}

// inline
bool
StrDict::contains(Code item) const {
    return storage.count(item);
}

Bytes
StrDict::get(Code key) const {
    try {
        return storage.at(key);
    } catch (out_of_range e) {
        throw out_of_range("Code not present in StrDict: " + int2str(key));
    }
}

void
StrDict::add_new_str(Bytes item) {
    storage[Code(size + 256)] = item;
    size++;

    if (size == capacity) {
        //! Optimize Candidate: remove duplicate check to accelerate performance
        check_duplicate_str();
        clear();
    }
}

void
StrDict::check_duplicate_str() {
    set<Bytes> s;
    unordered_map<Code, Bytes>::iterator itr;
    for (itr = storage.begin(); itr != storage.end(); itr++) {
        Bytes b = itr->second;
        if (s.count(b)) {
            cerr << "Duplicate strings present in StrDict" << endl;
            throw runtime_error("Duplicate strings present in StrDict");
        }
        s.insert(b);
    }
}

// TODO
string
StrDict::str() const {
    // return storage.str();
    cerr << "Not Implemented Yet" << endl;
    throw runtime_error("Not Implemented Yet");
}

//! TODO: visualize stat in histogram
string
StrDict::statistics() const {
    unsigned empty = 0, single = 0, multiple = 0;
    for (unsigned i = 0; i < storage.bucket_count(); i++) {
        switch (storage.bucket_size(i)) {
            case 0:
                empty++;
                break;
            case 1:
                single++;
                break;
            default:
                multiple++;
                break;
        }
    }
    stringstream ss;
    ss << "Totally " << storage.bucket_count() << " buckets\n";
    ss << empty << " buckets are empty; ";
    ss << single << " buckets contain single element; ";
    ss << multiple << " buckets contain multiple elements;\n";
    ss << "Current load factor " << storage.load_factor() << endl;
    string ret = ss.str();
    return ret;
}
