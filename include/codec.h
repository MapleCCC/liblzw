#ifndef _CODEC_H_
#define _CODEC_H_

#include <string>
#include <vector>

#include "bytes.h"
#include "code.h"
#include "code_dict.h"
#include "lzwfile.h"
#include "str_dict.h"

class LZWEncoder {
   public:
    LZWEncoder(int code_bitsize);
    // Caller is responsible to remember to delete the returned vector
    std::vector<Code>* encode_file(std::string filename);

   private:
    CodeDict code_dict;
    int virtual_eof;
    // Caller is responsible to remember to delete the returned vector
    std::vector<Code>* _encode(unsigned char* text, int text_size);
};

class LZWDecoder {
   public:
    LZWDecoder(unsigned code_bitsize);
    Bytes decode(Code code);
    void decode_file(std::string filename, lzwfile_codes_reader& code_reader);

   private:
    StrDict str_dict;
    Bytes prefix;
    Code virtual_eof;
};

#endif /* _CODEC_H_ */
