#ifndef _CODE_H_
#define _CODE_H_

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

#endif /* _CODE_H_ */
