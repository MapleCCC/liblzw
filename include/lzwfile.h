#ifndef _LZWFILE_H_
#define _LZWFILE_H_

#include <fstream>
#include <string>
#include <vector>

#include "bitarray.h"
#include "code.h"

// Caller is reponsible to remember to delete the returned vector
std::vector<std::string>* read_lzwfile_header(std::string lzwfile);
void write_lzwfile_header(const std::string& lzwfile,
                          const std::vector<std::string>* header);
// Caller is reponsible to remember to delete the returned vector
std::vector<Code>* read_lzwfile_codes(std::string lzwfile, int code_size);

class lzwfile_codes_writer {
   public:
    lzwfile_codes_writer(const std::string& lzwfile, int code_size);
    void write(const std::vector<Code>* codes);
    void close();

   private:
    Bitarray buffer;
    std::ofstream file_handle;
    int code_size;
};

#endif /* _LZWFILE_H_ */