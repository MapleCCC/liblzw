#include "code_dict.h"

#include <cmath>
#include <stdexcept>

using namespace std;

CodeDict::CodeDict(int code_bitsize) {
    if (code_bitsize <= 8) {
        throw runtime_error("Code bit size should be larger than 8");
    }

    capacity = pow(2, code_bitsize) - 256 - 1;
    size = 0;
    storage.reserve(pow(2, code_bitsize));

    for (int i = 0; i < 256; i++) {
        storage.set(Bytes((unsigned char)i), Code(i));
    }
}

void
CodeDict::clear() {
    // TODO: should we add another call to storage.reserve() here?
    storage.clear();
    size = 0;
    for (int i = 0; i < 256; i++) {
        storage.set(Bytes((unsigned char)i), Code(i));
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
        throw out_of_range("code is missing for string: " + key.str());
    }
}

void
CodeDict::add_new_code(Bytes item) {
    if (contains(item)) {
        throw runtime_error(item.str() + " already in code dict");
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
