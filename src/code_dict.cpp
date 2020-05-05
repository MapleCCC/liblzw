#include "code_dict.h"

#include <iostream>  // cerr, cout
#include <sstream>
#include <stdexcept>

using namespace std;

CodeDict::CodeDict(unsigned code_bitsize) {
    if (code_bitsize <= 8) {
        throw runtime_error("Code bit size should be larger than 8");
    }

    capacity = (1 << code_bitsize) - 256 - 1;
    size = 0;
    storage.reserve(1 << code_bitsize);

    for (int i = 0; i < 256; i++) {
        storage.set(Bytes(1, (unsigned char)i), Code(i));
    }
}

void
CodeDict::clear() {
    // TODO: should we add another call to storage.reserve() here?

    cout << "Statistics: " << statistics() << endl;

    storage.clear();
    size = 0;
    for (int i = 0; i < 256; i++) {
        storage.set(Bytes(1, (unsigned char)i), Code(i));
    }
}

inline bool
CodeDict::contains(Bytes item) {
    return storage.contains(item);
}

Code
CodeDict::get(Bytes key) {
    try {
        return storage.get(key);
    } catch (out_of_range) {
        throw out_of_range("code is missing for string: " + key);
    }
}

void
CodeDict::add_new_code(Bytes item) {
    if (contains(item)) {
        throw runtime_error(item + " already in code dict");
    }

    storage.set(item, Code(size + 256));
    size += 1;

    if (size == capacity) {
        clear();
    }
}

string
CodeDict::str() const {
    return storage.str();
}

//! TODO: visualize stat in histogram
string
CodeDict::statistics() const {
    stringstream ss;
    // ss << "Totally " << << " buckets\n";
    // ss << empty << " buckets are empty; ";
    // ss << occupied << " buckets contain elements; ";
    // ss << "Current load factor " << << endl;
    string ret = ss.str();
    return ret;
}
