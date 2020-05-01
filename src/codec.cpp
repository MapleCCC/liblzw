#include "codec.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;

LZWEncoder::LZWEncoder(unsigned code_bitsize) : code_dict(code_bitsize) {}

FileEncoder::FileEncoder(unsigned code_bitsize) : raw_encoder(code_bitsize) {
    virtual_eof = (1 << code_bitsize) - 1;
}

void
FileEncoder::encode_file(string filename, lzwfile_codes_writer& code_writer) {
    ifstream fp(filename.c_str(), ios::in | ios::binary);
    if (!fp.is_open()) {
        cerr << "Can't open file: " << filename << endl;
        throw runtime_error("Can't open file: " + filename);
    }

    raw_encoder.flush();
    Code code;
    Bytes byte;
    while (fp.peek() != EOF) {
        byte = Bytes(1, (unsigned char)fp.get());
        if ((code = raw_encoder.encode(byte)) != -1) {
            code_writer.write(code);
        }
    }
    if ((code = raw_encoder.flush()) != -1) {
        code_writer.write(code);
    }

    code_writer.write(virtual_eof);
}

Code
LZWEncoder::encode(Bytes byte) {
    Code ret = -1;
    // TODO: for lookup and retrieve from code dict, use stateful hash
    // algorithm object, to reduce hash recomputation cost.
    // This optimization could be done in Bytes class implementation level.
    Bytes current_word = prefix + byte;
    if (code_dict.contains(current_word)) {
        prefix = current_word;
    } else {
        ret = code_dict.get(prefix);
        code_dict.add_new_code(current_word);
        prefix = byte;
    }
    return ret;
}

Code
LZWEncoder::flush() {
    Code ret = -1;
    if (prefix.length()) {
        ret = code_dict.get(prefix);
        prefix.clear();
    }
    return ret;
}

FileDecoder::FileDecoder(unsigned code_bitsize) : raw_decoder(code_bitsize) {
    virtual_eof = (1 << code_bitsize) - 1;
}

void
FileDecoder::decode_file(std::string filename,
                         lzwfile_codes_reader& code_reader) {
    ofstream f(filename.c_str(), ios::out | ios::binary);
    if (!f.is_open()) {
        cerr << "Can't open file: " << filename << endl;
        throw runtime_error("Can't open file: " + filename);
    }

    raw_decoder.flush();
    Code code;
    while ((code = code_reader.read()) != virtual_eof) {
        f << raw_decoder.decode(code);
    }
    raw_decoder.flush();

    f.close();
}

LZWDecoder::LZWDecoder(unsigned code_bitsize) : str_dict(code_bitsize) {}

Bytes
LZWDecoder::decode(Code code) {
    if (str_dict.contains(code)) {
        Bytes suffix = str_dict.get(code);
        if (prefix.length()) {
            str_dict.add_new_str(prefix + suffix[0]);
        }
        prefix = suffix;
        return suffix;
    } else {
        Bytes current_word = prefix + prefix[0];
        str_dict.add_new_str(current_word);
        prefix = current_word;
        return current_word;
    }
}

void
LZWDecoder::flush() {
    prefix.clear();
}
