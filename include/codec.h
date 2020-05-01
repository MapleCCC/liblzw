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
    LZWEncoder(unsigned code_bitsize);
    Code encode(Bytes byte);
    Code flush();

   private:
    CodeDict code_dict;
    Bytes prefix;
};

class FileEncoder {
   public:
    FileEncoder(unsigned code_bitsize);
    void encode_file(std::string filename, lzwfile_codes_writer& code_writer);

   private:
    LZWEncoder raw_encoder;
    Code virtual_eof;
};

class LZWDecoder {
   public:
    LZWDecoder(unsigned code_bitsize);
    Bytes decode(Code code);

   private:
    StrDict str_dict;
    Bytes prefix;
};

class FileDecoder {
   public:
    FileDecoder(unsigned code_bitsize);
    void decode_file(std::string filename, lzwfile_codes_reader& code_reader);

   private:
    LZWDecoder raw_decoder;
    Code virtual_eof;
};

#endif /* _CODEC_H_ */
