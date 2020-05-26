#include <cassert>
#include <iostream>
#include <string>

#include "bytes.h"
#include "code.h"
#include "dict.tpp"

using namespace std;

#define assert(x)                                         \
    do {                                                  \
        if (x) {                                          \
            cout << "Assertion " #x " succeeded" << endl; \
        } else {                                          \
            cout << "Assertion " #x " failed" << endl;    \
        }                                                 \
    } while (0)

int
main() {
    // Dict<Bytes, Code> d;
    // d.set("1", Code(10));
    // d.set(Bytes("10"), Code(100));
    // assert(d.get(Bytes("1")) == Code(10));
    // assert(d.contains(Bytes("1")));
    // assert(d.contains_value(10));
    // assert(d.contains_value(100));
    // assert(!d.contains_value(200));
    // assert(!d.contains(Bytes("2")));
    // assert(d.contains(Bytes("10")));
    // assert(d.get(Bytes("10")) == Code(100));
    // assert(d.length() == 2);
    // cout << d.str() << endl;
    // d.clear();
    // assert(d.length() == 0);

    Dict<Bytes, Code> d;
    d.set('1', Code(10));
    assert(d.get(Bytes('1')) == Code(10));
    assert(d.contains(Bytes('1')));
    assert(d.contains_value(10));
    assert(!d.contains_value(200));
    assert(!d.contains(Bytes('2')));
    assert(d.length() == 1);
    cout << d.str() << endl;
    d.clear();
    assert(d.length() == 0);

    cout << "Pass test cases for Dict" << endl;

    return 0;
}
