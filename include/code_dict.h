#ifndef _CODE_DICT_H_
#define _CODE_DICT_H_

#include <string>
#include <unordered_map>

#include "bytes.h"
#include "code.h"

class CodeDict {
   public:
    CodeDict(int code_bitsize);
    // ~CodeDict() { storage.statistics(); }
    void clear();
    bool contains(Bytes);
    Code get(Bytes);
    void add_new_code(Bytes);
    std::string str() const;

   private:
    std::unordered_map<Bytes, Code> storage;
    int capacity;
    int size;
};

#endif /* _CODE_DICT_H_ */
