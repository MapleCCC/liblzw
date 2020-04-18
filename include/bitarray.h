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
    Bit get(unsigned index) const;
    Bitarray slice(unsigned start, unsigned stop) const;
    Bitarray slice(unsigned start) const;
    unsigned length() const;
    Bitarray operator+(const Bitarray& rhs);
    int to_int();
    static Bitarray from_int(int x, int bit_size);
    void push_bytes_back(Bytes);
    Bytes pop_byte_front();
    Bytes pop_bytes_front(unsigned n = 1);
    std::string str() const;

   private:
    std::vector<Bit> storage;
};

std::ostream& operator<<(std::ostream& os, const Bitarray& ba);

#endif /* _BITARRAY_H_ */
