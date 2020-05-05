#ifndef _STR_DICT_H_
#define _STR_DICT_H_

#include <string>
#include <unordered_map>

#include "bytes.h"
#include "code.h"

class StrDict {
   public:
    StrDict(unsigned code_bitsize);
    void clear();
    bool contains(Code) const;
    Bytes get(Code) const;
    void add_new_str(Bytes);
    std::string str() const;
    std::string statistics() const;

   private:
    std::unordered_map<Code, Bytes> storage;
    unsigned capacity;
    unsigned size;

    void check_duplicate_str();
};

#endif /* _STR_DICT_H_ */
