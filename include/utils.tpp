#ifndef _UTILS_H_
#define _UTILS_H_

#include <string>
#include <vector>

std::string int2str(int);
std::string byte2hex(unsigned char);
bool is_two_files_identical(std::string filename1, std::string filename2);
template <class T>
std::vector<std::vector<T> > grouper(std::vector<T> v, int n);
template <class T>
std::string stringfy_vector(std::vector<T> v);
std::string raw_str_repr(std::string s);

// Discard leftover elements
// Alternative implementation is to padded with user-supplied fill value
template <class T>
std::vector<std::vector<T> >
grouper(std::vector<T> v, int n) {
    std::vector<std::vector<T> > ret;
    int iteration = v.size() / n;
    for (int i = 0; i < iteration; i++) {
        std::vector<T> subvector;
        for (int j = 0; j < n; j++) {
            subvector.push_back(v.at(i * n + j));
        }
        ret.push_back(subvector);
    }
    return ret;
}

template <class T>
std::string
stringfy_vector(std::vector<T> v) {
    std::string ret("[");
    for (unsigned i = 0; i < v.size(); i++) {
        ret += static_cast<std::string>(v.at(i));
        if (i != v.size() - 1) {
            ret += ", ";
        }
    }
    ret += "]";
    return ret;
}

#endif /* _UTILS_H_ */
