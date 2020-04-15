#ifndef _CONVERTER_H_
#define _CONVERTER_H_

#include <vector>

#include "bit.h"
#include "bytes.h"

std::vector<Bit> bytes2bits(const Bytes& bytes);
Bytes bits2bytes(const std::vector<Bit>& bits);
int bits2int(const std::vector<Bit>& bits);
std::vector<Bit> int2bits(int x);

#endif /* _CONVERTER_H_ */
