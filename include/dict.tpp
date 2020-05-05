#ifndef _DICT_H_
#define _DICT_H_

#include <functional>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "bitmap.h"
#include "extra_except.h"

// WARNING: template declaration and definition can't be separated into two
// files.

// Should we use size_t to replace unsigned type in some places?

// Initial capacity is power of two. Everytime resize to twice as larger.
// Guarantee that capacity is always power of two, which is useful when
// calculating remainder of hash value of keys.
#define INIT_DICT_CAPCITY 8
#define DICT_RESIZE_FACTOR \
    2  // Optionally we can change the value of DICT_RESIZE_FACTOR in a dynamic
       // manner
#define MAX_LOAD_FACTOR (2 * 1.0 / 3)
#define PERTURB_SHIFT 5

template <class T, class S>
struct Bucket {
    T key;
    S value;
};

// Dict class implements associative array that support T type as key and S type
// as value.
template <class T, class S>
class Dict {
   public:
    Dict();
    ~Dict();
    int length() const;
    double load_factor() const;
    void clear();
    void reserve(unsigned);
    void set(const T& key, const S& value);
    S get(const T& key) const;
    bool contains(const T& key) const;
    bool contains_value(const S& value) const;
    std::string str() const;
    std::vector<T> keys() const;
    std::vector<S> values() const;

    // void statistics() const;

   private:
    unsigned size;
    unsigned capacity;
    Bucket<T, S>* slots;
    Bitmap indexer;
    std::hash<T> hasher;

    void resize(unsigned new_capacity);
    unsigned find_bucket(const T& key) const;

    // class KeyView {
    //     public:
    //     KeyView();
    //     private:

    // };
    // class ValueView {
    //     public:
    //     ValueView();
    //     private:

    // };
};

template <class T, class S>
Dict<T, S>::Dict() : indexer(INIT_DICT_CAPCITY) {
    size = 0;
    capacity = INIT_DICT_CAPCITY;
    slots = new Bucket<T, S>[capacity];
}

template <class T, class S>
Dict<T, S>::~Dict() {
    delete[] slots;
}

template <class T, class S>
inline int
Dict<T, S>::length() const {
    return size;
}

template <class T, class S>
inline double
Dict<T, S>::load_factor() const {
    return size * 1.0 / capacity;
}

static inline bool
is_power_of_2(unsigned x) {
    return (x & (x - 1)) == 0;
}

// Everytime resize to twice as larger. Guarantee that capacity is always power
// of two, which is useful when calculating remainder of hash value of keys.
template <class T, class S>
void
Dict<T, S>::resize(unsigned new_capacity) {
    if (new_capacity <= size) {
        std::cerr
            << "Only allowed to resize to new size that's greater than the "
               "original size\n";
        throw std::runtime_error(
            "Only allowed to resize to new size that's greater than the "
            "original size");
    }
    if (new_capacity <= capacity) {
        return;
    }
    if (!is_power_of_2(new_capacity)) {
        std::cerr << "Dict capacity has to be power of 2" << std::endl;
        throw std::runtime_error("Dict capacity has to be power of 2");
    }

    Bucket<T, S>* old_slots = slots;
    Bitmap old_indexer = indexer;
    unsigned old_capacity = capacity;

    slots = new Bucket<T, S>[new_capacity];
    capacity = new_capacity;
    size = 0;
    indexer.reset();
    indexer.resize(new_capacity);

    for (unsigned i = 0; i < old_capacity; i++) {
        if (old_indexer.get(i)) {
            set(old_slots[i].key, old_slots[i].value);
        }
    }

    delete[] old_slots;
}

// Should we resize the dict to smaller memory when clear() is called?
// Consult vector implementation:
// https://stackoverflow.com/questions/18467624/what-does-the-standard-say-about-how-calling-clear-on-a-vector-changes-the-capac/18467916#18467916
template <class T, class S>
void
Dict<T, S>::clear() {
    // Empty the bitmap
    indexer.reset();
    size = 0;
}

static unsigned
ceil_power_of_2(unsigned x) {
    unsigned ret = 1;
    while (ret < x) {
        ret <<= 1;
    }
    return ret;
}

template <class T, class S>
void
Dict<T, S>::reserve(unsigned size) {
    // Resize to new capacity that can, while maintaining ideal load factor,
    // contain enough number of elements as specified in the argument.
    // Note that current implementation design of dict is to ensure that
    // capacity is always power of 2, so as to make it more efficient when
    // conducting search operation.
    unsigned new_capacity = ceil_power_of_2(ceil(size * 1.0 / MAX_LOAD_FACTOR));
    resize(new_capacity);
}

