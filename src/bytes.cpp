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

Bytes::Bytes(const unsigned char c) {
    hashcode = 0;
    storage.push_back(c);
    update_hashcode(c);
}

Bytes::Bytes(const char* s) {
    hashcode = 0;
    int i = 0;
    while (s[i] != '\0') {
        storage.push_back((unsigned char)s[i]);
        update_hashcode((unsigned char)s[i]);
        i++;
    }
}

Bytes::Bytes(const string s) {
    hashcode = 0;
    for (unsigned int i = 0; i < s.length(); i++) {
        storage.push_back(s[i]);
        update_hashcode(s[i]);
    }
}

Bytes::Bytes(const Bytes& other) {
    storage = other.storage;
    hashcode = other.hashcode;
}

unsigned char
Bytes::get(int index) const {
    if (index < 0 || index >= storage.size()) {
        cerr << "bytes length is " << storage.size()
             << ", but try to index: " << index << endl;
        throw out_of_range("Index out of range");
    }
    return storage.at(index);
}

unsigned char
Bytes::get_first_byte() const {
    return storage[0];
}

void
Bytes::push_back(unsigned char c) {
    storage.push_back(c);
    update_hashcode(c);
}

// algorithmic detail is inside update_hashcode()
void
Bytes::update_hashcode(unsigned char c) {
    hashcode = hashcode * 31 + c;
}

long long
Bytes::hash() const {
    return hashcode;
}

// Turn out that operator= is not equal to returning new Bytes instance???!!!
Bytes&
Bytes::operator=(const Bytes& rhs) {
    if (this != &rhs) {
        storage = rhs.storage;
        hashcode = rhs.hashcode;
    }
    return *this;
}

// move assignment operator
Bytes&
Bytes::operator=(Bytes&& rhs) {
    if (this != &rhs) {
        storage = rhs.storage;
        hashcode = rhs.hashcode;
    }
    return *this;
}

bool
Bytes::operator==(const Bytes& rhs) const {
    return storage == rhs.storage;
}

bool
Bytes::operator!=(const Bytes& rhs) const {
    return !operator==(rhs);
}

Bytes
Bytes::operator+(const Bytes& rhs) const {
    Bytes ret;
    ret.storage = storage;
    ret.storage.insert(ret.storage.end(), rhs.storage.begin(),
                       rhs.storage.end());
    ret.hashcode = hashcode;
    for (int i = 0; i < rhs.storage.size(); i++) {
        ret.update_hashcode(rhs.storage[i]);
    }
    return ret;
}

// Copy and return-by-value may be too expensive. Investigate profile result to
// see if here suffices performance hotspot. Try adopting immutable data
// structure as internal storage mechanism to reduce space cost.
Bytes
Bytes::operator+(const unsigned char c) {
    Bytes ret;
    ret.storage = storage;
    ret.storage.push_back(c);
    ret.hashcode = hashcode;
    ret.update_hashcode(c);
    return ret;
}

string
Bytes::str() const {
    string ret;
    ret.push_back('b');
    ret.push_back('\'');
    for (int i = 0; i < storage.size(); i++) {
        ret.push_back('\\');
        ret.push_back('x');
        unsigned char byte = storage[i];
        ret += byte2hex(byte);
    }
    ret.push_back('\'');
    return ret;
}

string
operator+(const string& s, const Bytes& bytes) {
    return s + bytes.str();
}

ostream&
operator<<(ostream& os, const Bytes& bytes) {
    os << bytes.str();
    return os;
}
