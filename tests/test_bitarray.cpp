#include <cassert>
#include <iostream>

#include "bitarray.h"
#include "bytes.h"

using namespace std;

int
main() {
    Bitarray b;
    b.push_bytes_back(Bytes(1, '1'));
    unsigned char c = b.pop_byte_front();
    assert(c == 49);

    b = Bitarray::from_int(49, 12);
    assert(b.to_int() == 49);

    assert(b.slice(0, 8).to_int() == b.pop_byte_front());

    cout << "Pass test cases for bitarray" << endl;
    return 0;
}
