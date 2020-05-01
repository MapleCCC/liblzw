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
    ~lzwfile_codes_reader();
    Code read();
    bool eof();

   private:
    Bitarray buffer;
    std::ifstream file_handle;
    unsigned code_size;
};

class lzwfile_codes_writer {
   public:
    lzwfile_codes_writer(std::string lzwfile, unsigned code_size);
    ~lzwfile_codes_writer();
    void write(Code code);

   private:
    Bitarray buffer;
    std::ofstream file_handle;
    unsigned code_size;
};

#endif /* _LZWFILE_H_ */
