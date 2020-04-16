#include <cstdlib>

#include "dict.h"

class _dummy {};

template <class T>
class Set {
   public:
    void clear();
    void reserve(unsigned);
    void add(const T&);
    bool contains(const T&) const;
    // std::string str() const;

   private:
    Dict<T, _dummy> storage;
};

template <class T>
inline void
Set<T>::clear() {
    storage.clear();
}

template <class T>
inline void
Set<T>::reserve(unsigned size) {
    storage.reserve(size);
}

template <class T>
inline void
Set<T>::add(const T& elem) {
    storage.set(elem, _dummy());
}

template <class T>
inline bool
Set<T>::contains(const T& elem) const {
    return storage.contains(elem);
}
