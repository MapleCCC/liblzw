#include "bitmap.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

using namespace std;

// Mind: we use little-endian, that is, the least significant bit of the byte
// stores the smallest index

// Current implementation is unsigned char array. Alternatively we could use a
// big integer to implement. Some big int library needs to be built beforehand.

Bitmap::Bitmap(unsigned size) {
    this->size = size;
    nbytes = ceil(size * 1.0 / 8);

    storage = (unsigned char*)malloc(nbytes * sizeof(*storage));
    for (unsigned i = 0; i < nbytes; i++) {
        storage[i] = 0;
    }
}

Bitmap::Bitmap(const Bitmap& other) {
    size = other.size;
    nbytes = other.nbytes;
    storage = (unsigned char*)malloc(nbytes * sizeof(*storage));
    for (unsigned i = 0; i < nbytes; i++) {
        storage[i] = other.storage[i];
    }
}

Bitmap::~Bitmap() { free(storage); }

void
Bitmap::reset() {
    for (unsigned i = 0; i < nbytes; i++) {
        storage[i] = 0;
    }
}

inline unsigned
Bitmap::length() const {
    return size;
}

void
Bitmap::set(unsigned index, bool bit) {
    if (index >= size) {
        cerr << "Index out of range" << endl;
        throw out_of_range("Set bitmap out of range");
    }
    unsigned byte_index = index / 8;
    unsigned offset = index & (8 - 1);
    unsigned mask;
    if (bit) {
        mask = 1 << offset;
        storage[byte_index] |= mask;
    } else {
        mask = ~(1 << offset);
        storage[byte_index] &= mask;
    }
}

bool
Bitmap::get(unsigned index) const {
    if (index >= size) {
        cerr << "Index out of range" << endl;
        throw out_of_range("Get bitmap out of range");
    }
    unsigned byte_index = index / 8;
    unsigned offset = index & (8 - 1);
    unsigned mask = 1 << offset;
    return (storage[byte_index] & mask) >> offset;
}

void
Bitmap::resize(unsigned new_size) {
    // We don't check whether resizing to the new_size leads to truncating
    // existing bits stoarge. We leave the responsibility to caller instead.
    unsigned new_nbytes = ceil(new_size * 1.0 / 8);
    storage = (unsigned char*)realloc(storage, new_nbytes * sizeof(*storage));
    if (new_nbytes > nbytes) {
        for (unsigned i = nbytes; i < new_nbytes; i++) {
            storage[i] = 0;
        }
    }
    nbytes = new_nbytes;
    size = new_size;
}
