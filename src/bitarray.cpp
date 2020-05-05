#include "bitarray.h"

#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

Bitarray::Bitarray() {
    x = 0;
    size = 0;
};

// start is closed, stop is open
Bitarray
Bitarray::slice(unsigned start, unsigned stop) const {
    // FIXME: the boundary conditions for valid slice indices
    //! Optimize Candidate: remove boundary check to accelerate performance.
    //! Tradeoff robustness for performance. Assume caller is consenting adult,
    //! and reposible for passing in valid arguments.
    if (!(start <= stop && start <= size && stop <= size)) {
        cerr << "Buffer's length is " << this->length() << endl;
        cerr << "Slice invalid: start: " << start << " stop: " << stop << endl;
        throw runtime_error("Slice invalid");
    }

    Bitarray ret;
    unsigned mask = (1 << (stop - start)) - 1;
    ret.x = (x >> (size - stop)) & mask;
    ret.size = stop - start;
    return ret;
}

Bitarray
Bitarray::slice(unsigned stop) const {
    return slice(0, stop);
}

unsigned
Bitarray::length() const {
    return size;
}

Bitarray&
Bitarray::operator+=(const Bitarray& rhs) {
    if ((size + rhs.size) > MAX_BIG_INT_BIT_SIZE) {
        cerr << "Bitarray Overflow when Bitarray::operator+=" << endl;
        throw overflow_error("Bitarray Overflow when Bitarray::operator+=");
    }

    x = (x << rhs.size) + rhs.x;
    size += rhs.size;
    return *this;
}

int
Bitarray::to_int() {
    return x;
}

static unsigned
bit_length(unsigned x) {
    unsigned length = 0;
    unsigned temp = 1;
    while (temp < x) {
        temp <<= 1;
        length++;
    }
    return length;
}

Bitarray
Bitarray::from_int(int x, int bit_size = -1) {
    if (x < 0) {
        cerr << "Negative integer is not supported yet" << endl;
        throw runtime_error("Negative integer is not supported yet");
    }

    unsigned canonical_bit_length = bit_length(x);

    if (bit_size == -1) {
        bit_size = canonical_bit_length;
    } else {
        if (bit_size < (signed)canonical_bit_length) {
            cerr << "Overflow Error when calling from_int" << endl;
            // TODO: more informative error message
            throw overflow_error("Overflow Error when calling from_int");
        }
    }

    Bitarray ret;
    ret.x = x;
    ret.size = bit_size;
    return ret;
}

void
Bitarray::push_bytes_back(Bytes bytes) {
    for (unsigned i = 0; i < bytes.length(); i++) {
        unsigned char byte = bytes[i];
        x = (x << 8) + byte;
        size += 8;
    }
}

unsigned char
Bitarray::pop_byte_front() {
    if (size < 8) {
        cerr << "pop byte from bitarray with less than 8 bits" << endl;
        throw runtime_error("pop byte from bitarray with less than 8 bits");
    }

    unsigned char ret = x >> (size - 8);
    x &= (1 << (size - 8)) - 1;
    size -= 8;
    return ret;
}

string
Bitarray::str() const {
    string ret;
    ret += "Bitarray(";
    unsigned mask = 1 << (size - 1);
    for (unsigned i = 0; i < size; i++) {
        ret += (x & mask) ? '1' : '0';
        mask >>= 1;
    }
    ret += ')';
    return ret;
}

ostream&
operator<<(ostream& os, const Bitarray& ba) {
    os << ba.str();
    return os;
}
