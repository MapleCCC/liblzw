#include "str_dict.h"

#include <cmath>
#include <stdexcept>
using namespace std;

StrDict::StrDict(int code_bitsize) {
    if (code_bitsize <= 8) {
        throw runtime_error("Code bit size should be larger than 8");
    }
    capacity = pow(2, code_bitsize) - 256 - 1;
    size = 0;
    for (int i = 0; i < 256; i++) {
        storage.set(Code(i), Bytes((unsigned char)i));
        str_cache.add(Bytes((unsigned char)i));
    }
}

void
StrDict::clear() {
    storage.clear();
    str_cache.clear();
    size = 0;
    for (int i = 0; i < 256; i++) {
        storage.set(Code(i), Bytes((unsigned char)i));
        str_cache.add(Bytes((unsigned char)i));
    }
}

bool
StrDict::contains(Code item) {
    return storage.contains(item);
}

Bytes
StrDict::get(Code key) {
    try {
        return storage.get(key);
    } catch (out_of_range e) {
        // throw out_of_range("Code not present in StrDict: " + key);
        throw out_of_range("Code not present in StrDict: ");
    }
}

void
StrDict::add_new_str(Bytes item) {
    if (str_cache.contains(item)) {
        // throw runtime_error("string already in StrDict: " + item);
        throw runtime_error("string already in StrDict: ");
    }
    storage.set(Code(size + 256), item);
    size++;
    str_cache.add(item);

    if (size == capacity) {
        clear();
    }
}
