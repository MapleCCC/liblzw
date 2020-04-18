#include "bytes.h"

#include <iostream>

#include "utils.tpp"  // byte2hex

using namespace std;

/* TODO: encapsulate intialize_hashcode and update_hashcode to standalone
 * stateful hash object */
// static void initialize_hashcode() {
//     hashcode = 0;
// }

Bytes::Bytes() { hashcode = 0; }

Bytes::Bytes(unsigned char c) {
    hashcode = 0;
    push_back(c);
}

Bytes::Bytes(const Bytes& other) {
    hashcode = 0;
    storage = other.storage;
    hashcode = other.hashcode;
}

unsigned char
Bytes::get(unsigned index) const {
    if (index >= storage.size()) {
        cerr << "bytes length is " << storage.size()
             << ", but try to index: " << index << endl;
        throw out_of_range("Index out of range");
    }
    return storage.at(index);
}

unsigned char
Bytes::get_first_byte() const {
    if (!storage.size()) {
        cerr << "Can't get first byte of an empty byte string" << endl;
        throw out_of_range("Index out of range");
    }
    return storage.at(0);
}

void
Bytes::push_back(unsigned char c) {
    storage.push_back(c);
    update_hashcode(c);
}

// algorithmic detail is inside update_hashcode()
void
Bytes::update_hashcode(unsigned char c) {
    hashcode = (hashcode << 4) - hashcode + c;
}

long long
Bytes::hash() const {
    return hashcode;
}

Bytes&
Bytes::operator=(const Bytes& rhs) {
    hashcode = rhs.hashcode;
    storage = rhs.storage;
    return *this;
}

bool
Bytes::operator==(const Bytes& rhs) const {
    return storage == rhs.storage;
}

bool
Bytes::operator!=(const Bytes& rhs) const {
    return storage != rhs.storage;
}

Bytes
Bytes::operator+(const Bytes& rhs) const {
    Bytes ret(*this);
    ret.storage.insert(ret.storage.end(), rhs.storage.begin(),
                       rhs.storage.end());
    for (unsigned i = 0; i < rhs.storage.size(); i++) {
        ret.update_hashcode(rhs.storage.at(i));
    }
    return ret;
}

// Copy and return-by-value may be too expensive. Investigate profile result to
// see if here suffices performance hotspot. Try adopting immutable data
// structure as internal storage mechanism to reduce space cost. Or use move
// semantic to avoid the cost of creating and copying temporaries.
Bytes
Bytes::operator+(const unsigned char c) {
    Bytes ret(*this);
    ret.push_back(c);
    return ret;
}

string
Bytes::str() const {
    string ret;
    ret.push_back('b');
    ret.push_back('\'');
    for (unsigned i = 0; i < storage.size(); i++) {
        ret.push_back('\\');
        ret.push_back('x');
        unsigned char byte = storage.at(i);
        ret += byte2hex(byte);
    }
    ret.push_back('\'');
    return ret;
}

ostream&
operator<<(ostream& os, const Bytes& bytes) {
    os.write((char*)&(bytes.storage)[0],
             bytes.storage.size() * sizeof(unsigned char));
    return os;
}
