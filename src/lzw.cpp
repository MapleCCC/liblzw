#include <iostream>
#include <vector>

#include "code.h"
#include "codec.h"
#include "lzwfile.h"
#include "utils.h"
using namespace std;

#define CODE_BITSIZE 12

void compress(const string& lzwfile, const vector<string>* header);
void decompress(const string& lzwfile);

int
main(int argc, char** argv) {
    if (argc <= 2) {
        cerr << "Too few arguments." << endl;
        exit(EXIT_FAILURE);
    }

    if (!strcmp(argv[1], "-c")) {
        if (argc < 4) {
            cerr << "Too few arguments." << endl;
            exit(EXIT_FAILURE);
        }
        string lzwfile(argv[2]);
        vector<string>* header = new vector<string>;
        for (int i = 0; i < argc - 3; i++) {
            header->push_back(string(argv[i + 3]));
        }
        compress(lzwfile, header);
    } else if (!strcmp(argv[1], "-d")) {
        if (argc != 3) {
            cerr << "Too many arguments." << endl;
            exit(EXIT_FAILURE);
        }
        string lzwfile(argv[2]);
        decompress(lzwfile);
    } else {
        cerr << "Invalid arguments" << endl;
        exit(EXIT_FAILURE);
    }

    return 0;
}

void
compress(const string& lzwfile, const vector<string>* header) {
    write_lzwfile_header(lzwfile, header);
    LZWEncoder encoder(CODE_BITSIZE);
    lzwfile_codes_writer writer(lzwfile, CODE_BITSIZE);
    for (int i = 0; i < header->size(); i++) {
        vector<Code>* codes = encoder.encode_file(header->at(i));
        writer.write(codes);
        delete codes;
    }
    writer.close();
}

void
decompress(const string& lzwfile) {
    vector<string>* header = read_lzwfile_header(lzwfile);
    LZWDecoder decoder(CODE_BITSIZE);
    vector<Code>* codes = read_lzwfile_codes(lzwfile, CODE_BITSIZE);
    vector<Code>::iterator codes_itr = codes->begin();
    for (int i = 0; i < header->size(); i++) {
        decoder.decode_file(header->at(i), codes_itr);
    }
    delete header;
    delete codes;
}
