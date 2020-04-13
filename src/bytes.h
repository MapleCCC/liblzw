#ifndef _BYTES_H_
#define _BYTES_H_

#include <string>
#include <vector>

// [DEPRECATED] Don't try to print Bytes, because it contains unsigned char
// data, some of which is non-printable. Thumb of rule is to not misuse binary
// data as ASCII text, although in some situations they are interchangeable. For
// safety, it's best to avoid doing that.

// Bytes type is merely a semantic thin abstraction over vector of unsigned
// chars.
class Bytes {
   public:
    Bytes() {}
    Bytes(const unsigned char);
    Bytes(const char*);
    Bytes(const std::string);
    unsigned char get(int index) const;
    int length() const { return storage.size(); }
    unsigned char get_first_byte() const;
    void push_back(unsigned char c);
    long long hash() const;
    bool operator==(const Bytes& rhs) const;
    bool operator!=(const Bytes& rhs) const;
    Bytes operator+(const Bytes& rhs) const;
    Bytes operator+(const unsigned char c);
    std::string str() const;
    // friend std::string operator+(const std::string& s, const Bytes&
    // bytes); friend std::ostream& operator<<(std::ostream& os, const
    // Bytes& bytes);

   private:
    std::vector<unsigned char> storage;
};

std::string operator+(const std::string& s, const Bytes& bytes);
std::ostream& operator<<(std::ostream& os, const Bytes& bytes);

void ENABLE_BYTES_HASH_CACHE();
void DISABLE_BYTES_HAHS_CACHE();

#endif /* _BYTES_H_ */
