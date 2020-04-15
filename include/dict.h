#ifndef _DICT_H_
#define _DICT_H_

#include <cmath>
#include <exception>
#include <iostream>
#include <stdexcept>

#include "bitmap.h"

// TODO: add generic template
// WARNING: template declaration and definition can't be separated into two
// files.

// Should we use size_t to replace unsigned type in some places?

// Initial capacity is power of two. Everytime resize to twice as larger.
// Guarantee that capacity is always power of two, which is useful when
// calculating remainder of hash value of keys.
#define INIT_DICT_CAPCITY 8
#define DICT_RESIZE_FACTOR \
    2  // Optionally we can dynamically change the value of DICT_RESIZE_FACTOR
#define MAX_LOAD_FACTOR 2 * 1.0 / 3
#define PERTURB_SHIFT 5

// Should the macro COLLEFCT_STATISTICS_MODE defined in .cpp or .h?
#define COLLECT_STATISTICS_MODE 1

// Should we declare Bucket type inside Dict?
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
    void clear();
    void reserve(unsigned);
    void set(const T& key, const S& value);
    S get(const T& key) const;
    bool contains(const T& key) const;
    bool contains_value(const S& value) const;
    void statistics() const;
    std::string str() const;

   private:
    unsigned size;
    unsigned capacity;
    Bucket<T, S>* slots;
    Bitmap indexer;
    // Dict<S, NULL> values;
    // bool is_values_cached;
#if COLLECT_STATISTICS_MODE == 1
    int collision_count;
#endif

    void resize(unsigned new_capacity);
    double load_factor() const;
    unsigned find_bucket(const T& key) const;
};

template <class T, class S>
Dict<T, S>::Dict() : indexer(INIT_DICT_CAPCITY) {
    size = 0;
    capacity = INIT_DICT_CAPCITY;
    slots = new Bucket<T, S>[capacity];

#if COLLECT_STATISTICS_MODE == 1
    collision_count = 0;
#endif
}

template <class T, class S>
Dict<T, S>::~Dict() {
    /* Don't know why delete statement cause crash. Before we figure out why,
 let's just comment it out and be careful with the unfreed memory. */
    // delete[] slots;
}

// Get error, why?
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

// Everytime resize to twice as larger. Guarantee that capacity is always power
// of two, which is useful when calculating remainder of hash value of keys.
template <class T, class S>
void
Dict<T, S>::resize(unsigned new_capacity) {
    std::cout << "Enter dict resize routine: " << new_capacity << std::endl;
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

    std::cout << "5\n";
    Bucket<T, S>* new_slots = new Bucket<T, S>[new_capacity];
    for (unsigned i = 0; i < capacity; i++) {
        if (!indexer.get(i)) {
            continue;
        }
        new_slots[i].key = slots[i].key;
        new_slots[i].value = slots[i].value;
    }
    std::cout << "6\n";
    std::cout << "About to execute delete" << std::endl;
    /* Don't know why delete statement cause crash. Before we figure out why,
     let's just comment it out and be careful with the unfreed memory. */
    // delete[] slots;
    std::cout << "7\n";
    slots = new_slots;
    std::cout << "8\n";
    indexer.resize(new_capacity);
    std::cout << "9\n";
    capacity = new_capacity;
    std::cout << "Finish dict resize routine" << std::endl;
}

template <class T, class S>
void
Dict<T, S>::clear() {
    // Empty the bitmap
    indexer.reset();
    size = 0;
}

template <class T, class S>
void
Dict<T, S>::reserve(unsigned size) {
    unsigned new_capacity = ceil(size * 1.0 / MAX_LOAD_FACTOR);
    resize(new_capacity);
}

// Move to extra_exception.h
class KeyError : public std::exception {
   public:
    KeyError() {}
    KeyError(const std::string& msg_) throw() : msg(msg_) {}
    virtual ~KeyError() throw() {}
    const char* what() const throw() { return msg.c_str(); }

   private:
    const std::string msg;
};

template <class T, class S>
unsigned
Dict<T, S>::find_bucket(const T& key) const {
    // We can be assured that the recurrence is going to iterate through all
    // slots. As long as that size < capacity, we will not go into infinite
    // loop.
    unsigned mask = capacity - 1;
    // wrap hash() call inside abs(), only positive value is needed
    unsigned h = std::abs(key.hash());
    unsigned perturb = h;
    unsigned index = h & mask;
    while (indexer.get(index) && slots[index].key != key) {
        // #if COLLECT_STATISTICS_MODE == 1
        //         collision_count++;
        // #endif
        perturb = perturb >> PERTURB_SHIFT;
        index = (5 * index + 1 + perturb) & mask;
    }
    // Can't find the bucket containing key, return next probe position.
    // Found the bucket containing the key, return the bucket's index
    // position.
    return index;
}

template <class T, class S>
void
Dict<T, S>::set(const T& key, const S& value) {
    std::cout << "Enter dict set routine" << std::endl;
    unsigned index = find_bucket(key);
    std::cout << "Find index: " << index << std::endl;
    // if (index == -1) {
    // std::cerr << "Dict doesn't have space for insertion" << std::endl;
    // throw runtime_error("Dict doesn't have space for insertion");
    // }
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
    std::cout << "Finish dict set routine" << std::endl;
}

// FIXME: possible infinite loop
template <class T, class S>
S
Dict<T, S>::get(const T& key) const {
    unsigned index = find_bucket(key);
    // if (index == -1) {
    // std::cerr << "Key not present in the dict" << std::endl;
    // throw KeyError("Key not present in the dict");
    // }
    if (indexer.get(index)) {
        return slots[index].value;
    } else {
        std::cerr << "Key not present in the dict" << std::endl;
        throw KeyError("Key not present in the dict");
    }
}

// FIXME: possible infinite loop
// If membership test is heavy operation, we can optionally use bloom filter
// to accelerate.
template <class T, class S>
bool
Dict<T, S>::contains(const T& key) const {
    unsigned index = find_bucket(key);
    // if (index == -1) {
    // return false;
    // }
    if (indexer.get(index)) {
        return true;
    } else {
        return false;
    }
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
    for (int i = 0; i < capacity; i++) {
        if (!indexer.get(i)) {
            continue;
        }
        if (slots[i].value == value) {
            return true;
        }
    }
    return false;
}

template <class T, class S>
void
Dict<T, S>::statistics() const {
#if COLLECT_STATISTICS_MODE == 1
    std::cout << "Collision count: " << collision_count << std::endl;
#else
    std::cout << "Collect-statistics-mode is not enabled" << std::endl;
#endif
}

template <class T, class S>
std::string
Dict<T, S>::str() const {
    std::string ret;
    ret += "{";
    for (int i = 0; i < capacity; i++) {
        if (!indexer.get(i)) {
            continue;
        }
        ret += slots[i].key.str() + ":" + slots[i].value.str() + ", ";
    }
    if (size) {
        ret.erase(ret.end() - 2, ret.end());
    }
    ret += "}";
    return ret;
}

#endif /* _DICT_H_ */
