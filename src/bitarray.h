#ifndef _BITARRAY_H_
#define _BITARRAY_H_

#include <iostream>
#include <vector>

#include "bit.h"
#include "bytes.h"

class Bitarray {
   public:
    Bitarray() {}
    Bitarray(const std::vector<Bit>& bits);
    Bit get(int index) const;
    Bitarray slice(int start, int stop);
    Bitarray slice(int start);
    int length() const;
    Bitarray operator+(const Bitarray& rhs);
    int to_int();
    static Bitarray from_int(int x, int bit_size);
    void push_bytes_back(Bytes);
    Bytes pop_byte_front();
    Bytes pop_bytes_front(int n = 1);
    operator std::string() const;

   private:
    std::vector<Bit> storage;
};

std::ostream& operator<<(std::ostream& os, const Bitarray& ba);

#endif /* _BITARRAY_H_ */
