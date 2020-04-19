#ifndef _BITARRAY_H_
#define _BITARRAY_H_

#include <iostream>
#include <vector>

#include "bigint.h"
#include "bit.h"
#include "bytes.h"

class Bitarray {
   public:
    Bitarray();
    Bitarray slice(unsigned start, unsigned stop) const;
    Bitarray slice(unsigned stop) const;
    unsigned length() const;
    Bitarray& operator+=(const Bitarray& rhs);
    int to_int();
    static Bitarray from_int(int x, int bit_size);
    void push_bytes_back(Bytes);
    unsigned char pop_byte_front();
    std::string str() const;

   private:
    BigInt x;
    unsigned size;
};

std::ostream& operator<<(std::ostream& os, const Bitarray& ba);

#endif /* _BITARRAY_H_ */
