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
Code::operator!=(const Code& rhs) const {
    return x != rhs.x;
}

string
Code::str() const {
    return int2str(x);
}

ostream&
operator<<(ostream& os, const Code& code) {
    os << code.unbox();
    return os;
}
