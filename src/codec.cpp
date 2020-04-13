#include "codec.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;

LZWEncoder::LZWEncoder(int code_bitsize) : code_dict(code_bitsize) {
    virtual_eof = pow(2, code_bitsize) - 1;
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
        cout << "Encoding the " << i << "th text" << endl;
        // TODO: for lookup and retrieve from code dict, use stateful hash
        // algorithm object, to reduce hash recomputation cost.
        // This optimization could be done in Bytes class implementation level.
        Bytes current_word = prefix + text[i];
        cout << "current_word: " << current_word << endl;
        cout << "current code dict: " << code_dict.str() << endl;
        if (code_dict.contains(current_word)) {
            cout << "enter contained branch" << endl;
            prefix = current_word;
        } else {
            cout << "enter not contained branch" << endl;
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

LZWDecoder::LZWDecoder(int code_bitsize) : str_dict(code_bitsize) {
    virtual_eof = pow(2, code_bitsize) - 1;
}

void
LZWDecoder::decode_file(std::string filename,
                        vector<Code>::iterator& codes_itr) {
    ofstream f(filename.c_str(), ios::out | ios::binary);
    if (!f.is_open()) {
        cerr << "Can't open file: " << filename << endl;
        throw runtime_error("Can't open file: " + filename);
    }

    vector<Bytes>* buf = _decode(codes_itr);
    for (int i = 0; i < buf->size(); i++) {
        Bytes bytes = buf->at(i);
        for (int i = 0; i < bytes.length(); i++) {
            f << bytes.get(i);
        }
    }
    f.close();
    delete buf;
}

vector<Bytes>*
LZWDecoder::_decode(vector<Code>::iterator& codes_itr) {
    vector<Bytes>* ret = new vector<Bytes>;
    // TODO: add heuristic: reserve reasonable space for vector, so as to reduce
    // overhead of resizing
    ENABLE_BYTES_HASH_CACHE();
    Bytes prefix;
    for (;; codes_itr++) {
        Code code = *codes_itr;
        if (code == virtual_eof) {
            codes_itr++;
            break;
        }

        if (str_dict.contains(code)) {
            Bytes sufix = str_dict.get(code);
            if (prefix.length()) {
                str_dict.add_new_str(prefix + sufix.get_first_byte());
            }
            prefix = sufix;
            ret->push_back(sufix);
        } else {
            Bytes current_word = prefix + prefix.get_first_byte();
            str_dict.add_new_str(current_word);
            ret->push_back(current_word);
            prefix = current_word;
        }
    }
    DISABLE_BYTES_HAHS_CACHE();
    return ret;
}