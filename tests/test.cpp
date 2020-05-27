#include <cassert>
#include <exception>
#include <iostream>

#include "bitarray.h"
#include "bytes.h"
#include "code.h"
#include "code_dict.h"
#include "str_dict.h"
#include "utils.tpp"

using namespace std;

// Add more test cases to cover as many interface as possible.
// C++ test coverage calculation tool?

void
test_code_dict() {
    CodeDict cd(12);
    cd.add_new_code("123");
    assert(cd.get("123") == 256);
    assert(cd.contains("123"));
    assert(!cd.contains("234"));

    cd.add_new_code("234");
    assert(cd.get("234") == 257);
    assert(cd.contains("234"));
    assert(cd.contains("123"));
    assert(!cd.contains("256"));

    cd.clear();
    assert(!cd.contains("123"));
    assert(!cd.contains("234"));
    cd.add_new_code("234");
    assert(cd.get("234") == 256);
    assert(cd.contains("234"));
    assert(!cd.contains("256"));

    cd.clear();

    for (int i = 0; i < (1 << 12) - 256 - 1; i++) {
        cd.add_new_code("1" + int2str(i));
    }
    cd.add_new_code("2301");
    assert(cd.get("2301") == 256);
    assert(!cd.contains("10"));

    cout << "Pass test cases for CodeDict" << endl;
}

void
test_str_dict() {
    StrDict sd(12);
    sd.add_new_str(Bytes("123"));
    assert(sd.get(256) == "123");
    assert(sd.contains(256));
    assert(!sd.contains(257));

    sd.add_new_str(Bytes("234"));
    assert(sd.get(257) == "234");
    assert(sd.contains(256));
    assert(sd.contains(257));
    assert(!sd.contains(258));

    sd.clear();
    assert(!sd.contains(256));
    assert(!sd.contains(257));
    sd.add_new_str("234");
    assert(sd.get(256) == "234");
    assert(sd.contains(256));
    assert(!sd.contains(257));

    sd.clear();

    for (int i = 0; i < (1 << 12) - 256 - 1; i++) {
        sd.add_new_str("1" + int2str(i));
    }
    sd.add_new_str("2301");
    assert(sd.get(256) == "2301");
    assert(!sd.contains(257));

    cout << "Pass test cases for StrDict" << endl;
}

void
test_is_two_files_identical() {
    assert(is_two_files_identical(".gitignore", ".gitignore"));
    assert(!is_two_files_identical(".travis.yml", ".gitignore"));

    cout << "Pass test cases for is_two_files_identical" << endl;
}

void
test_grouper() {
    vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);
    v.push_back(6);
    v.push_back(7);
    vector<vector<int> > v2 = grouper(v, 2);
    vector<int> v3;
    for (unsigned i = 0; i < v2.size(); i++) {
        vector<int> subv = v2.at(i);
        v3.insert(v3.end(), subv.begin(), subv.end());
    }
    v3.push_back(7);
    assert(v == v3);

    cout << "Pass test cases for grouper" << endl;
}

void
test_bitarray() {
    Bitarray b;
    b.push_bytes_back(Bytes(1, (unsigned char)0));
    Bytes byte(1, b.pop_byte_front());
    assert(byte.length() == 1);
    assert(byte[0] == 0);
    assert(b.length() == 0);
    b.push_bytes_back(Bytes(1, (unsigned char)0));
    byte = Bytes(1, b.pop_byte_front());
    assert(byte.length() == 1);
    assert(byte[0] == 0);
    assert(b.length() == 0);

    b = Bitarray::from_int(0, 12);
    assert(b.length() == 12);
    assert(b.pop_byte_front() == (unsigned char)0);

    cout << "Pass test cases for bitarray" << endl;
}

int
main() {
    try {
        test_code_dict();
        test_str_dict();
        test_is_two_files_identical();
        test_grouper();
        test_bitarray();

        cout << "Pass all test cases!" << endl;
    } catch (exception e) {
        cout << "Catched exception in main\n";
        cout << e.what() << endl;
    }
    return 0;
}
