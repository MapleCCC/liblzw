#ifndef _STR_DICT_H_
#define _STR_DICT_H_

#include <string>
#include <unordered_map>

#include "bytes.h"
#include "code.h"

class StrDict {
   public:
    StrDict(int code_bitsize);
    // ~StrDict() { storage.statistics(); }
    void clear();
    bool contains(Code);
    Bytes get(Code);
    void add_new_str(Bytes);
    std::string str() const;

   private:
    std::unordered_map<Code, Bytes> storage;
    int capacity;
    int size;

    void check_duplicate_str();
};

#endif /* _STR_DICT_H_ */
