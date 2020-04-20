#ifndef _CODE_H_
#define _CODE_H_

#include <functional>
#include <iostream>

// Code type is merely a thin abstraction over int type
class Code {
   public:
    Code(int x = 0) : x(x) {}
    // no setter method. Immutable type.
    int unbox() const { return x; }
    long hash() const;
    bool operator==(const Code&) const;
    bool operator!=(const Code&) const;
    std::string str() const;

   private:
    int x;
};

std::ostream& operator<<(std::ostream& os, const Code& code);

namespace std {
template <>
struct hash<Code> {
    size_t operator()(const Code c) { return c.hash(); }
};
}  // namespace std

#endif /* _CODE_H_ */
