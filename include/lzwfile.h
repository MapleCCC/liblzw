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

class lzwfile_codes_reader {
   public:
    lzwfile_codes_reader(std::string lzwfile, unsigned code_size);
    Code read();
    bool eof();
    void close();

   private:
    Bitarray buffer;
    std::ifstream file_handle;
    unsigned code_size;
};

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
