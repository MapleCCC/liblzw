#include "utils.tpp"

#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

string
int2str(int x) {
    ostringstream ss;
    ss << x;
    // WARNING: ostringstream.str() returns a temporary object that will be
    // destructed at the end of the expression
    string s = ss.str();
    return s;
}

string
byte2hex(unsigned char c) {
    string ret;
    char first_hex_digit;
    char second_hex_digit;
    int temp = (int)c >> 4;
    if (temp <= 9) {
        first_hex_digit = (char)(temp + 48);
    } else {
        first_hex_digit = (char)(temp - 9 - 1 + 65);
    }
    temp = (int)c & 15;
    if (temp < 10) {
        second_hex_digit = (char)(temp + 48);
    } else {
        second_hex_digit = (char)(temp - 10 + 65);
    }
    ret.push_back(first_hex_digit);
    ret.push_back(second_hex_digit);
    return ret;
}

bool
is_two_files_identical(string filename1, string filename2) {
    ifstream f1(filename1, ios::in | ios::binary | ios::ate);
    int file_size1 = f1.tellg();
    char* content1 = new char[file_size1];
    f1.seekg(0, ios::beg);
    f1.read(content1, file_size1);

    ifstream f2(filename2, ios::in | ios::binary | ios::ate);
    int file_size2 = f2.tellg();
    char* content2 = new char[file_size2];
    f2.seekg(0, ios::beg);
    f2.read(content2, file_size2);

    if (file_size1 != file_size2) {
        return false;
    }

    for (int i = 0; i < file_size1; i++) {
        if (content1[i] != content2[i]) {
            return false;
        }
    }
    return true;
}
