#include "codec.h"

#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;

LZWEncoder::LZWEncoder(int code_bitsize) : code_dict(code_bitsize) {
    virtual_eof = (1 << code_bitsize) - 1;
}

vector<Code>*
LZWEncoder::encode_file(string filename) {
    ifstream f(filename.c_str(), ios::in | ios::binary | ios::ate);
    if (!f.is_open()) {
        cerr << "Can't open file: " << filename << endl;
        throw runtime_error("Can't open file: " + filename);
    }
    int file_size = f.tellg();
    unsigned char* buf = new unsigned char[file_size];
    f.seekg(0, ios::beg);
    // It's strange that C++ decides to use char instead of unsigned char to
    // store byte data. We use reinterpret_cast to workaround it.
    f.read(reinterpret_cast<char*>(buf), file_size);
    f.close();
    vector<Code>* ret = _encode(buf, file_size);
    ret->push_back(virtual_eof);
    delete[] buf;
    return ret;
}

vector<Code>*
LZWEncoder::_encode(unsigned char* text, int text_size) {
    Bytes prefix;
    vector<Code>* ret = new vector<Code>;
    // TODO: add heuristic: reserve reasonable space for vector, so as to reduce
    // overhead of resizing
    for (int i = 0; i < text_size; i++) {
        // TODO: for lookup and retrieve from code dict, use stateful hash
        // algorithm object, to reduce hash recomputation cost.
        // This optimization could be done in Bytes class implementation level.
        Bytes current_word = prefix + (char)text[i];
        if (code_dict.contains(current_word)) {
            prefix = current_word;
        } else {
            ret->push_back(code_dict.get(prefix));
            code_dict.add_new_code(current_word);
            prefix = text[i];
        }
    }
    if (prefix.length()) {
        ret->push_back(code_dict.get(prefix));
    }
    return ret;
}

void
LZWDecoder::decode_file(std::string filename,
                        lzwfile_codes_reader& code_reader) {
    ofstream f(filename.c_str(), ios::out | ios::binary);
    if (!f.is_open()) {
        cerr << "Can't open file: " << filename << endl;
        throw runtime_error("Can't open file: " + filename);
    }

    Code code;
    while ((code = code_reader.read()) != virtual_eof) {
        f << decode(code);
    }

    f.close();
}

LZWDecoder::LZWDecoder(unsigned code_bitsize) : str_dict(code_bitsize) {
    virtual_eof = (1 << code_bitsize) - 1;
}

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
