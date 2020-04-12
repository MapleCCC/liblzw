#ifndef _DICT_H_
#define _DICT_H_

#include <cmath>
#include <stdexcept>

// TODO: add generic template
// WARNING: template declaration and definition can't be separated into two
// files.

#define INIT_DICT_SLOT_NUM 8
#define DICT_RESIZE_FACTOR \
    2  // Optionally we can dynamically change the value of DICT_RESIZE_FACTOR
#define MAX_LOAD_FACTOR 2 / 3

// Should we declare Bucket type inside Dict?
template <class T, class S>
struct Bucket {
    T key;
    S value;
    Bucket* next;
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
    void reserve(int);
    void set(const T& key, const S& value);
    S get(const T& key) const;
    bool contains(const T& key) const;
    bool contains_value(const S& value) const;
    void statistics() const;

   private:
    int size;
    int occupied_slot_num;
    int all_slot_num;
    Bucket<T, S>** slots;
    // Dict<S, NULL> values;
    // bool is_values_cached;
    // int collision_count;

    void resize(int);
    double load_factor() const;
};

template <class T, class S>
Dict<T, S>::Dict() {
    all_slot_num = INIT_DICT_SLOT_NUM;
    slots = new Bucket<T, S>*[all_slot_num];
    size = 0;
    for (int i = 0; i < this->all_slot_num; i++) {
        slots[i] = NULL;
    }
}

template <class T, class S>
Dict<T, S>::~Dict() {
    clear();
    delete slots;
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
    return size * 1.0 / all_slot_num;
}

// TODO
template <class T, class S>
void
Dict<T, S>::resize(int new_size) {
    return;
}

template <class T, class S>
void
Dict<T, S>::clear() {
    for (int i = 0; i < all_slot_num; i++) {
        Bucket<T, S>* p = slots[i];
        while (p != NULL) {
            Bucket<T, S>* temp = p->next;
            delete p;
            p = temp;
        }
        slots[i] = NULL;
        occupied_slot_num = 0;
    }
    size = 0;
}

/* Boundary Condition: Return NULL if not found */
template <class T, class S>
static Bucket<T, S>*
find_bucket(Bucket<T, S>* entry, T key) {
    Bucket<T, S>* p = entry;
    while (p != NULL) {
        if (p->key == key) {
            return p;
        }
        p = p->next;
    }
    return p;
}

template <class T, class S>
void
Dict<T, S>::set(const T& key, const S& value) {
    int h = abs(key.hash()) % all_slot_num;
    Bucket<T, S>* slot = slots[h];
    Bucket<T, S>* p = find_bucket(slot, key);
    if (p == NULL) {
        // WARNING: don't use malloc here. Why?
        Bucket<T, S>* new_bucket = new Bucket<T, S>;
        new_bucket->key = key;
        new_bucket->value = value;
        new_bucket->next = slots[h];
        slots[h] = new_bucket;
        size += 1;
        if (slot == NULL) {
            occupied_slot_num++;
        }
        if (load_factor() > MAX_LOAD_FACTOR) {
            resize(all_slot_num * DICT_RESIZE_FACTOR);
        }
    } else {
        p->value = value;
    }
}

template <class T, class S>
S
Dict<T, S>::get(const T& key) const {
    int h = abs(key.hash()) % all_slot_num;
    Bucket<T, S>* p = find_bucket(slots[h], key);
    if (p == NULL) {
        throw out_of_range("key is not present");
    } else {
        return p->value;
    }
}

// If membership test is heavy operation, we can optionally use bloom filter to
// accelerate.
template <class T, class S>
bool
Dict<T, S>::contains(const T& key) const {
    int h = abs(key.hash()) % all_slot_num;
    Bucket<T, S>* p = find_bucket(slots[h], key);
    return bool(p);
}

// Currently a naive linear search. May be expensive and inefficient for large
// dict. Consider use another O(1) implementation. Trade space for time.
// More optionally, we can trade not that much space for not that much time, use
// probablistic data structure like bloom filter. If we can't find it in bloom
// filter, it must not appear before. If we can find it in bloom filter, we need
// to iterate over the slots to check if it really no appears.
template <class T, class S>
bool
Dict<T, S>::contains_value(const S& value) const {
    for (int i = 0; i < all_slot_num; i++) {
        Bucket<T, S>* p = slots[i];
        while (p != NULL) {
            if (p->value == value) {
                return true;
            }
            p = p->next;
        }
    }
    return false;
}

template <class T, class S>
void
Dict<T, S>::statistics() const {
    for (int i = 0; i < slots.size(); i++) {
    }
}

#endif /* _DICT_H_ */
