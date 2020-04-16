#ifndef _STR_DICT_H_
#define _STR_DICT_H_

#include "bytes.h"
#include "code.h"
#include "dict.h"
#include "set.h"

class StrDict {
   public:
    StrDict(int code_bitsize);
    void clear();
    bool contains(Code);
    Bytes get(Code);
    void add_new_str(Bytes);

   private:
    Dict<Code, Bytes> storage;
    int capacity;
    int size;
    Set<Bytes> str_cache;
};

#endif /* _STR_DICT_H_ */
