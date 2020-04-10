#include "converter.h"

#include <iostream>
#include <stdexcept>

#include "utils.h"
using namespace std;

vector<Bit>
bytes2bits(const Bytes& bytes) {
    vector<Bit> ret;
    for (int i = 0; i < bytes.length(); i++) {
        unsigned char byte = bytes.get(i);
        int x = (int)byte;
        vector<Bit> bits = int2bits(x);
        // FIXME: why is here problem?
        // for (int j = 0; j < (8-bits.size()); j++) {
        int pad_zero_num = 8 - bits.size();
        for (int j = 0; j < pad_zero_num; j++) {
            bits.insert(bits.begin(), ZERO);
        }
        ret.insert(ret.end(), bits.begin(), bits.end());
    }
    return ret;
}

Bytes
bits2bytes(const std::vector<Bit>& bits) {
    if (bits.size() % 8) {
        cerr << "bits number need to be multiple of 8" << endl;
        throw runtime_error("bits number need to be multiple of 8");
    }
    Bytes ret;
    vector<vector<Bit> > eight_bits = grouper(bits, 8);
    for (int i = 0; i < eight_bits.size(); i++) {
        vector<Bit> eight_bit = eight_bits.at(i);
        Bytes byte((unsigned char)bits2int(eight_bit));
        ret = ret + byte;
    }
    return ret;
}

int
bits2int(const std::vector<Bit>& bits) {
    int ret = 0;
    for (int i = 0; i < bits.size(); i++) {
        ret = ret * 2 + bits.at(i);
    }
    return ret;
}

vector<Bit>
int2bits(int x) {
    vector<Bit> ret;
    while (x != 0) {
        ret.push_back(x % 2);
        x = x / 2;
    }
    reverse(ret.begin(), ret.end());
    return ret;
}