template <class T, class S>
unsigned
Dict<T, S>::find_bucket(const T& key) const {
    // We can be assured that the recurrence is going to iterate through all
    // slots. As long as that size < capacity, we will not go into infinite
    // loop.
    unsigned mask = capacity - 1;
    unsigned h = hasher(key);
    unsigned perturb = h;
    unsigned index = h & mask;
    while (indexer.get(index) && slots[index].key != key) {
        perturb = perturb >> PERTURB_SHIFT;
        index = (5 * index + 1 + perturb) & mask;
    }
    // Either, can't find the bucket containing key, return next probe position.
    // Or, found the bucket containing the key, return the bucket's index
    // position.
    return index;
}

template <class T, class S>
void
Dict<T, S>::set(const T& key, const S& value) {
    unsigned index = find_bucket(key);
    if (indexer.get(index)) {
        slots[index].value = value;
    } else {
        slots[index].key = key;
        slots[index].value = value;
        size++;
        indexer.set(index, 1);

        if (load_factor() > MAX_LOAD_FACTOR) {
            resize(capacity * DICT_RESIZE_FACTOR);
        }
    }
}

template <class T, class S>
S
Dict<T, S>::get(const T& key) const {
    unsigned index = find_bucket(key);
    if (indexer.get(index)) {
        return slots[index].value;
    } else {
        std::cerr << "Key not present in the dict" << std::endl;
        throw KeyError("Key not present in the dict");
    }
}

// If membership test is heavy operation, we can optionally use bloom filter
// to accelerate.
template <class T, class S>
inline bool
Dict<T, S>::contains(const T& key) const {
    return indexer.get(find_bucket(key));
}

// Currently a naive linear search. May be expensive and inefficient for
// large dict. Consider use another O(1) implementation. Trade space for
// time. More optionally, we can trade not that much space for not that much
// time, use probablistic data structure like bloom filter. If we can't find
// it in bloom filter, it must not appear before. If we can find it in bloom
// filter, we need to iterate over the slots to check if it really no
// appears. The problem of using hashing-like data structure is that the
// dict interface doesn't require value type to be hashable. Adding
// additional requirements that the value type is hashable will strengthen
// the parameter constraints and limit the use case, which is not desirable
// for general purpose data structure. If reducing cost of contains_value is
// really wanted, it could be done in upper level. The user of dict who also
// knows the hashability of value type can maintain a separate data
// structure itself to cache the values.
template <class T, class S>
bool
Dict<T, S>::contains_value(const S& value) const {
    for (unsigned i = 0; i < capacity; i++) {
        if (indexer.get(i) && slots[i].value == value) {
            return true;
        }
    }
    return false;
}

// // FIXME
// template <class T, class S>
// void
// Dict<T, S>::statistics() const {
// #if COLLECT_STATISTICS_MODE == 1
//     for (int i = 0; i < instance_count; i++) {
//         cout << "Size: " << size_statistics.at(instance_count)
//              << ", Capacity: " << capacity_statistics.at(instance_count)
//              << ", Collisions: " << collision_statistics.at(instance_count)
//              << endl;
//     }
// #else
//     std::cout << "Collect-statistics-mode is not enabled" << std::endl;
// #endif
// }

template <class T, class S>
std::string
Dict<T, S>::str() const {
    std::string ret;
    std::stringstream ss;
    ret += "{";
    for (unsigned i = 0; i < capacity; i++) {
        if (indexer.get(i)) {
            ss << slots[i].key << ":" << slots[i].value << ", ";
            ret += ss.str();
        }
    }
    if (size) {
        ret.erase(ret.end() - 2, ret.end());
    }
    ret += "}";
    return ret;
}

// TODO: provide view-like object.
// TODO: Set like.
// TODO: Insertion order.
template <class T, class S>
std::vector<T>
Dict<T, S>::keys() const {
    std::vector<T> ret;
    for (unsigned i = 0; i < capacity; i++) {
        if (indexer.get(i)) {
            ret.push_back(slots[i].key);
        }
    }
    return ret;
}

// TODO: provide view-like object.
// TODO: Insertion order.
template <class T, class S>
std::vector<S>
Dict<T, S>::values() const {
    std::vector<S> ret;
    for (unsigned i = 0; i < capacity; i++) {
        if (indexer.get(i)) {
            ret.push_back(slots[i].value);
        }
    }
    return ret;
}

#endif /* _DICT_H_ */
