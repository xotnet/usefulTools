#ifndef printX
#define printX

#include <iostream>

// usage - print("Hello ", 25, " Its a Str", 0.0002f, "\n")
template <typename... T>
void print(T... args) {
    ((std::cout << args), ...);
}

#endif
