#ifndef _CODE_DICT_H_
#define _CODE_DICT_H_

#include <string>
#include <unordered_map>

#include "bytes.h"
#include "code.h"

class CodeDict {
   public:
    CodeDict(unsigned code_bitsize);
    void clear();
    bool contains(Bytes);
    Code get(Bytes);
    void add_new_code(Bytes);
    std::string str() const;
    std::string statistics() const;

   private:
    std::unordered_map<Bytes, Code> storage;
    unsigned capacity;
    unsigned size;
};

#endif /* _CODE_DICT_H_ */
