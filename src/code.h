#ifndef _CODE_H_
#define _CODE_H_

#include <iostream>

// Code type is merely a thin abstraction over int type
class Code {
   public:
    Code(int x = 0) : x(x) {}
    // no setter method. Immutable type.
    int get() const { return x; }
    long hash() const;
    bool operator==(const Code&) const;
    bool operator==(const int) const;
    operator int() const;
    operator std::string()
        const;  // so that static_cast<string>() is applicable to Code type

    // friend bool operator==(const int lhs, const Code& rhs);
    // friend std::ostream& operator<<(std::ostream& os, const Code& code);

   private:
    int x;
};

bool operator==(const int lhs, const Code& rhs);
std::ostream& operator<<(std::ostream& os, const Code& code);
std::string operator+(const std::string s, const Code code);

#endif /* _CODE_H_ */
