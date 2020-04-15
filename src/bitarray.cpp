#include "bitarray.h"

#include <algorithm>
#include <cassert>
#include <iostream>

#include "converter.h"
using namespace std;

Bitarray::Bitarray(const vector<Bit>& bits) : storage(bits) {}

Bit
Bitarray::get(int index) const {
    if (index < 0 || index >= storage.size()) {
        cerr << "Index out of range" << endl;
        throw runtime_error("Index out of range");
    }
    return storage.at(index);
}

// start is closed, stop is open
Bitarray
Bitarray::slice(int start, int stop) {
    // FIXME: the boundary conditions for valid slice indices
    if (start > stop || start < 0 || start > storage.size() || stop < 0 ||
        stop > storage.size()) {
        cerr << "Buffer's length is " << this->length() << endl;
        cerr << "Slice invalid: start: " << start << " stop: " << stop << endl;
        throw runtime_error("Slice invalid");
    }

    // Will it be faster to insert than double construction?
    // Bitarray new_bitarray(vector<Bit>(storage.begin() + start,
    // storage.begin()+stop));
    Bitarray new_bitarray;
    new_bitarray.storage.insert(new_bitarray.storage.end(),
                                storage.begin() + start,
                                storage.begin() + stop);
    assert(new_bitarray.length() == stop - start);
    return new_bitarray;
}

Bitarray
Bitarray::slice(int stop) {
    return slice(0, stop);
}

int
Bitarray::length() const {
    return storage.size();
}

Bitarray
Bitarray::operator+(const Bitarray& rhs) {
    Bitarray new_bitarray;
    new_bitarray.storage.insert(new_bitarray.storage.end(), storage.begin(),
                                storage.end());
    new_bitarray.storage.insert(new_bitarray.storage.end(), rhs.storage.begin(),
                                rhs.storage.end());
    return new_bitarray;
}

int
Bitarray::to_int() {
    return bits2int(storage);
}

Bitarray
// Bitarray::from_int(int x, int bit_size) {
Bitarray::from_int(int x, int bit_size = -1) {
    if (x < 0) {
        cerr << "Negative integer is not supported yet" << endl;
        throw runtime_error("Negative integer is not supported yet");
    }

    vector<Bit> bits = int2bits(x);

    if (bit_size == -1) {
        bit_size = bits.size();
    } else {
        if (bit_size < bits.size()) {
            cerr << "Overflow Error" << endl;
            // TODO: more informative error message
            throw overflow_error("Overflow Error");
        }
    }

    // FIXME: don't know why here has error?
    // for (int i = 0; i < (bit_size - bits.size()); i++) {
    int pad_zero_num = bit_size - bits.size();
    for (int i = 0; i < pad_zero_num; i++) {
        bits.insert(bits.begin(), ZERO);
    }

    Bitarray ret(bits);
    return ret;
}

void
Bitarray::push_bytes_back(Bytes bytes) {
    vector<Bit> bits = bytes2bits(bytes);
    storage.insert(storage.end(), bits.begin(), bits.end());
}

Bytes
Bitarray::pop_byte_front() {
    if (storage.size() < 8) {
        cerr << "pop byte from bitarray with less than 8 bits" << endl;
        throw runtime_error("pop byte from bitarray with less than 8 bits");
    }
    return pop_bytes_front(1);
}

Bytes
Bitarray::pop_bytes_front(int n) {
    if (storage.size() < n * 8) {
        cerr << "pop bytes number exceeds bitarray's containing bytes' number"
             << endl;
        throw runtime_error(
            "pop bytes number exceeds bitarray's containing bytes' number");
    }
    Bytes bs =
        bits2bytes(vector<Bit>(storage.begin(), storage.begin() + 8 * n));
    storage = vector<Bit>(storage.begin() + 8 * n, storage.end());
    return bs;
}

string
Bitarray::str() const {
    string ret;
    ret += "Bitarray(";
    for (int i = 0; i < storage.size(); i++) {
        ret += storage[i] ? '1' : '0';
        if (i != storage.size() - 1) {
            ret += ", ";
        }
    }
    ret += ")";
    return ret;
}

ostream&
operator<<(ostream& os, const Bitarray& ba) {
    os << ba.str();
    return os;
}
