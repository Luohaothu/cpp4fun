#include <iostream>

int a = []() {
    std::cout << "a assigned" << std::endl;
    return 1;
}();