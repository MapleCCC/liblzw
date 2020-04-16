#include "code.h"

#include "utils.tpp"  // int2str
using namespace std;

// For now, we are good with returning integer itself as its own hash value.
// Inspired from CPython.
long
Code::hash() const {
    return x;
}

bool
Code::operator==(const Code& rhs) const {
    return x == rhs.x;
}

bool
Code::operator==(const int rhs) const {
    return x == rhs;
}

Code::operator int() const { return x; }

string
Code::str() const {
    return int2str(x);
}

bool
operator==(const int lhs, const Code& rhs) {
    return lhs == rhs.get();
}

ostream&
operator<<(ostream& os, const Code& code) {
    os << code.get();
    return os;
}

string
operator+(const string& s, const Code& code) {
    return s + code.str();
}
