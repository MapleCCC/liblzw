#include "internal.h"

#include "codec.h"
#include "config.h"
#include "lzwfile.h"

using namespace std;

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
