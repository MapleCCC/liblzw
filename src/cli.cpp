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

    if (!strcmp(argv[1], "-c")) {
        if (argc < 4) {
            cerr << "Too few arguments." << endl;
            return EXIT_FAILURE;
        }
        string lzwfile(argv[2]);
        vector<string>* header = new vector<string>;
        for (int i = 0; i < argc - 3; i++) {
            header->push_back(string(argv[i + 3]));
        }
        try {
            compress(lzwfile, header);
            delete header;
        } catch (exception e) {
            cerr << "During compression, catched exception: " << e.what()
                 << endl;
            return EXIT_FAILURE;
        }
    } else if (!strcmp(argv[1], "-d")) {
        if (argc != 3) {
            cerr << "Too many arguments." << endl;
            return EXIT_FAILURE;
        }
        string lzwfile(argv[2]);
        try {
            decompress(lzwfile);
        } catch (exception e) {
            cerr << "During compression, catched exception: " << e.what()
                 << endl;
            return EXIT_FAILURE;
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
