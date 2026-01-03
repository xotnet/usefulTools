template<typename T>
struct smartPtr {
    private:
    void memset (void* p, char o, unsigned int size) {
        for (int i = 0; i<size; ++i) {
            ((char*)p)[i] = o;
        }
    }

    public:

    T* ptr = nullptr; // pointer to data
    unsigned int size = 0; // count of elements!

    smartPtr(unsigned int sz) {
        ptr = new T[sz];
        size = sz;
    }
    ~smartPtr() {
        delete[] ptr;
    }

    T* get() {
        return (T*)ptr;
    }
    void erase() {
        unsigned int sizeInBytes = size * sizeof(T);
        memset(ptr, 0, sizeInBytes);
    }
    unsigned int getSizeBytes() {
        return sizeof(T) * size;
    }

    T& operator[](unsigned int index) {
        return ptr[index];
    }
};
