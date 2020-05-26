#include <cassert>
#include <iostream>

#include "bitmap.h"

using namespace std;

int
main() {
    Bitmap bmp(10);
    bmp.set(3, 1);
    assert(bmp.get(3));
    bmp.set(3, 0);
    assert(!bmp.get(3));
    bmp.set(1, 1);
    bmp.set(9, 1);
    assert(bmp.get(1));
    assert(bmp.get(9));
    bmp.reset();
    assert(!bmp.get(1));
    assert(!bmp.get(9));

    bmp.set(1, 1);
    bmp.set(9, 1);
    bmp.resize(20);
    assert(bmp.get(1));
    assert(bmp.get(9));
    assert(!bmp.get(19));

    cout << "Pass test cases for Bitmap" << endl;

    return 0;
}
