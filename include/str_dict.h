#ifndef _STR_DICT_H_
#define _STR_DICT_H_

#include <string>

#include "bytes.h"
#include "code.h"
#include "dict.tpp"

class StrDict {
   public:
    StrDict(unsigned code_bitsize);
    // ~StrDict() { storage.statistics(); }
    void clear();
    bool contains(Code);
    Bytes get(Code);
    void add_new_str(Bytes);
    std::string str() const;

   private:
    Dict<Code, Bytes> storage;
    unsigned capacity;
    unsigned size;

    void check_duplicate_str();
};

#endif /* _STR_DICT_H_ */
