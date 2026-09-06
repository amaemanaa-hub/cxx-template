#include <iostream>
#include <vector>
#include "rust_lib/src/lib.rs.h"

int main() {
    int32_t value = get_int();
    std::cout << "get_int(" << value << ");" << std::endl;
    return 0;
}