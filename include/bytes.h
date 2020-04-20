#ifndef _BYTES_H_
#define _BYTES_H_

#include <functional>
#include <string>
#include <vector>

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

    friend std::ostream& operator<<(std::ostream& os, const Bytes& bytes);

   private:
    std::vector<unsigned char> storage;
    long long hashcode;

    void update_hashcode(unsigned char);
};

namespace std {
template <>
struct hash<Bytes> {
    size_t operator()(const Bytes& b) { return b.hash(); }
};
}  // namespace std

inline unsigned
Bytes::length() const {
    return storage.size();
}

#endif /* _BYTES_H_ */
