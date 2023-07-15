#include <silk.h>
#include <iostream>
#ifdef _WIN32
#include <locale>
#endif

int main() {
// set output encode for windows
#ifdef _WIN32
    std::locale::global(std::locale("en_US.UTF-8"));
#endif
    std::cout << "Current silk library version:" << getVersion() << std::endl;
    return 0;
}