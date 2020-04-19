#include <cassert>
#include <iostream>

#include "bytes.h"
using namespace std;

int
main() {
    Bytes a('1'), b;
    b.push_back('1');
    assert(a == b);

    cout << "Pass all test cases for bytes" << endl;
    return 0;
}
