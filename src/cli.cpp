#include "cli.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "code.h"
#include "codec.h"
#include "config.h"
#include "lzwfile.h"
using namespace std;

void compress(const string& lzwfile, const vector<string>* header);
void decompress(const string& lzwfile);

int
cli(int argc, char** argv) {
    if (argc <= 2) {
        cerr << "Too few arguments." << endl;
        return EXIT_FAILURE;
    }

    if (!strcmp(argv[1], "compress")) {
        string lzwfile;
        vector<string>* header = new vector<string>;
        for (int i = 2; i < argc; i++) {
            if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) {
                if (i == argc - 1) {
                    cerr << "-o|--output option present but no value specified" << endl;
                    return EXIT_FAILURE;
                }
                lzwfile = string(argv[++i]);
            }
            else {
                header->push_back(string(argv[i]));
            }
        }

        if (lzwfile.empty()) {
            lzwfile = string("compressed");
        }

        try {
            compress(lzwfile, header);
            delete header;
        } catch (exception e) {
            cerr << "During compression, catched exception: " << e.what()
                 << endl;
            return EXIT_FAILURE;
        }
    } else if (!strcmp(argv[1], "decompress")) {
        for (int i = 2; i < argc; i++) {
            string lzwfile(argv[i]);
            try {
                decompress(lzwfile);
            } catch (exception e) {
                cerr << "During compression, catched exception: " << e.what()
                    << endl;
                return EXIT_FAILURE;
            }
        }
    } else {
        cerr << "Invalid arguments" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

void
compress(const string& lzwfile, const vector<string>* header) {
    write_lzwfile_header(lzwfile, header);

    FileEncoder file_encoder(CODE_BITSIZE);
    lzwfile_codes_writer code_writer(lzwfile, CODE_BITSIZE);

    for (unsigned i = 0; i < header->size(); i++) {
        cout << "Compressing " << header->at(i) << endl;
        file_encoder.encode_file(header->at(i), code_writer);
    }
    cout << "Finish compression" << endl;
}

void
decompress(const string& lzwfile) {
    vector<string>* header = read_lzwfile_header(lzwfile);
    FileDecoder file_decoder(CODE_BITSIZE);
    lzwfile_codes_reader code_reader(lzwfile, CODE_BITSIZE);

    for (unsigned i = 0; i < header->size(); i++) {
        cout << "Deflating " << header->at(i) << endl;
        file_decoder.decode_file(header->at(i), code_reader);
    }

    delete header;
    cout << "Finish decompression" << endl;
}
