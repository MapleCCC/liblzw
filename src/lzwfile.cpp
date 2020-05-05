#include "lzwfile.h"

#include <cstdio>  // EOF

#include "bitarray.h"
#include "bytes.h"
using namespace std;

static string
_readline(ifstream& f) {
    string line;
    if (getline(f, line)) {
        return line;
    } else {
        cerr << "getline failed" << endl;
        throw runtime_error("getline failed");
    }
}

vector<string>*
read_lzwfile_header(string lzwfile) {
    ifstream f(lzwfile, ios::in | ios::binary);
    if (!f.is_open()) {
        cerr << "Can't open file: " << lzwfile << endl;
        throw runtime_error("Can't open file: " + lzwfile);
    }

    vector<string>* header = new vector<string>;
    string line = _readline(f);
    while (!line.empty()) {
        header->push_back(line);
        line = _readline(f);
    }

    f.close();
    return header;
}

void
write_lzwfile_header(const string& lzwfile, const vector<string>* header) {
    // For now, we don't take care of the case of existing file. Should that
    // happen, we simply choose to fail early.
    ifstream ifs(lzwfile);
    if (ifs.good()) {
        cerr << "Can't write header to existent file" << endl;
        throw runtime_error("Can't write header to existent file");
    }
    ifs.close();

    ofstream ofs(lzwfile, ios::out | ios::binary | ios::trunc);
    if (!ofs.is_open()) {
        cerr << "Can't open file: " << lzwfile << endl;
        throw runtime_error("Can't open file: " + lzwfile);
    }

    for (unsigned i = 0; i < header->size(); i++) {
        ofs << header->at(i) << endl;
    }
    ofs << endl;
    ofs.close();
}

lzwfile_codes_reader::lzwfile_codes_reader(string lzwfile, unsigned code_size) {
    this->code_size = code_size;

    file_handle.open(lzwfile, ios::in | ios::binary);
    if (!file_handle.is_open()) {
        cerr << "Can't open file: " << lzwfile << endl;
        throw runtime_error("Can't open file: " + lzwfile);
    }

    // Skip through the header
    string line = _readline(file_handle);
    while (!line.empty()) {
        line = _readline(file_handle);
    }
}

lzwfile_codes_reader::~lzwfile_codes_reader() { file_handle.close(); }

Code
lzwfile_codes_reader::read() {
    while (buffer.length() < code_size) {
        //! Optimize Candidate: remove EOF check to accelerate performance.
        //! Tradeoff robustness for performance. Assume caller is consenting
        //! adult, and reposible for passing in valid arguments.
        if (file_handle.peek() == EOF) {
            cerr << "Can't read from lzwfile anymore. EOF reached." << endl;
            throw runtime_error(
                "Can't read from lzwfile anymore. EOF reached.");
        }

        unsigned char byte;
        /* WARNING: don't use >> operator. Instead, use get() method. Otherwise
           it will ignore bytes who when decoded to ASCII are whitespace
           characters */
        // f >> byte;
        byte = file_handle.get();
        buffer.push_bytes_back(Bytes(1, byte));
    }

    Code code = buffer.slice(code_size).to_int();
    buffer = buffer.slice(code_size, buffer.length());
    return code;
}

bool
lzwfile_codes_reader::eof() {
    // WARNING: don't use f.good() or f.eof(), as they only return error
    // when we have already read past the end of file. Stupid C!
    return file_handle.peek() == EOF;
}

lzwfile_codes_writer::lzwfile_codes_writer(string lzwfile, unsigned code_size) {
    this->code_size = code_size;

    // write from the end, assuming that the file already has header but not
    // body a.k.a. (i.e. codes)
    file_handle.open(lzwfile, ios::out | ios::binary | ios::app);
    if (!file_handle.is_open()) {
        cerr << "Can't open file: " << lzwfile << endl;
        throw runtime_error("Can't open file: " + lzwfile);
    }
}

lzwfile_codes_writer::~lzwfile_codes_writer() {
    if (buffer.length()) {
        //! Flush the leftover bits
        buffer.push_bytes_back(Bytes(1, (unsigned char)0));
        unsigned char byte = buffer.pop_byte_front();
        file_handle << byte;
    }
    file_handle.close();
}

void
lzwfile_codes_writer::write(Code code) {
    buffer += Bitarray::from_int(code, code_size);

    while (buffer.length() >= 8) {
        unsigned char byte = buffer.pop_byte_front();
        file_handle << byte;
    }
}
