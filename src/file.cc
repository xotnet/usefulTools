#include <stdio.h>

struct file {
    bool isOpen = false;
    FILE* f = NULL;
    file(const char* name, const char* perm) {
        f = fopen(name, perm);
        if (f != NULL) {
            isOpen = true;
        }
    }
    ~file() {
        fclose(f);
    }

    unsigned int size() {
        if (f == NULL) {
            return 0;
        }
        fseek(f, 0, SEEK_END);
        int size = ftell(f);
        fseek(f, 0, SEEK_SET);
        return size;
    }
    
    int read(char* buf, const unsigned int size) {
        return fread(buf, size, 1, f);
    }
    int write(const char* buf, const unsigned int size) {
        return fwrite(buf, size, 1, f);
    }
};
