#ifndef _BITMAP_H_
#define _BITMAP_H_

// When initialized, all bits are set to 0.

class Bitmap {
   public:
    Bitmap(unsigned size);
    ~Bitmap();
    void reset();
    unsigned length() const;
    void set(unsigned index, bool bit);
    bool get(unsigned index) const;
    void resize(unsigned new_size);

   private:
    unsigned char* storage;
    unsigned size;
    unsigned nbytes;
};

#endif /* _BITMAP_H_ */
