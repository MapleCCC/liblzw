#ifndef _CODE_DICT_H_
#define _CODE_DICT_H_

#include <string>

#include "bytes.h"
#include "code.h"
#include "dict.tpp"

class CodeDict {
   public:
    CodeDict(unsigned code_bitsize);
    // ~CodeDict() { storage.statistics(); }
    void clear();
    bool contains(Bytes);
    Code get(Bytes);
    void add_new_code(Bytes);
    std::string str() const;

   private:
    Dict<Bytes, Code> storage;
    unsigned capacity;
    unsigned size;
};

#endif /* _CODE_DICT_H_ */
