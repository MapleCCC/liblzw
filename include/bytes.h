#ifndef _BYTES_H_
#define _BYTES_H_

#include <functional>
#include <string>

typedef std::string Bytes;

namespace std {
template <>
struct hash<Bytes> {
    size_t operator()(const Bytes& b) { return string_hasher(b); }

   private:
    hash<string> string_hasher;
};
}  // namespace std

#endif /* _BYTES_H_ */
