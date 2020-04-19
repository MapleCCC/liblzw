#include "str_dict.h"

#include <stdexcept>

#include "set.tpp"
using namespace std;

StrDict::StrDict(int code_bitsize) {
    if (code_bitsize <= 8) {
        throw runtime_error("Code bit size should be larger than 8");
    }

    capacity = (1 << code_bitsize) - 256 - 1;
    size = 0;
    storage.reserve(1 << code_bitsize);

    for (int i = 0; i < 256; i++) {
        storage.set(Code(i), Bytes((unsigned char)i));
    }
}

void
StrDict::clear() {
    // TODO: should we add another call to storage.reserve() here?
    storage.clear();
    size = 0;
    for (int i = 0; i < 256; i++) {
        storage.set(Code(i), Bytes((unsigned char)i));
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
        throw out_of_range("Code not present in StrDict: " + key.str());
    }
}

void
StrDict::add_new_str(Bytes item) {
    storage.set(Code(size + 256), item);
    size++;

    if (size == capacity) {
        // check_duplicate_str();
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
