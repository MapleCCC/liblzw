#ifndef _BYTES_H_
#define _BYTES_H_

#include <string>
#include <vector>

// [DEPRECATED] Don't try to print Bytes, because it contains unsigned char
// data, some of which is non-printable. Thumb of rule is to not misuse binary
// data as ASCII text, although in some situations they are interchangeable. For
// safety, it's best to avoid doing that.

// TODO: implement move semantic to improve performance in many situations

// Bytes type is merely a semantic thin abstraction over vector of unsigned
// chars.
class Bytes {
   public:
    Bytes();
    Bytes(unsigned char);
    Bytes(const Bytes&);
    unsigned char get(unsigned index) const;
    unsigned length() const;
    unsigned char get_first_byte() const;
    void push_back(unsigned char c);
    long long hash() const;
    Bytes& operator=(const Bytes&);
    bool operator==(const Bytes& rhs) const;
    bool operator!=(const Bytes& rhs) const;
    Bytes operator+(const Bytes& rhs) const;
    Bytes operator+(const unsigned char c);
    std::string str() const;

   private:
    std::vector<unsigned char> storage;
    long long hashcode;

    void init();
    void update_hashcode(unsigned char);
};

std::ostream& operator<<(std::ostream& os, const Bytes& bytes);

inline unsigned
Bytes::length() const {
    return storage.size();
}

#endif /* _BYTES_H_ */
