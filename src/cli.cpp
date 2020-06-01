#include "cli.h"

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "internal.h"

using namespace std;

int cli_compress(int argc, char** argv);
int cli_decompress(int argc, char** argv);

int
cli(int argc, char** argv) {
    if (argc <= 1) {
        cerr << "Too few arguments." << endl;
        return EXIT_FAILURE;
    }

    string subcommand(argv[1]);

    if (subcommand == "compress") {
        return cli_compress(argc, argv);
    } else if (subcommand == "decompress") {
        return cli_decompress(argc, argv);
    } else if (subcommand == "-h" || subcommand == "--help") {
        cout << "Usage:" << endl
             << "" << endl
             << "# Compression" << endl
             << "$ lzw compress [-o|--output <ARCHIVE>] <FILES>..." << endl
             << "" << endl
             << "# Decompression" << endl
             << "$ lzw decompress <ARCHIVE> " << endl
             << endl;
    } else {
        cerr << "Invalid arguments" << endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int
cli_compress(int argc, char** argv) {
    string lzwfile;
    vector<string>* header = new vector<string>;
    for (int i = 2; i < argc; i++) {
        if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output")) {
            if (i == argc - 1) {
                cerr << "-o|--output option present but no value specified"
                     << endl;
                return EXIT_FAILURE;
            }
            lzwfile = string(argv[++i]);
        } else {
            header->push_back(string(argv[i]));
        }
    }

    if (lzwfile.empty()) {
        lzwfile = string("compressed");
    }

    try {
        compress(lzwfile, header);
    } catch (exception e) {
        cerr << "During compression, catched exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }

    delete header;

    return EXIT_SUCCESS;
}

int
cli_decompress(int argc, char** argv) {
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

    return EXIT_SUCCESS;
}
