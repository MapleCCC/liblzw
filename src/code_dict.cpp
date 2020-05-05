#include "code_dict.h"

#include <iostream>  // cerr
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
        storage[Bytes(1, (unsigned char)i)] = Code(i);
    }
}

void
CodeDict::clear() {
    // cout << "Statistics: " << statistics() << endl;

    storage.clear();
    // reverse after clear. Standard doesn't provide guarantee that calling
    // clear() doesn't change capacity. Ref:
    // https://stackoverflow.com/questions/18467624/what-does-the-standard-say-about-how-calling-clear-on-a-vector-changes-the-capac
    storage.reserve(capacity + 256 + 1);
    size = 0;
    for (int i = 0; i < 256; i++) {
        storage[Bytes(1, (unsigned char)i)] = Code(i);
    }
}

// inline
bool
CodeDict::contains(Bytes item) const {
    return storage.count(item);
}

Code
CodeDict::get(Bytes key) const {
    try {
        return storage.at(key);
    } catch (out_of_range) {
        throw out_of_range("code is missing for string: " + key);
    }
}

void
CodeDict::add_new_code(Bytes item) {
    if (contains(item)) {
        throw runtime_error(item + " already in code dict");
    }

    storage[item] = Code(size + 256);
    size += 1;

    if (size == capacity) {
        clear();
    }
}

// TODO
string
CodeDict::str() const {
    // return storage.str();
    cerr << "Not Implemented Yet" << endl;
    throw runtime_error("Not Implemented Yet");
}

//! TODO: visualize stat in histogram
string
CodeDict::statistics() const {
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
