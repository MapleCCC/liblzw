#ifndef _CODE_H_
#define _CODE_H_

#include <functional>
#include <iostream>

typedef int Code;

namespace std {
template <>
struct hash<Code> {
    size_t operator()(const Code c) { return int_hasher(c); }

   private:
    hash<int> int_hasher;
};
}  // namespace std

#endif /* _CODE_H_ */
