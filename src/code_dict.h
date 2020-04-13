#ifndef _CODE_DICT_H_
#define _CODE_DICT_H_

#include <string>

#include "bytes.h"
#include "code.h"
#include "dict.h"

class CodeDict {
   public:
    CodeDict(int code_bitsize);
    void clear();
    bool contains(Bytes);
    Code get(Bytes);
    void add_new_code(Bytes);
    std::string str() const;

   private:
    Dict<Bytes, Code> storage;
    int capacity;
    int size;
};

#endif /* _CODE_DICT_H_ */
