#include <cassert>
#include <exception>
#include <iostream>

#include "bitarray.h"
#include "bytes.h"
#include "code.h"
#include "code_dict.h"
#include "codec.h"
#include "dict.h"
#include "lzwfile.h"
#include "str_dict.h"
#include "utils.h"

using namespace std;

// Add more test cases to cover as many interface as possible.
// C++ test coverage calculation tool?

void
test_dict() {
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
}

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

    for (int i = 0; i < pow(2, 12) - 256 - 1; i++) {
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

    for (int i = 0; i < pow(2, 12) - 256 - 1; i++) {
        sd.add_new_str("1" + int2str(i));
    }
    sd.add_new_str("2301");
    assert(sd.get(256) == "2301");
    assert(!sd.contains(257));

    cout << "Pass test cases for StrDict" << endl;
}

void
test_codec() {
    LZWEncoder encoder(12);
    LZWDecoder decoder(12);

    vector<Code>* codes = encoder.encode_file("Ephesians.txt");
    vector<Code>::iterator codes_itr = codes->begin();
    decoder.decode_file("E.txt", codes_itr);
    delete codes;

    assert(is_two_files_identical("Ephesians.txt", "E.txt"));

    cout << "Pass test cases for codec" << endl;
}

void
test_utils() {
    assert(is_two_files_identical("Ephesians.txt", "E.txt"));
    assert(!is_two_files_identical("Ephesians.txt", ".gitignore"));

    cout << "Pass test cases for utils" << endl;
}

void
test_lzwfile() {
    vector<string>* header = read_lzwfile_header("a.lzw");
    for (int i = 0; i < header->size(); i++) {
        cout << header->at(i) << endl;
    }
    write_lzwfile_header("b.lzw", header);
    delete header;

    vector<Code>* codes = read_lzwfile_codes("a.lzw", 12);
    cout << "Print codes" << endl;
    for (int i = 0; i < codes->size(); i++) {
        cout << codes->at(i) << endl;
    }
    lzwfile_codes_writer writer("b.lzw", 12);
    writer.write(codes);
    writer.close();
    delete codes;

    assert(is_two_files_identical("a.lzw", "b.lzw"));

    cout << "Pass test cases for lzwfile" << endl;
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
    for (int i = 0; i < v2.size(); i++) {
        cout << "First subvector: " << endl;
        vector<int> subv = v2.at(i);
        for (int j = 0; j < subv.size(); j++) {
            cout << subv.at(j) << " ";
        }
        cout << endl;
    }
}

void
test_bitarray() {
    Bitarray b;
    b.push_bytes_back((unsigned char)0);
    Bytes byte = b.pop_byte_front();
    assert(byte.length() == 1);
    assert(byte.get(0) == 0);
    assert(b.length() == 0);
    b.push_bytes_back((unsigned char)0);
    byte = b.pop_bytes_front();
    assert(byte.length() == 1);
    assert(byte.get(0) == 0);
    assert(b.length() == 0);

    b = Bitarray::from_int(0, 12);
    assert(b.length() == 12);
    assert(b.pop_byte_front() == Bytes((unsigned char)0));

    cout << "Pass test cases for bitarray" << endl;
}

int
main() {
    try {
        // string s = int2str(9);
        // cout << s << endl;
        // s = int2str(0);
        // cout << s << endl;
        // s = int2str(7);
        // cout << s << endl;
        // s = int2str(190);
        // cout << s << endl;

        // char cs[] = {'1', '\0'};
        // string s(cs);
        // cout << (s == "1") << endl;

        // test_dict();
        // test_code_dict();
        // test_str_dict();
        // test_utils();
        // test_codec();
        // test_bitarray();
        // test_grouper();
        test_lzwfile();

        cout << "Pass all test cases!" << endl;
    } catch (exception e) {
        cout << "Catched exception in main\n";
        cout << e.what() << endl;
    }
    return 0;
}
