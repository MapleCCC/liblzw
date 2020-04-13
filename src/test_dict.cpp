#include <cassert>
#include <iostream>

#include "bytes.h"
#include "code.h"
#include "dict.h"
using namespace std;

int
main() {
    Dict<Bytes, Code> d;
    d.set("1", Code(10));
    d.set(Bytes("10"), Code(100));
    assert(d.get(Bytes("1")) == Code(10));
    assert(d.contains(Bytes("1")));
    assert(d.contains_value(10));
    assert(d.contains_value(100));
    assert(!d.contains_value(200));
    assert(!d.contains(Bytes("2")));
    assert(d.contains(Bytes("10")));
    assert(d.get(Bytes("10")) == Code(100));
    assert(d.length() == 2);
    d.clear();
    assert(d.length() == 0);

    cout << "Pass test cases for Dict" << endl;

    return 0;
}
