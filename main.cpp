#include <iostream>

#include "felix/String.hpp"

int main() {
    const auto str = felix::String{"hello world! ⭐"};

    std::cout << str << std::endl;
    
    for (const auto& i : str.graphemes()) {
        std::cout << i << '\n';
    }

    for (const auto& e : str) {
        std::cout << e << '\n';
    }

//    std::cout << str << std::endl;

//    assert(str.size() == 12);
//    assert(!str.is_small());
//    assert(str.use_count() == 1);
//
//    const auto str_copy = str;
//
//    assert(str_copy.use_count() == 2);
//
//    const auto foo = felix::String("hello");
//    assert(foo.is_small());
//    assert(foo.size() == 5);
//    assert(foo.use_count() == 0);
//
//    const auto bar = foo;
//    assert(bar.is_small());
//    assert(bar.size() == 5);
//    assert(bar.use_count() == 0);

    return 0;
}
