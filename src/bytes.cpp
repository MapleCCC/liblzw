#include "bytes.h"

#include <iostream>

#include "utils.h"

using namespace std;

Bytes::Bytes(const unsigned char c) { storage.push_back(c); }

Bytes::Bytes(const char* s) {
    int i = 0;
    while (s[i] != '\0') {
        storage.push_back((unsigned char)s[i]);
        i++;
    }
}

Bytes::Bytes(const string s) {
    for (unsigned int i = 0; i < s.length(); i++) {
        storage.push_back(s[i]);
    }
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

// inline
void
Bytes::push_back(unsigned char c) {
    storage.push_back(c);
}

long long
Bytes::hash() const {
    long long h = 0;
    for (unsigned int i = 0; i < storage.size(); i++) {
        h = h * 31 + storage.at(i);
    }
    return h;
}

bool
Bytes::operator==(const Bytes& rhs) {
    // int len = length();
    // if (len != rhs.length()) return false;
    // for (int i = 0; i < len; i++) {
    //     if (storage[i] != rhs.storage[i]) {
    //         return false;
    //     }
    // }
    // return true;
    return storage == rhs.storage;
}

Bytes
Bytes::operator+(const Bytes& rhs) const {
    Bytes ret;
    ret.storage = storage;
    ret.storage.insert(ret.storage.end(), rhs.storage.begin(),
                       rhs.storage.end());
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
    return ret;
}

Bytes::operator std::string() const {
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

std::string
operator+(const std::string& s, const Bytes& bytes) {
    return s + static_cast<string>(bytes);
}

std::ostream&
operator<<(ostream& os, const Bytes& bytes) {
    os << static_cast<string>(bytes);
    return os;
}
